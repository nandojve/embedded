/**
 * @file pal_trx_access.c
 *
 * @brief Transceiver registers & Buffer accessing functions for
 *        AVR32 MCUs with SPI Module
 *
 * This file implements functions for reading and writing transceiver
 * registers and transceiver buffer for AVR32 MCUs with SPI Module.
 *
 * $Id: pal_trx_access.c 29250 2011-11-10 13:41:43Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */
/* === Includes ============================================================ */

#include <stdint.h>
#include "pal_types.h"
#include "return_val.h"
#include "pal.h"
#include "pal_irq.h"

/* === Macros ============================================================== */
/*
 * Write access command of the transceiver
 */
#define WRITE_ACCESS_COMMAND            (0xC0)

/*
 * Read access command to the tranceiver
 */
#define READ_ACCESS_COMMAND             (0x80)

/*
 * Frame write command of transceiver
 */
#define TRX_CMD_FW                      (0x60)

/*
 * Frame read command of transceiver
 */
#define TRX_CMD_FR                      (0x20)

/*
 * SRAM write command of transceiver
 */
#define TRX_CMD_SW                      (0x40)

/*
 * SRAM read command of transceiver
 */
#define TRX_CMD_SR                      (0x00)

/* === Prototypes ========================================================== */

/* === Implementation ====================================================== */

/**
 * @brief SPI write function
 *
 * This function reads the status of SPI register for Transmission.
 *@param b Data to be written to the register
 */
uint8_t spi_write(uint8_t b)
{
    uint32_t status;
    while((RF_SPI.sr & AVR32_SPI_SR_TDRE_MASK) == 0);
    RF_SPI.tdr = b;
    while ((RF_SPI.sr & AVR32_SPI_SR_TXEMPTY_MASK) == 0);
    do
    {
        status = RF_SPI.sr;
        status &= AVR32_SPI_SR_RDRF_MASK;
    } while (status != AVR32_SPI_SR_RDRF_MASK);
    return (uint8_t)RF_SPI.rdr;
}

/**
 * @brief SPI read function
 *
 * This function reads the status of SPI register for Reception.
 *@param value Data to be read via spi
 */
void spi_read(uint8_t value)
{
    uint32_t status;
    RF_SPI.tdr = SPI_DUMMY_VALUE;
    do
    {
        status = RF_SPI.sr;
        status &= AVR32_SPI_SR_RDRF_MASK;
    } while (status != AVR32_SPI_SR_RDRF_MASK);
    value =(uint8_t)RF_SPI.rdr;
    /* To make compiler happy */
    value = value;
}


/**
 * @brief Initializes the transceiver interface
 *
 * This function initializes the transceiver interface.
 */
void trx_interface_init(void)
{
    /*Spi Control Register*/
    uint32_t csr = 0x00;

    /*Software Reset on the SPI*/
    RF_SPI.cr = AVR32_SPI_CR_SWRST_MASK;
    RF_SPI.cr = AVR32_SPI_CR_SWRST_MASK;

    /*Enabling the SPI interface*/
    RF_SPI.cr = AVR32_SPI_CR_SPIEN_MASK;

    /*Setting the SPI mode as master*/
    RF_SPI.mr = RF_SPI_NCPS_MASK | AVR32_SPI_MR_MSTR_MASK | AVR32_SPI_MR_MODFDIS_MASK;

    /*
     * Setting the SPI clock,.i.e.,
     * Data on Leading Edge + 16 Bits Per Transfer + SPICLK/4 = SPI Clock
     * For Example :Spi input clock is 32 Mhz and divider is 4 = 6.4 Mhz
     */
    csr = AVR32_SPI_CSR2_NCPHA_MASK | (RF_SPI_DIV << 8);
    RF_SPI_CS_REG = csr;
}

/**
 * @brief Writes data into a transceiver register
 *
 * This function writes a value into transceiver register.
 *
 * @param addr Address of the trx register
 * @param data Data to be written to trx register
 *
 */
