/**
 * @file pal_lcd.c
 *
 * @brief PAL board specific functionality
 *
 * This file implements PAL board specific functionality.
 *
 * $Id: pal_lcd.c 31419 2012-03-23 07:45:21Z prashanth.udumula $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2011, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include "pal.h"
#include "pal_lcd.h"

/* === GLOBALS ============================================================= */

static int16_t temp_x = 0;
static int16_t temp_y = 0;

static uint8_t atmel[] =
{
    0x00, 0x00, 0x80, 0xe0, 0xf8, 0xfa, 0xfb, 0x03, 0xff, 0xff, 0x03, 0xfb, 0xfb, 0x0b,
    0xfb, 0xfb, 0x0b, 0xfb, 0xf3, 0x03, 0xfb, 0xfb, 0xdb, 0xdb, 0xdb, 0xdb, 0x03, 0xfb,
    0xfb, 0x03, 0x03, 0x03, 0x34, 0x36, 0x37, 0x31, 0x30, 0x37, 0x37, 0x30, 0x37, 0x37,
    0x30, 0x37, 0x37, 0x30, 0x37, 0x37, 0x30, 0x37, 0x37, 0x30, 0x37, 0x37, 0x36, 0x36,
    0x36, 0x36, 0x30, 0x37, 0x37, 0x16, 0x06, 0x02
};

/* @brief picture memory buffer */

static uint8_t page_0_picture_buffer[128] = {0};
static uint8_t page_1_picture_buffer[128] = {0};
static uint8_t page_2_picture_buffer[128] = {0};
static uint8_t page_3_picture_buffer[128] = {0};
static uint32_t extracted_data = 0;
static uint8_t flag = 1;

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief LCD and CONTROL port initialization
 */
void lcd_port_init()
{
    LCD_CONTROL_DIR = LCD_CONTROL_CONFIG;
    LCD_DATA_DIR = LCD_DATA_DIR_CONFIG;
}

/**
 * @brief LCD initialization
 */
void lcd_init()
{
    lcd_port_init();
    LCD_CS1_LOW;
    LCD_RES_LOW;
    /* A small delay to reset the LCD */
    pal_timer_delay(100);
    LCD_RES_HIGH;
    LCD_CS1_HIGH;
    lcd_send_command(LCD_CMD_SET_BIAS_9);
    lcd_send_command(LCD_CMD_SET_ADC_REVERSE);
    lcd_send_command(LCD_CMD_SET_COM_NORMAL);
    lcd_send_command(LCD_CMD_SET_RESISTOR_RATIO);
    lcd_send_command(LCD_CMD_SET_VOLUME_FIRST);
    lcd_send_command(LCD_CMD_SET_VOLUME_SECOND | LCD_SET_BRIGHTNESS);
    lcd_send_command(LCD_CMD_CONTRAST);
    lcd_send_command(LCD_SET_VOLTAGE_SETTINGS);
    /* A small delay for the voltage to stabilize */
    pal_timer_delay(100);
    lcd_send_command(LCD_CMD_DISPLAY_ON);
    lcd_send_command(LCD_CMD_DISPLAY_REVERSE);
    lcd_clr();
    array_copy();
}

/**
 * @brief Displays the text data in the array on to the LCD
 *
 * @param array  The arrey pointer containing the text to be displayed
 * @param length The length of the passed array
 * @param row    The row in which the text is to be displayed
 */
void lcd_text_display(uint8_t *array, uint16_t length, uint8_t row)
{
    uint8_t collumn;
    if (row == 0)
    {
        row = 6;
    }
    else if (row == 1)
    {
        row = 7;
    }
    else if (row == 2)
    {
        row = 0;
    }
    else if (row == 3)
    {
        row = 1;
    }

    lcd_send_command(LCD_CMD_SET_PAGE | row);
    if (length > 17)
    {
        length = 17;
    }
    length = (length * CHARACTER_WIDTH) + 20;
    for (collumn = 20; collumn <= length; collumn++)
    {
        lcd_send_command(LCD_CMD_SET_COLUMN_LOWER | (collumn & 0x0f));
        lcd_send_command(LCD_CMD_SET_COLUMN_UPPER | (collumn & 0xf0));
        lcd_send_data(*(array + (collumn - 20)));
    }
}

/**
 * @brief Clears the LCD display
 */
void lcd_clr(void)
{
    uint8_t row, collumn;
    for (row = 0; row <= 1; row++)
    {
        lcd_send_command(LCD_CMD_SET_PAGE | row);
        for (collumn = 20; collumn < 148; collumn++)
        {
            lcd_send_command(LCD_CMD_SET_COLUMN_UPPER | (collumn & 0xf0));
            lcd_send_command(LCD_CMD_SET_COLUMN_LOWER | (collumn & 0x0f));
            lcd_send_data(0x00);
        }
    }
    for (row = 6; row <= 7; row++)
    {
        lcd_send_command(LCD_CMD_SET_PAGE | row);
        for (collumn = 20; collumn < 148; collumn++)
        {
            lcd_send_command(LCD_CMD_SET_COLUMN_UPPER | (collumn & 0xf0));
            lcd_send_command(LCD_CMD_SET_COLUMN_LOWER | (collumn & 0x0f));
            lcd_send_data(0x00);
        }
    }
}

