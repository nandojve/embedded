/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : lcd.c
* Device(s)     : RZ/A1H RSK+RZA1H
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : LCD Module utility functions.
* Please refer to the header file lcd.h for detail explanation
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Includes assembly level definitions */
#include <string.h>

#include "r_typedefs.h"                   /* Default  type definition header */
#include "iodefine.h"                            /* I/O Register root header */
#include "ascii.h"                                      /* ASCII font header */
#include "rspi.h"                               /* rspi Device Driver header */
#include "dev_drv.h"                                 /* Device driver header */
#include "sample_riic_rza1h_rsk_i2c.h"   /* I2C RSK+RZA1H Eval-board header */
#include "lcd_pmod.h"   /* PMOD LCD controlling function prototypes & macros */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* LCD cursor co-ordinates */
uint8_t curx = 0;
uint8_t cury = 0;

/* set m_InverseMode to 1 to invert display */
uint8_t m_InverseMode = 0;

uint32_t g_units = 0u;
uint32_t g_cmd = 0u;

uint8_t g_data_block_content = 0u;

uint16_t font_colour = COL_BLACK;
uint16_t back_colour = COL_WHITE;

/***********************************************************************************************************************
* Local Function Prototypes
***********************************************************************************************************************/
static void init_pmod_lcd (void);

static void charput (uint8_t const val);
static void display_write_command (uint8_t const  cmd);
static void display_write_data (uint8_t data);
static void display_write_pixel (uint8_t const x, uint8_t const y, uint16_t const colour);
static void display_draw_horz_line (uint8_t const line, uint16_t const  colour);
static void display_set_addr_window (uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye);
static void display_write_char_column (uint8_t const x, uint8_t const y, uint8_t const pattern);
static void display_write_data_multi (uint16_t const data, int16_t count);
static void display_write_image (uint8_t const *data, int16_t count);