void pal_trx_reg_write(uint8_t addr, uint8_t data)
{
    /*Saving the current interrupt status & disabling the global interrupt */
    ENTER_CRITICAL_REGION();

    /* Prepare the command byte */
    addr |= WRITE_ACCESS_COMMAND;

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the Read command byte */
    spi_write(addr);

    /* Write the byte in the transceiver data register */
    spi_write(data);

    /* Stop the SPI transaction by setting SEL high */
    SS_HIGH();

    /*Restoring the interrupt status which was stored & enabling the global interrupt */
    LEAVE_CRITICAL_REGION();
}


/**
 * @brief Reads current value from a transceiver register
 *
 * This function reads the current value from a transceiver register.
 *
 * @param addr Specifies the address of the trx register
 * from which the data shall be read
 *
 * @return value of the register read
 */
uint8_t pal_trx_reg_read(uint8_t addr)
{
    uint8_t register_value = 0;

    /*Saving the current interrupt status & disabling the global interrupt */
    ENTER_CRITICAL_REGION();

    /* Prepare the command byte */
    addr |= READ_ACCESS_COMMAND;

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the Read command byte */
    spi_write(addr);

    /* Do dummy read for initiating SPI read */
    register_value = spi_write(SPI_DUMMY_VALUE);

    /* Stop the SPI transaction by setting SEL high */
    SS_HIGH();

    /*Restoring the interrupt status which was stored & enabling the global interrupt */
    LEAVE_CRITICAL_REGION();

    return register_value;
}


/**
 * @brief Reads frame buffer of the transceiver
 *
 * This function reads the frame buffer of the transceiver.
 *
 * @param[out] data Pointer to the location to store frame
 * @param[in] length Number of bytes to be read from the frame
 * buffer.
 */
void pal_trx_frame_read(uint8_t *data, uint8_t length)
{
    /*Saving the current interrupt status & disabling the global interrupt */
    ENTER_CRITICAL_REGION();

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the command byte */
    spi_write(TRX_CMD_FR);

    do
    {
        /*
         * Do dummy read for initiating SPI read and
         * Upload the received byte in the user provided location
         */
        *data++ = spi_write(SPI_DUMMY_VALUE);

    } while (--length > 0);

    /* Stop the SPI transaction by setting SEL high */
    SS_HIGH();

    /*Restoring the interrupt status which was stored & enabling the global interrupt */
    LEAVE_CRITICAL_REGION();
}


/**
 * @brief Writes data into frame buffer of the transceiver
 *
 * This function writes data into the frame buffer of the transceiver
 *
 * @param[in] data Pointer to data to be written into frame buffer
 * @param[in] length Number of bytes to be written into frame buffer
 */
void pal_trx_frame_write(uint8_t *data, uint8_t length)
{
    /*Saving the current interrupt status & disabling the global interrupt */
    ENTER_CRITICAL_REGION();

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the command byte */
    spi_write(TRX_CMD_FW);

    do
    {
        /* Write the user provided data in the transceiver data register */
        spi_write(*data++);

    } while (--length > 0);

    /* Stop the SPI transaction by setting SEL high */
    SS_HIGH();

    /*Restoring the interrupt status which was stored & enabling the global interrupt */
    LEAVE_CRITICAL_REGION();
}


/**
 * @brief Subregister read
 *
 * @param   Addr  offset of the register
 * @param   Mask  bit mask of the subregister
 * @param   Pos   bit position of the subregister
 * @return  Data  value of the read bit(s)
 */
uint8_t pal_trx_bit_read(uint8_t addr, uint8_t mask, uint8_t pos)
{
    uint8_t ret;
    ret = pal_trx_reg_read(addr);
    ret &= mask;
    ret >>= pos;
    return ret;
}


/**
 * @brief Subregister write
 *
 * @param[in]   addr  Offset of the register
 * @param[in]   mask  Bit mask of the subregister
 * @param[in]   pos   Bit position of the subregister
 * @param[out]  value  Data, which is muxed into the register
 */
void pal_trx_bit_write(uint8_t reg_addr, uint8_t mask, uint8_t pos, uint8_t new_value)
{
    uint8_t current_reg_value;
    current_reg_value = pal_trx_reg_read(reg_addr);
    current_reg_value &= (uint8_t)~(uint32_t)mask;  // Implicit casting required to avoid IAR Pa091.
    new_value <<= pos;
    new_value &= mask;
    new_value |= current_reg_value;
    pal_trx_reg_write(reg_addr, new_value);
}