/**
 * @brief Sends commands to the LCD
 *
 * @param command A command to be sent to the lcd
 */
void lcd_send_command(unsigned char command)
{
    LCD_A0_LOW;     // active low for command bits
    LCD_CS1_LOW;    // LCD chip is selected and is ready to accept commands
    LCD_RD_HIGH;    // read pin must be high because we are not reading
    LCD_WR_LOW;     // the latch is transparent
    LCD_DATA_PORT = command;    // command is written to the port
    LCD_WR_HIGH;    // the data is latched by going high
    LCD_CS1_HIGH;   // LCD chip is deselected
    LCD_A0_HIGH;    // active high after everything is set
}

/**
 * @brief Sends data to be stored in the LCD RAM
 *
 * @param command The data to be sent to the lcd
 */
void lcd_send_data(unsigned char data)
{
    LCD_A0_HIGH;    // active high for data bits
    LCD_CS1_LOW;    // LCD chip is selected and is ready to accept commands
    LCD_RD_HIGH;    // read pin must be high because we are not reading
    LCD_WR_LOW;     // the latch is transparent
    LCD_DATA_PORT = data;   // data is written to the port
    LCD_WR_HIGH;    // the data is latched by going high
    LCD_CS1_HIGH;   // LCD chip is deselected
    LCD_A0_LOW;     // active low for command bits
}

/**
 * @brief Copies the picture to the display buffer
 */
void array_copy(void)
{
    for (uint8_t collumn = 0; collumn <= 31 ; collumn++)
    {
        page_0_picture_buffer[collumn] = atmel[collumn];
        page_1_picture_buffer[collumn] = atmel[collumn + 32];
    }
}

/**
 * @brief Shifts the image towards the right
 */
void shift_right(void)
{
    uint8_t temp = page_0_picture_buffer[127];
    for (int8_t collumn = 126; collumn >= 0; collumn--)
    {
        page_0_picture_buffer[collumn + 1] = page_0_picture_buffer[collumn];
    }
    page_0_picture_buffer[0] = temp;

    temp = page_1_picture_buffer[127];
    for (int8_t collumn = 126; collumn >= 0; collumn--)
    {
        page_1_picture_buffer[collumn + 1] = page_1_picture_buffer[collumn];
    }
    page_1_picture_buffer[0] = temp;

    temp = page_2_picture_buffer[127];
    for (int8_t collumn = 126; collumn >= 0; collumn--)
    {
        page_2_picture_buffer[collumn + 1] = page_2_picture_buffer[collumn];
    }
    page_2_picture_buffer[0] = temp;

    temp = page_3_picture_buffer[127];
    for (int8_t collumn = 126; collumn >= 0; collumn--)
    {
        page_3_picture_buffer[collumn + 1] = page_3_picture_buffer[collumn];
    }
    page_3_picture_buffer[0] = temp;
    lcd_print();
}

/**
 * @brief Shifts the image towards the left
 */
void shift_left(void)
{
    uint8_t temp = page_0_picture_buffer[0];
    for (int8_t collumn = 0; collumn <= 126; collumn++)
    {
        page_0_picture_buffer[collumn] = page_0_picture_buffer[collumn + 1];
    }
    page_0_picture_buffer[127] = temp;

    temp = page_1_picture_buffer[0];
    for (int8_t collumn = 0; collumn <= 126; collumn++)
    {
        page_1_picture_buffer[collumn] = page_1_picture_buffer[collumn + 1];
    }
    page_1_picture_buffer[127] = temp;

    temp = page_2_picture_buffer[0];
    for (int8_t collumn = 0; collumn <= 126; collumn++)
    {
        page_2_picture_buffer[collumn] = page_2_picture_buffer[collumn + 1];
    }
    page_2_picture_buffer[127] = temp;

    temp = page_3_picture_buffer[0];
    for (int8_t collumn = 0; collumn <= 126; collumn++)
    {
        page_3_picture_buffer[collumn] = page_3_picture_buffer[collumn + 1];
    }
    page_3_picture_buffer[127] = temp;
    lcd_print();
}

/**
 * @brief Prints the image on the lcd
 */
