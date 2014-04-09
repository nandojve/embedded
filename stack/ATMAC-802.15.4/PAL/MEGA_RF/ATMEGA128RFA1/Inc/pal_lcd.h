/**
 * @file pal_lcd.h
 *
 * @brief This file contains TAL API function declarations
 *
 * $Id: pal_lcd.h 31419 2012-03-23 07:45:21Z prashanth.udumula $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef LCD_H
#define LCD_H

/* === INCLUDES ============================================================ */


/* === MACROS ============================================================== */

/**
 * The control pins of LCD
 */
#define LCD_CS1             (PE4)    /* chip select  */
#define LCD_RES             (PE2)    /* chip reset   */
#define LCD_A0              (PE3)    /* data/control */
#define LCD_RW              (PE6)    /* write        */
#define LCD_RD              (PE7)    /* read         */
#define LCD_CONTROL_CONFIG  (0xDC)   /* lcd control pins are config as output */
#define LCD_DATA_DIR_CONFIG (0xFF)   /* lcd data pins are config as output  */

/**
 * The mask for checking and setting bits
 */
#define MSB_MASK (0x80000000) /* for checking the first bit of a 32 bit value */
#define LSB_MASK (0x00000001) /* for checking the last bit of a 32 bit value */

/**
 * The data port of LCD
 */
#define LCD_DATA_PORT     PORTB

/**
 * The direction of data port
 */
#define LCD_DATA_DIR      DDRB

/**
 * The control port of LCD
 */
#define LCD_CONTROL_PORT  PORTE

/**
 * The direction of control port
 */
#define LCD_CONTROL_DIR   DDRE

/**
 * @brief Macros for making control pins of LCD high
 */
#define LCD_CS1_HIGH  (LCD_CONTROL_PORT |= _BV(LCD_CS1))
#define LCD_RES_HIGH  (LCD_CONTROL_PORT |= _BV(LCD_RES))
#define LCD_A0_HIGH   (LCD_CONTROL_PORT |= _BV(LCD_A0))
#define LCD_WR_HIGH   (LCD_CONTROL_PORT |= _BV(LCD_RW))
#define LCD_RD_HIGH   (LCD_CONTROL_PORT |= _BV(LCD_RD))

/**
 * @brief Macros for making control pins of LCD low
 */
#define LCD_CS1_LOW   (LCD_CONTROL_PORT &= ~_BV(LCD_CS1))
#define LCD_RES_LOW   (LCD_CONTROL_PORT &= ~_BV(LCD_RES))
#define LCD_A0_LOW    (LCD_CONTROL_PORT &= ~_BV(LCD_A0))
#define LCD_WR_LOW    (LCD_CONTROL_PORT &= ~_BV(LCD_RW))
#define LCD_RD_LOW    (LCD_CONTROL_PORT &= ~_BV(LCD_RD))


/* LCD command bytes */

/* Command for LCD display off */
#define LCD_CMD_DISPLAY_OFF               (0xAE)
/* Command for LCD display on */
#define LCD_CMD_DISPLAY_ON                (0xAF)
/* Command to start form first line */
#define LCD_CMD_SET_DISP_START_LINE       (0x40)
/* Command setting LCD page */
#define LCD_CMD_SET_PAGE                  (0xB0)
/* Command setting the upper address of the column */
#define LCD_CMD_SET_COLUMN_UPPER          (0x10)
/* Command setting the lower address of the column */
#define LCD_CMD_SET_COLUMN_LOWER          (0x00)
/* Command to set RAM SREG correspondance to normal */
#define LCD_CMD_SET_ADC_NORMAL            (0xA0)
/* Command to set RAM SREG correspondance to reverse */
#define LCD_CMD_SET_ADC_REVERSE           (0xA1)
/* Command to set the lcd bias to 1/9 */
#define LCD_CMD_SET_BIAS_9                (0xA2)
/* Command to set the lcd bias to 1/7 */
#define LCD_CMD_SET_BIAS_7                (0xA3)
/* Command to display in normal */
#define LCD_CMD_SET_COM_NORMAL            (0xC0)
/* Command to display in reverse */
#define LCD_CMD_SET_COM_REVERSE           (0xC8)
/* Command to set internal regulator resistor ratio */
#define LCD_CMD_SET_RESISTOR_RATIO        (0x20)
/* Command to set the volume of lcd */
#define LCD_CMD_SET_VOLUME_FIRST          (0x81)
/* Command to set the volume */
#define LCD_CMD_SET_VOLUME_SECOND         (0x00)
/* Command to set the contrast */
#define LCD_CMD_CONTRAST                  (0x23)
/* Command to set the voltage settings */
#define LCD_SET_VOLTAGE_SETTINGS          (0x2f)
/* Command for normal display settings */
#define LCD_CMD_DISPLAY_ALL_POINTS_NORMAL (0xa4)
/* Command for making all points in the lcd on */
#define LCD_CMD_DISPLAY_ALL_POINTS_ON     (0xa5)
/* Command for normal display settings */
#define LCD_CMD_DISPLAY_NORMAL            (0xa6)
/* Command for complimentary lcd display */
#define LCD_CMD_DISPLAY_REVERSE           (0xa7)
/* Command to set lcd brightness */
#define LCD_SET_BRIGHTNESS                (0x10)
/* The width of each character */
#define CHARACTER_WIDTH                   (8)

/* === GLOBALS ============================================================= */


/* === PROTOTYPES ========================================================== */

/**
 * @brief Initializing the ports connected to LCD
 */
void lcd_port_init(void);

/**
 * @brief Initializing the LCD
 */
void lcd_init(void);

/**
 * @brief A function to clear the LCD
 */
void lcd_clr(void);

/**
 * @brief A function to pass commands into the ram of the LCD
 *
 * @param command The LCD command to be sent
 */
void lcd_send_command(unsigned char command);

/**
 * @brief A function to write data into the ram of the LCD
 *
 * @param data The data to be sent
 */
void lcd_send_data(unsigned char data);

/**
 * @brief Points to the x,y coordinates in the LCD
 *
 * @param column To choose a particular column in the LCD
 * @param row To choose a particular row in the LCD
 */
void lcd_picture_xy(int16_t row, int16_t column);

/**
 * @brief Shifts the image up
 */
void shift_up(void);

/**
 * @brief Shifts the image down
 */
void shift_down(void);

/**
 * @brief Prints the data back to the respective location on the dispaly buffer
 */
void buffer_print(uint8_t index);

/**
 * @brief Extracts the data form the ith row of the display buffers
 */
void buffer_extract(uint8_t index);

/**
 * @brief Shifts the data towards the lsb
 */
void shift_towards_lsb(void);

/**
 * @brief Shifts the data towards the msb
 */
void shift_towards_msb(void);

/**
 * @brief Prints the image on the lcd
 */
void lcd_print(void);

/**
 * @brief Shifts the image towards the left
 */
void shift_left(void);

/**
 * @brief Shifts the image towards the right
 */
void shift_right(void);

/**
 * @brief Copies the picture to the display buffer
 */
void array_copy(void);

/**
 * @brief Displays the text data in the array on to the LCD
 */
void lcd_text_display(uint8_t *array, uint16_t length, uint8_t row);

#endif