/***********************************************************************************************************************
* Function Name : R_LCD_Init
* Description   : Initialises the serial port and the LCD display. Clear the display with black background.
*                 Set the font colour to white.
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
void R_LCD_Init (void)
{
    /* initialise Standard PMOD display */
    init_pmod_lcd();
}
/***********************************************************************************************************************
* End of function Init_LCD
***********************************************************************************************************************/
/**********************************************************************************************************************
* Function Name : Display_LCD
* Description   : This function controls LCD writes to the start of the line  of the LCD.
*                 specified by position of the LCD. If position is greater than the
*                 number of lines available then the LCD write will be to the last line.
* Argument      : (uint8_t)position -   Line number of display
*                 (const char *) string -Pointer to data to be written to
*                                display. Last character should be null.
* Return value  : none
***********************************************************************************************************************/
void Display_LCD (uint8_t const position,  uint8_t * const string)
{
    DisplaySetCursor(0, position);
    DisplayStr(string);
}
/***********************************************************************************************************************
* End of function Display_LCD
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Display_On
* Description   : Enable Display (display can be written to whiles disabled)
* Argument      : None
* Return value  : None
***********************************************************************************************************************/
void Display_On (void)
{
    display_write_command(ST7735_DISPON);   /* display on */
    DisplayDelayMs(120);  /* delay requirement stated in data sheet */
}
/***********************************************************************************************************************
* End of function Display_On
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Display_Off
* Description   : Disable Display (display can be written to whiles disabled)
* Argument      : None
* Return value  : None
***********************************************************************************************************************/
void Display_Off (void)
{
    display_write_command(ST7735_DISPOFF);   /* display off */
    DisplayDelayMs(120);  /* delay requirement stated in data sheet */

}
/***********************************************************************************************************************
* End of function Display_Off
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : DisplayStr(const uint8_t *string)
* Description   : Displays a string at the current cursor position
* Argument      : char *string - pointer to a string
* Return value  : none
***********************************************************************************************************************/
void DisplayStr (uint8_t * str)
{
    uint16_t i;
    uint16_t size;

    size = (uint16_t)strlen((const char *)str);
    /* load characters into screen bitmap */
    for (i = 0; i < size; i++)
    {
        charput(*str++);
    }
}
/***********************************************************************************************************************
* End of function DisplayStr
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : DisplayCenter
* Description   : Display a string in the center of the specified line.
* Argument      : uint8_t line_num - line number (0-15)
*                 pointer to a string.
* Return value  : none
***********************************************************************************************************************/
void DisplayCenter (uint8_t const line_num, uint8_t * const str)
{
    uint8_t spaces;

    DisplaySetCursor(0, line_num);

    spaces = (uint8_t)((CHAR_PER_LINE - strlen((const char *)str)) / 2);
    while (spaces--)
    {
        charput(' ');
    }
    DisplayStr(str);
}
/***********************************************************************************************************************
* End of function DisplayCenter
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Clear_Display
* Description   : This function clears the screen
* Argument      : uint16_t colour - in R5G6B5 format
* Return value  : none
***********************************************************************************************************************/
void Clear_Display (uint16_t colour)
{
    display_set_addr_window(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

    /* Write to RAM */
    display_write_command(ST7735_RAMWR);

    display_write_data_multi(colour, (SCREEN_HEIGHT) * (SCREEN_WIDTH) );

    /* Cursor to Home Position */
    DisplaySetCursor(0, 0);
}
/***********************************************************************************************************************
* End of function Clear_Display
***********************************************************************************************************************/

#define BMP_OFFSET (10u)
/***********************************************************************************************************************
* Function Name : Display_Image
* Description   : This function puts a 24-bit image on the 16-bit display, colour reduction is embedded in display
*                 routine.
* Argument      : uint8_t  *image       - 24 bit RGB888 format RAW data no header
*                 uint16_t image_width  - Target image width
*                 uint16_t image_height - Target image height
*                 uint16_t loc_x        - Image horizontal position on display
*                 uint16_t loc_y        - Image vertical position on display
* Return value  : none
***********************************************************************************************************************/
void Display_Image (uint8_t *image, uint8_t image_width, uint8_t image_height, uint8_t loc_x, uint8_t loc_y)
{
    uint8_t y = image[BMP_OFFSET];
    for(y = 0; y < image_height; y++)
    {
        display_set_addr_window((uint8_t)(loc_x), (uint8_t)(loc_y + y), (uint8_t)(image_width - 1), (uint8_t)(loc_y + image_height - 1));
        display_write_command(ST7735_RAMWR);
        display_write_image(image[BMP_OFFSET] + image + ((image_height - y - 1) * image_width * 3), (int16_t)(image_width * 3));
    }
}
/***********************************************************************************************************************
* End of function Clear_Display
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : DisplayClearLine
* Description   : Clear the specified line
* Argument      : uint8_t line numb 0 - 15
* Return value  : none
***********************************************************************************************************************/
void DisplayClearLine (uint8_t line_num)
{
    uint8_t i;
    uint8_t y;    
    
    if (line_num < MAX_LINES)
    {
        for (i = 0; i < FONT_HEIGHT; i++)
        {
        y = (uint8_t)((line_num * FONT_HEIGHT) + i);    
            display_draw_horz_line(y, back_colour);
        }
        DisplaySetCursor(0, line_num);
    }
}
/***********************************************************************************************************************
* End of function DisplayClearLine
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : DisplaySetCursor(uint8_t x, uint8_t y)
* Description   : Set the cursor to the specified position. the position must be
*                 within the limits.
* Argument      : uint8_t x - X position
*                         y - Y position
* Return value  : none
***********************************************************************************************************************/
void DisplaySetCursor (uint8_t const x, uint8_t const y)
{
    if (x < CHAR_PER_LINE)
    {
        curx = x;
    }

    if (y < MAX_LINES)
    {
        cury = y;
    }
}
/***********************************************************************************************************************
* End of function DisplaySetCursor
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : DisplaySetFontColour(uint16_t col)
* Description   : Set the font (foreground) colour
* Argument      : uint16_t col - colour value in r5g6b5 format
* Return value  : none
***********************************************************************************************************************/
void DisplaySetFontColour (uint16_t const col)
{
    font_colour = col;
}
/***********************************************************************************************************************
* End of function DisplaySetFontColour
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : DisplaySetBackColour(uint16_t col)
* Description   : Set the background colour
* Argument      : uint16_t col - colour value in r5g6b5 format
* Return value  : none
***********************************************************************************************************************/
void DisplaySetBackColour (uint16_t const col)
{
    back_colour = col;
}
/***********************************************************************************************************************
* End of function DisplaySetBackColour
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : DisplayDelay
* Description   : Delay routine for LCD or any other devices.
* Argument      : (uint32_t) units - time in, approximately, microsec
* Return value  : None
***********************************************************************************************************************/
void DisplayDelay (uint32_t const units)
{
    uint32_t counter = units * DELAY_TIMING;
    
    /* Decrement the counter and wait */
    while (counter--)
    {
        __asm__("nop");
    }
}
/***********************************************************************************************************************
* End of function DisplayDelay
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : DisplayDelayMs
* Description   : Delay routine for LCD or any other devices.
* Argument      : (uint32_t) time_ms - time in milli second
* Return value  : None
***********************************************************************************************************************/
void DisplayDelayMs (uint32_t  time_ms)
{
    while (time_ms)
    {
		DisplayDelay(1000);    /* 1 ms */
		time_ms--;
    }
}
/***********************************************************************************************************************
* End of function DisplayDelayMs
***********************************************************************************************************************/


/***********************************************************************************************************************
* Internal Functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : init_pmod_lcd
* Description   : Initialises the LCD display.
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void init_pmod_lcd (void)
{
    /* Preset the output ports for i/o pins prior to setting as outputs */
    DATA_CMD_PIN = 0x1;
    ENABLE_PIN  = 0x1;

    BL_ENABLE_PIN = 0x0;        /* back-light off */

   /* PMOD1 reset (IO[4]) is to be modified and its output value set high. */
    Sample_RIIC_PortExpAccess(DEVDRV_CH_3, RZA1H_RSK_I2C_PX_IO1, (uint8_t)PORTX_OUTPUT_REG, 0xFF, 0xB7);

    DisplayDelayMs(10);

    Display_Off();

    /* PMOD1 reset (IO[4]) is to be modified and its output value set low. */
#ifndef USE_PMOD2
    Sample_RIIC_PortExpAccess(DEVDRV_CH_3, RZA1H_RSK_I2C_PX_IO1, (uint8_t)PORTX_OUTPUT_REG, (uint8_t)0xFF, (uint8_t)0xA7);
#else
    Sample_RIIC_PortExpAccess(DEVDRV_CH_3, RZA1H_RSK_I2C_PX_IO1, (uint8_t)PORTX_OUTPUT_REG, (uint8_t)0xFF, (uint8_t)0x97);
#endif

    /* Wait 10 ms */
    DisplayDelayMs(10);

    /* PMOD1 reset (IO[4]) is to be modified and its output value set high. */
    Sample_RIIC_PortExpAccess(DEVDRV_CH_3, RZA1H_RSK_I2C_PX_IO1, (uint8_t)PORTX_OUTPUT_REG, (uint8_t)0xFF, (uint8_t)0xB7);

    display_write_command(ST7735_SWRESET);  /* software reset */
    DisplayDelayMs(120);
    
    display_write_command(ST7735_SLPOUT);   /* out of sleep mode */
    DisplayDelayMs(120);
    
    display_write_command(ST7735_COLMOD);   /* set color mode */
    display_write_data(0x05);               /* 16 bit colour */
    DisplayDelayMs(10);
 
    display_write_command(ST7735_MADCTL);   /* memory access control (directions) */
    display_write_data(0xC8);               /* row address/col address, bottom to top refresh */
    
    display_write_command(ST7735_NORON);   /* normal mode */
    DisplayDelayMs(10);
    
    Clear_Display(back_colour);

    display_write_command(ST7735_DISPON);   /* display on */
    DisplayDelayMs(120);
    
    BL_ENABLE_PIN = 0x01;    /* enable back-light */
}
/***********************************************************************************************************************
* End of function init_pmod_lcd
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : charput
* Description   : Displays a character at the current cursor position.
*                 Advances the cursor by 1 position.
*                 Cursor wraps to next line at the end of the line.
*                 Cursor wraps to Home position at the end of the display.
*
*                 The following control codes are accepted:
*
*                 LF increments y cursor position
*                 CR resets x cursor to 0.
*                 BS moves x cursor back by 1 place if possible.
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void charput (uint8_t const val)
{
    uint8_t * ptr;
    uint8_t   i;
    uint8_t   pattern;
    uint8_t   x;
    uint8_t   y;
    
    switch (val)
    {
        /* Carriage return character */
        case CR:
        {
            curx = 0;
        }    
        break;

        /* Line feed character */
        case LF:
        {
            cury++;
            if (cury >= MAX_LINES)
            {
                cury = 0;
            }
        }    
        break;

        /* Backspace character */
        case BS:
        {
            if (curx > 0)
            {
                curx--;
            }
        }
        break;

        /* Normal displayable characters */
        default:
        {
            /* Ensure value is within the ASCII range */
            if ((val >= 0x20) && (val <= 0x7f))
            {
                /* Get pointer to start of representation of character in ASCII table */
                ptr = (uint8_t *)&ASCII_TABLE[val - 0x20][0];
                
                /* Copy character over */
                for (i = 0; i < FONT_WIDTH; i++)
                {
                    pattern = *ptr++;
                    x = (uint8_t)((curx * FONT_WIDTH) + i);
                    y = (uint8_t)(cury * FONT_HEIGHT);
                    display_write_char_column(x, y, pattern);
                }
                
                /* move cursor to next co-ordinate on LCD */
                curx++;

                /* move to next row if reached the end of line */
                if (curx >= CHAR_PER_LINE)
                {
                    curx = 0;
                    cury++;
                    if (cury >= MAX_LINES)
                    {
                        /* loop back to top line if the last line of the display is reached */
                        cury = 0;
                    }
                }
            }
        }
        break;
    }
}
/***********************************************************************************************************************
* End of function charput
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : display_write_command
* Description   : Encapsulation of autocode SPI5 send, sends 1 cmd byte.
* Argument      : cmd - Command to write
* Return value  : None
* Note          : DATA_CMD_PIN is set lo for command 
***********************************************************************************************************************/
static void display_write_command (uint8_t const cmd)
{
    uint16_t trans_data = 0u;

    UNUSED_VARIABLE(trans_data);

    /* data cmd pin low to indicate a command */
    DATA_CMD_PIN = 0x0;
    
    /* assert chip select */
    ENABLE_PIN  = 0x0;
    
    g_cmd = (uint32_t)cmd;

    SPDR_1L = (uint16_t)(cmd << 8u);

    while (0u == (SPSR_1 & 0x80));

    /* check if number of receive data is reached */
    if (0u != (SPSR_1 & 0x80))
    {
        /* Read receiver buffer */
        trans_data = SPDR_1L;
    }

    if (0u != (SPSR_1 & 0x01))
    {
        while(1)
        {
            /* Overrun error occurred */
        };
    }

    /* de-assert chip select */
    ENABLE_PIN  = 0x1;

    /* data cmd pin high to signify data */
    DATA_CMD_PIN = 0x1;

}
/***********************************************************************************************************************
* End of function display_write_command
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name : display_write_data
* Description   : Write a data byte to the display
* Argument      : data - data to write
* Return value  : None
* Note          : DATA_CMD_PIN is set hi for data. 
***********************************************************************************************************************/
static void display_write_data (uint8_t const data)
{
    uint16_t trans_data = 0u;

    UNUSED_VARIABLE(trans_data);

    /* data cmd pin high to signify data */
    DATA_CMD_PIN = 0x1;
    
    /* assert chip select */
    ENABLE_PIN  = 0x0;
    
    g_data_block_content = data;

    /* send command */
    SPDR_1L = (uint16_t)(g_data_block_content << 8u);

    while (0u == (SPSR_1 & 0x80));
	
    /* check if number of receive data is reached */
    if ((uint8_t)0u != (SPSR_1 & 0x80))
    {
        /* Read receiver buffer */
        trans_data = SPDR_1L;
    }

    if (0u != (SPSR_1 & 0x01))
    {
        while(1)
        {
            /* Overrun error occurred */
        };
    }

    /* de-assert chip select */
    ENABLE_PIN  = 0x1;

    /* data cmd pin high to signify data */
    DATA_CMD_PIN = 0x1;
}
/***********************************************************************************************************************
* End of function display_write_data
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : display_write_data_multi
* Description   : Write a data byte to the display
* Argument      : data - data to write
* Return value  : None
* Note          : DATA_CMD_PIN is set hi for data.
***********************************************************************************************************************/
static void display_write_data_multi (uint16_t const data, int16_t count)
{
    uint16_t trans_data = 0u;

    UNUSED_VARIABLE(trans_data);

    /* data cmd pin high to signify data */
    DATA_CMD_PIN = 0x1;

    /* assert chip select */
    ENABLE_PIN  = 0x0;

    while(count-- > 0)
    {
        g_data_block_content = (uint8_t)(data>>8);

        /*send command */
        SPDR_1L = (uint16_t)(g_data_block_content << 8u);

        while (0u == (SPSR_1 & 0x80));

        /* check if number of receive data is reached */
        if (0u != (SPSR_1 & 0x80))
        {
            /* Read receiver buffer */
            trans_data = SPDR_1L;
        }

        if (0u != (SPSR_1 & 0x01))
        {
            while(1)
            {
                /* Overrun error occurred */
            };
        }
        g_data_block_content = (uint8_t)data;

        /*send command */
        SPDR_1L = (uint16_t)(g_data_block_content << 8u);

        while (0u == (RSPI1.SPSR & 0x80));

        //check if number of receive data is reached
        if (0u != (RSPI1.SPSR & 0x80))
        {
            /* Read receiver buffer */
            trans_data = SPDR_1L;
        }

        if (0u != (RSPI1.SPSR & 0x01))
        {
            while(1)
            {
                /* Overrun error occurred */
            };
        }
    }
    /* de-assert chip select */
    ENABLE_PIN  = 0x1;

    /* data cmd pin high to signify data */
    DATA_CMD_PIN = 0x1;
}
/***********************************************************************************************************************
* End of function display_write_data_multi
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : display_write_image_line
* Function Name : Display_Image
* Description   : This function puts a single line of a 24-bit image on the 16-bit display,
*                 colour reduction to 16-bit is embedded in function.
* Argument      : uint8_t  *data  - pointer to image data in RGB888 format RAW data no header
*                 uint16_t count  - Target image width * 3 (1 byte per colour (RGB) per pixel)
* Return value  : none
* Note          : DATA_CMD_PIN is set hi for data.
***********************************************************************************************************************/
static void display_write_image (uint8_t const *data, int16_t count)
{
    uint16_t trans_data = 0u;
    uint16_t idata = 0u;
    uint8_t r,g,b;

    UNUSED_VARIABLE(trans_data);

    /* data cmd pin high to signify data */
    DATA_CMD_PIN = 0x1;

    /* assert chip select */
    ENABLE_PIN  = 0x0;

    while(count > 0)
    {
        r = (*(data++)) >>3;
        g = (*(data++)) >>2;
        b = (*(data++)) >>3;
        count = (int16_t)(count - 3);

        idata  = (uint16_t)((b<<11) + (g<<5) + r);

        g_data_block_content = (uint8_t)(idata>>8);

        /*send command */
        SPDR_1L = (uint16_t)(g_data_block_content << 8u);

        while (0u == (SPSR_1 & 0x80));

        /* check if number of receive data is reached */
        if (0u != (SPSR_1 & 0x80))
        {
            /* Read receiver buffer */
            trans_data = SPDR_1L;
        }

        if (0u != (SPSR_1 & 0x01))
        {
            while(1)
            {
                /* Overrun error occurred */
            };
        }

        g_data_block_content = (uint8_t)idata;

        /*send command */
        SPDR_1L = (uint16_t)(g_data_block_content << 8u);

        while (0u == (SPSR_1 & 0x80));

        //check if number of receive data is reached
        if (0u != (SPSR_1 & 0x80))
        {
            /* Read receiver buffer */
            trans_data = SPDR_1L;
        }

        if (0u != (SPSR_1 & 0x01))
        {
            while(1)
            {
                /* Overrun error occurred */
            };
        }
    }
    /* de-assert chip select */
    ENABLE_PIN  = 0x1;

    /* data cmd pin high to signify data */
    DATA_CMD_PIN = 0x1;
}
/***********************************************************************************************************************
* End of function display_write_image
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : display_write_pixel
* Description   : Write the colour to the current cursor position
* Argument      : x,y - pixel position
*          colour in R5G6B5 format
* Return value  : None
***********************************************************************************************************************/
static void display_write_pixel (uint8_t const x, uint8_t const y, uint16_t const colour)
{
    uint8_t x1;
    uint8_t y1;
    
    x1 = (uint8_t)(x + 1);
    y1 = (uint8_t)(y + 1);
    display_set_addr_window(x, y, x1, y1);
    display_write_command(ST7735_RAMWR);  
    display_write_data((uint8_t)(colour >> 8));  
    display_write_data((uint8_t)(colour));
}