void lcd_print(void)
{
    lcd_send_command(LCD_CMD_SET_PAGE | 6);
    for (uint8_t collumn = 20; collumn <= 147 ; collumn++)
    {
        lcd_send_command(LCD_CMD_SET_COLUMN_UPPER | (collumn & 0xf0));
        lcd_send_command(LCD_CMD_SET_COLUMN_LOWER | (collumn & 0x0f));
        lcd_send_data(page_0_picture_buffer[collumn - 20]);
    }

    lcd_send_command(LCD_CMD_SET_PAGE | 7);
    for (uint8_t collumn = 20; collumn <= 147 ; collumn++)
    {
        lcd_send_command(LCD_CMD_SET_COLUMN_UPPER | (collumn & 0xf0));
        lcd_send_command(LCD_CMD_SET_COLUMN_LOWER | (collumn & 0x0f));
        lcd_send_data(page_1_picture_buffer[collumn - 20]);
    }

    lcd_send_command(LCD_CMD_SET_PAGE | 0);
    for (uint8_t collumn = 20; collumn <= 147 ; collumn++)
    {
        lcd_send_command(LCD_CMD_SET_COLUMN_UPPER | (collumn & 0xf0));
        lcd_send_command(LCD_CMD_SET_COLUMN_LOWER | (collumn & 0x0f));
        lcd_send_data(page_2_picture_buffer[collumn - 20]);
    }

    lcd_send_command(LCD_CMD_SET_PAGE | 1);
    for (uint8_t collumn = 20; collumn <= 147 ; collumn++)
    {
        lcd_send_command(LCD_CMD_SET_COLUMN_UPPER | (collumn & 0xf0));
        lcd_send_command(LCD_CMD_SET_COLUMN_LOWER | (collumn & 0x0f));
        lcd_send_data(page_3_picture_buffer[collumn - 20]);
    }
}

/**
 * @brief Shifts the data towards the msb
 */
void shift_towards_msb(void)
{
    /* checks if the first bit is set */
    if (extracted_data & MSB_MASK)
    {
        extracted_data = extracted_data << 1;
        extracted_data |= LSB_MASK;
        flag = 0;
    }
    if (flag)
    {
        extracted_data = extracted_data << 1;
    }
    flag = 1;
}

/**
 * @brief Shifts the data towards the lsb
 */
void shift_towards_lsb(void)
{
    /* checks if the last bit is set */
    if (extracted_data & LSB_MASK)
    {
        extracted_data = extracted_data >> 1;
        extracted_data |= MSB_MASK;
        flag = 0;
    }
    if (flag)
    {
        extracted_data = extracted_data >> 1;
    }
    flag = 1;
}

/**
 * @brief Extracts the data form the ith row of the display buffers
 *
 * @param index Extracts particular column to the display buffer
 */
void buffer_extract(uint8_t index)
{
    extracted_data  = page_3_picture_buffer[index];
    extracted_data  = extracted_data << 8;
    extracted_data |= page_2_picture_buffer[index];
    extracted_data  = extracted_data << 8;
    extracted_data |= page_1_picture_buffer[index];
    extracted_data  = extracted_data << 8;
    extracted_data |= page_0_picture_buffer[index];
}

/**
 * @brief Prints the data back to the respective location on the dispaly buffer
 *
 * @param index Prints particular column to the display buffer
 */
void buffer_print(uint8_t index)
{
    page_0_picture_buffer[index] = (uint8_t)extracted_data;
    page_1_picture_buffer[index] = (uint8_t)(extracted_data >> 8);
    page_2_picture_buffer[index] = (uint8_t)(extracted_data >> 16);
    page_3_picture_buffer[index] = (uint8_t)(extracted_data >> 24);
}

/**
 * @brief Shifts the image down
 */
void shift_down(void)
{
    for (uint8_t row = 0; row <= 127 ; row++)
    {
        buffer_extract(row);
        shift_towards_msb();
        buffer_print(row);
    }
    lcd_print();
}

/**
 * @brief Shifts the image up
 */
void shift_up(void)
{
    for (uint8_t row = 0; row <= 127 ; row++)
    {
        buffer_extract(row);
        shift_towards_lsb();
        buffer_print(row);
    }
    lcd_print();
}

/**
 * @brief Points to the x,y coordinates in the LCD
 *
 * @param column To choose a particular column in the LCD
 * @param row To choose a particular row in the LCD
 */
void lcd_picture_xy(int16_t row, int16_t column)
{
    if (column > 127)
    {
        column = 127;
    }
    else if (column < 0)
    {
        column = 0;
    }

    if (row > 31)
    {
        row = 31;
    }
    else if (row < 0)
    {
        row = 0;
    }
    /* calculate the offset needed */
    temp_x = row - temp_x;
    temp_y = column - temp_y;

    if (temp_x > 0)
    {
        for (uint8_t shift = 0; shift < temp_x ; shift++)
        {
            shift_down();
        }
    }
    else if (temp_x < 0)
    {
        temp_x = -temp_x;
        for (uint8_t shift = 0; shift < temp_x ; shift++)
        {
            shift_up();
        }
    }

    if (temp_y > 0)
    {
        for (uint8_t shift = 0; shift < temp_y ; shift++)
        {
            shift_right();
        }
    }
    else if (temp_y < 0)
    {
        temp_y = -temp_y;
        for (uint8_t shift = 0; shift < temp_y ; shift++)
        {
            shift_left();
        }
    }
    /* restore the old values */
    temp_x = row;
    temp_y = column;
}