#if defined(ENABLE_TRX_SRAM) || defined(DOXYGEN)
/**
 * @brief Writes data into SRAM of the transceiver
 *
 * This function writes data into the SRAM of the transceiver
 *
 * @param addr Start address in the SRAM for the write operation
 * @param data Pointer to the data to be written into SRAM
 * @param length Number of bytes to be written into SRAM
 */
void pal_trx_sram_write(uint8_t addr, uint8_t *data, uint8_t length)
{
    /*Saving the current interrupt status & disabling the global interrupt */
    ENTER_CRITICAL_REGION();

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the command byte */
    spi_write(TRX_CMD_SW);

    /* Send the address from which the write operation should start */
    spi_write(addr);

    do
    {
        /* Upload the user data to transceiver data register */
        spi_write(*data++);

    } while (--length > 0);

    /* Stop the SPI transaction by setting SEL high */
    SS_HIGH();

    /*Restoring the interrupt status which was stored & enabling the global interrupt */
    LEAVE_CRITICAL_REGION();
}
#endif  /* #if defined(ENABLE_TRX_SRAM) || defined(DOXYGEN) */


#if defined(ENABLE_TRX_SRAM) || defined(ENABLE_TRX_SRAM_READ) || defined(DOXYGEN)
/**
 * @brief Reads data from SRAM of the transceiver
 *
 * This function reads from the SRAM of the transceiver
 *
 * @param[in] addr Start address in SRAM for read operation
 * @param[out] data Pointer to the location where data stored
 * @param[in] length Number of bytes to be read from SRAM
 */
void pal_trx_sram_read(uint8_t addr, uint8_t *data, uint8_t length)
{
    PAL_WAIT_500_NS();

    /*Saving the current interrupt status & disabling the global interrupt */
    ENTER_CRITICAL_REGION();

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the command byte */
    spi_write(TRX_CMD_SR);

    /* Send the address from which the read operation should start */
    spi_write(addr);

    do
    {
        /* Upload the received byte in the user provided location */
        *data++ = spi_write(SPI_DUMMY_VALUE);
    } while (--length > 0);

    SS_HIGH();

    /*Restoring the interrupt status which was stored & enabling the global interrupt */
    LEAVE_CRITICAL_REGION();
}
#endif  /* #if defined(ENABLE_TRX_SRAM) || defined(ENABLE_TRX_SRAM_READ) || defined(DOXYGEN) */


#if defined(ENABLE_TRX_SRAM) || defined(DOXYGEN)
/**
 * @brief Writes and reads data into/from SRAM of the transceiver
 *
 * This function writes data into the SRAM of the transceiver and
 * simultaneously reads the bytes.
 *
 * @param addr Start address in the SRAM for the write operation
 * @param idata Pointer to the data written/read into/from SRAM
 * @param length Number of bytes written/read into/from SRAM
 */
void pal_trx_aes_wrrd(uint8_t addr, uint8_t *idata, uint8_t length)
{
    uint8_t *odata;

    PAL_WAIT_500_NS();

    ENTER_TRX_REGION();

#ifdef NON_BLOCKING_SPI
    while (spi_state != SPI_IDLE)
    {
        /* wait until SPI gets available */
    }
#endif

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the command byte */
    spi_write(TRX_CMD_SW);

    /* write SRAM start address */
    spi_write(addr);

    /* now transfer data */
    odata = idata;

    /* write data byte 0 - the obtained value in SPDR is meaningless */
    spi_write(*idata++);

    /* Reading Spi Data for the length specified */
    while (length > 0)
    {
        spi_write(*idata++);
        *odata++ = SPI_READ();
        length--;
    }

    /* To get the last data byte, write some dummy byte */
    *odata = spi_write(SPI_DUMMY_VALUE);

    /* Stop the SPI transaction by setting SEL high */
    SS_HIGH();
    LEAVE_TRX_REGION();
}
#endif  /* #if defined(ENABLE_TRX_SRAM) || defined(DOXYGEN) */

/* EOF */