/***********************************************************************************************************************
* End of function display_write_pixel
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : display_draw_horz_line
* Description   : Draw horz line with the specified colour
* Argument      : line - line number to draw
*          colour in R5G6B5 format    
* Return value  : None
***********************************************************************************************************************/
static void display_draw_horz_line (uint8_t const line, uint16_t const colour)
{
    uint8_t i;
    uint8_t x;
    uint8_t y;
    
    if (line < SCREEN_HEIGHT)
    {
    x = SCREEN_WIDTH - 1;
    y = (uint8_t)(line + 1);
        display_set_addr_window(0, line, x, y);
        display_write_command(ST7735_RAMWR);
        
        for (i = 0; i < SCREEN_WIDTH; i++)
        {
            display_write_data((uint8_t)(colour >> 8));  
            display_write_data((uint8_t)(colour));
        }        
    }
}
/***********************************************************************************************************************
* End of function display_draw_horz_line
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : display_set_addr_window
* Description   : Draw horz line with the specified colour
* Argument      : xs, ys start pixel position
*                 ys, ye end pixel position
* Return value  : None
***********************************************************************************************************************/
static void display_set_addr_window (uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye)
{
    if (ys <= ye && xs <= xe && ye < SCREEN_HEIGHT && xe < SCREEN_WIDTH)
    {
        xs = (uint8_t)( xs + LEFT_MARGIN);
        xe = (uint8_t)( xe + LEFT_MARGIN);
        ys = (uint8_t)( ys + TOP_MARGIN);
        ye = (uint8_t)( ye + TOP_MARGIN);
        
        display_write_command(ST7735_CASET);   /* column addr set */
        display_write_data(0x00);
        display_write_data(xs);                /* XSTART */ 
        display_write_data(0x00);
        display_write_data(xe);                /* XEND   */

        display_write_command(ST7735_RASET);   /* row addr set */
        display_write_data(0x00);
        display_write_data(ys);                /* YSTART */
        display_write_data(0x00);
        display_write_data(ye);                /* YEND   */
        
    }
}
/***********************************************************************************************************************
* End of function display_set_addr_window
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : display_write_char_column
* Description   : Write a character at a current cursor position
* Argument      : x,y pixel position
*                 pattern to draw 1 = font colour, 0 = back colour
* Return value  : None
***********************************************************************************************************************/
static void display_write_char_column (uint8_t const x, uint8_t const y, uint8_t const pattern)
{
    uint8_t mask;
    uint8_t i;
    uint8_t y1;
    
    mask = 0x80;
    for (i = 0; i < FONT_HEIGHT; i++)
    {
        y1 = (uint8_t)(y + i);
        if (pattern & mask)
        {
            display_write_pixel(x, y1,font_colour);
        }
        else
        {
            display_write_pixel(x, y1,back_colour);
        }
        mask = (uint8_t)(mask >> 1);
    }
}
/***********************************************************************************************************************
* End of function display_write_char_column
***********************************************************************************************************************/




