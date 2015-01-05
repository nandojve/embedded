/******************************************************************************
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
*******************************************************************************/
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : lcd_pmod.h
* Description  : This Header file contains the Macro Definitions & prototypes
*                for the functions used in lcd.c
*
*  This function is created to drive the Okaya LCD display with either ST7735 
*  or ST7715 driver device. The commands for both the devices are the same.
*
*  The display is controlled using the SPI bus. In this example, the SCI5 is 
*  used. This can be modified to the SCI connected to the PMOD interface. 
*  The SCI driver file will also be required. 
*
*  The display memory has an offset with respect to the actual pixel. 
*  This is not documented but realised from driving the display. 
*  The offset is set as LEFT MARGIN and TOP MARGIN. this offset is catered for 
*  internally, so as far as the user is concerned, cursor position 0,0 is the 
*  top left pixel.
* 
*  The simplest procedure to run the display is as follows:
*       Init_LCD(); Initialise the serial port and set up the display. 
*                   Clear the display.
*                   The font colour is set to white and background colour to black.
*       DisplaySetFontColour(COL_YELLOW);   set the font colour to desired colour
*       DisplaySetBackColour(COL_BLUE);     set the background colour to desired value
*       DisplayCenter(1,"Renesas");         write a title on line 1 of the display.
*                                           Note: Line 0 is the top line.
***********************************************************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*******************************************************************************/

/***********************************************************************************************************************
Macro Definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef LCD_H
#define LCD_H

/***********************************************************************************************************************
User Includes (Project Level Includes)
***********************************************************************************************************************/
/* Defines standard variable types used in this file */
#include <stdint.h>

/***********************************************************************************************************************
Macro Definitions for Okaya display on PMOD connector
***********************************************************************************************************************/
#include "iodefine.h"

/***********************************************************************************************************************
*
*  SCREEN
*
*  The screen size is 128 x 128 pixels, with coordinate 0,0 at the top left.
*  The display controller is ST7735 or ST7715.
*
***********************************************************************************************************************/
#define SCREEN_HEIGHT 128            /* 16 lines @ 8 bits = 128. */
#define SCREEN_WIDTH  128           

/* DATA/COMMAND select pin */
#define DATA_CMD_PIN              (PORT4.Pn.BIT.Pn15)

/* DATA/COMMAND select pin */
#define DATA_CMD_PIN_DIR          (PORT4.PMn.BIT.PMn15)

/* Pin defines common to both displays */

#ifndef USE_PMOD2
/* Display/enable pin */
#define ENABLE_PIN                (PORT1.Pn.BIT.Pn4)
/* Display enable pin */
#define ENABLE_PIN_DIR            (PORT1.PMn.BIT.PMn4)
#else
/* Display/enable pin */
#define ENABLE_PIN                (PORT1.Pn.BIT.Pn5)
/* Display enable pin */
#define ENABLE_PIN_DIR            (PORT1.PMn.BIT.PMn5)
#endif

/* Vbat enable pin */
#define VBAT_ENABLE_PIN           (PORT4.PMn.BIT.PMn15)

/* Vbat enable pin */
#define VBAT_ENABLE_PIN_DIR       (PORT4.PMn.BIT.PMn15) 

/* Vdd enable pin */
#define BL_ENABLE_PIN             (PORT3.Pn.BIT.Pn7) 

/* Vdd enable pin */
#define VDD_ENABLE_PIN_DIR        (PORT3.Pn.BIT.Pn7) 

/* Automatic calculation of parameters */
#define FONT_WIDTH                (6u)   /* including a space */
#define FONT_HEIGHT               (8u)   /* including 1 pixel space */
#define MAX_LINES                 (SCREEN_HEIGHT / FONT_HEIGHT)
#define CHAR_PER_LINE             (SCREEN_WIDTH / FONT_WIDTH)

#define LEFT_MARGIN (2u)             /* Allow 2 pixel margin on the left and the top */
#define TOP_MARGIN  (3u)
#define CR 0x0d
#define LF 0x0a
#define BS 0x08


/***********************************************************************************************************************

  DISPLAY COLOUR DEFINITIONS (16 bits) R5G6B5 format

  Only Primary & secondary colours are defined here. Other colours can be created using 
  RGB values.
***********************************************************************************************************************/
#define COL_BLACK       0X0000
#define COL_RED         0Xf800
#define COL_GREEN       0X07E0
#define COL_BLUE        0X001F
#define COL_YELLOW      0XFFE0    
#define COL_CYAN        0X07FF
#define COL_MAGENTA     0XF81F
#define COL_WHITE       0XFFFF

/***********************************************************************************************************************

  DISPLAY COMMAND SET ST7735

***********************************************************************************************************************/
#define ST7735_NOP      (0x0)
#define ST7735_SWRESET  (0x01)
#define ST7735_SLPIN    (0x10)
#define ST7735_SLPOUT   (0x11)
#define ST7735_PTLON    (0x12)
#define ST7735_NORON    (0x13)
#define ST7735_INVOFF   (0x20)
#define ST7735_INVON    (0x21)
#define ST7735_DISPOFF  (0x28)
#define ST7735_DISPON   (0x29)
#define ST7735_CASET    (0x2A)
#define ST7735_RASET    (0x2B)
#define ST7735_RAMWR    (0x2C)
#define ST7735_COLMOD   (0x3A)
#define ST7735_MADCTL   (0x36)
#define ST7735_FRMCTR1  (0xB1)
#define ST7735_INVCTR   (0xB4)
#define ST7735_DISSET5  (0xB6)
#define ST7735_PWCTR1   (0xC0)
#define ST7735_PWCTR2   (0xC1)
#define ST7735_PWCTR3   (0xC2)
#define ST7735_VMCTR1   (0xC5)
#define ST7735_PWCTR6   (0xFC)
#define ST7735_GMCTRP1  (0xE0)
#define ST7735_GMCTRN1  (0xE1)

/* delay for delay counter */
#define DELAY_TIMING              (8)

/***********************************************************************************************************************
Function Prototypes
***********************************************************************************************************************/
/* Initialises the debug LCD */
void R_LCD_Init (void);

/* Display string at specific line of display */
void Display_LCD (uint8_t const position,  uint8_t * const string);

/* Display the string at current cursor position */
void DisplayStr (uint8_t * str);

/* Display the sting at the center of the specified line */
void DisplayCenter (uint8_t const line_num, uint8_t * const str);

/* Clears the display */
void Clear_Display (uint16_t colour);

/* Clear a specified line */
void DisplayClearLine(uint8_t line_num);

/* Set the current cursor position */
void DisplaySetCursor (uint8_t const x, uint8_t const y);

/* Delay function */
void DisplayDelay (uint32_t const units);
void DisplayDelayMs (uint32_t  time_ms);

/* Set Font colour */
void DisplaySetFontColour (uint16_t const col);

/* Set Background colour */
void DisplaySetBackColour (uint16_t const col);

/* Simple image blit */
void Display_Image (uint8_t *image, uint8_t image_width, uint8_t image_height, uint8_t loc_x, uint8_t loc_y);

/* Enable display */
void Display_On (void);

/* Disable display */
void Display_Off (void);

#endif

