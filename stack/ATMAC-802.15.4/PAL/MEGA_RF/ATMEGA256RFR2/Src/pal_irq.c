/**
 * @file pal_irq.c
 *
 * @brief PAL IRQ functionality
 *
 * This file contains functions to initialize, enable, disable and install
 * handler for the transceiver interrupts. It also contains functions to enable,
 * disable and get the status of global interrupt
 *
 * $Id: pal_irq.c 29425 2011-11-18 13:06:06Z sschneid $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */

#include <stdint.h>
#include "pal.h"

#if (PAL_TYPE == ATMEGA256RFR2)

/* === Types =============================================================== */


/* === Globals ============================================================= */

/*
 * Function pointers to store the callback function of
 * the transceiver interrupt
 */
/** Function pointer to store callback for transceiver TX_END interrupt. */
static irq_handler_t irq_hdl_trx_tx_end;
/** Function pointer to store callback for transceiver RX_END interrupt. */
static irq_handler_t irq_hdl_trx_rx_end;
/** Function pointer to store callback for transceiver CCA_ED interrupt. */
static irq_handler_t irq_hdl_trx_cca_ed;
/** Function pointer to store callback for transceiver AWAKE interrupt. */
static irq_handler_t irq_hdl_trx_awake;

#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) || (defined DOXYGEN)
/** Function pointer to store callback for transceiver timestamp (RX_START) interrupt. */
static irq_handler_t irq_hdl_trx_tstamp;
#endif

#if (defined ENABLE_ALL_TRX_IRQS) || (defined DOXYGEN)
/** Function pointer to store callback for transceiver AMI interrupt. */
static irq_handler_t irq_hdl_trx_ami;
/** Function pointer to store callback for transceiver BATMON interrupt. */
static irq_handler_t irq_hdl_trx_batmon;
/** Function pointer to store callback for transceiver PLL_LOCK interrupt. */
static irq_handler_t irq_hdl_trx_pll_lock;
/** Function pointer to store callback for transceiver PLL_UNLOCK interrupt. */
static irq_handler_t irq_hdl_trx_pll_unlock;
/** Function pointer to store callback for transceiver AES_READY interrupt. */
static irq_handler_t irq_hdl_trx_aes_ready;
/** Function pointer to store callback for transceiver TX_START interrupt. */
static irq_handler_t irq_hdl_trx_tx_start;
/** Function pointer to store callback for transceiver AMI0 interrupt. */
static irq_handler_t irq_hdl_trx_ami0;
/** Function pointer to store callback for transceiver AMI1 interrupt. */
static irq_handler_t irq_hdl_trx_ami1;
/** Function pointer to store callback for transceiver AMI2 interrupt. */
static irq_handler_t irq_hdl_trx_ami2;
/** Function pointer to store callback for transceiver AMI3 interrupt. */
static irq_handler_t irq_hdl_trx_ami3;
#endif  /* ENABLE_ALL_TRX_IRQS */

/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

/**
 * @brief Initializes the transceiver TX END interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver TX END interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver TX END interrupt
 */
void pal_trx_irq_init_tx_end(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_tx_end = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver RX END interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver RX END interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver RX END interrupt
 */
void pal_trx_irq_init_rx_end(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_rx_end = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver CCA ED END interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver CCA ED END interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver CCA ED END interrupt
 */
void pal_trx_irq_init_cca_ed(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_cca_ed = (irq_handler_t)trx_irq_cb;
}


#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) || defined(DOXYGEN)
/**
 * @brief Initializes the transceiver timestamp interrupt (RX START interrupt)
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver timestamp interrupt (RX START interrupt)
 *
 * @param trx_irq_cb Callback function for the transceiver
 * timestamp interrupt (RX START interrupt)
 */
void pal_trx_irq_init_tstamp(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_tstamp = (irq_handler_t)trx_irq_cb;
}
#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */


/**
 * @brief Initializes the transceiver AWAKE interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver AWAKE interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver AWAKE interrupt
 */
void pal_trx_irq_init_awake(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_awake = (irq_handler_t)trx_irq_cb;
}


#if defined(ENABLE_ALL_TRX_IRQS) || defined(DOXYGEN)
/**
 * @brief Initializes the transceiver AMI interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver AMI interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver AMI interrupt
 */
void pal_trx_irq_init_ami(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_ami = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver BATMON interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver BATMON interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver BATMON interrupt
 */
void pal_trx_irq_init_batmon(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_batmon = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver PLL_LOCK interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver PLL_LOCK interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver PLL_LOCK interrupt
 */
void pal_trx_irq_init_pll_lock(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_pll_lock = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver PLL_UNLOCK interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver PLL_UNLOCK interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver PLL_UNLOCK interrupt
 */
void pal_trx_irq_init_pll_unlock(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_pll_unlock = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver AES_READY interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver AES_READY interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver AES_READY interrupt
 */
void pal_trx_irq_init_aes_ready(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_aes_ready = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver TX_START interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver TX_START interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver TX_START interrupt
 */
void pal_trx_irq_init_tx_start(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_tx_start = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver MAF AMI0 interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver MAF AMI0 interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver MAF AMI0 interrupt
 */
void pal_trx_irq_init_maf_0_ami(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_ami0 = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver MAF AMI1 interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver MAF AMI1 interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver MAF AMI1 interrupt
 */
void pal_trx_irq_init_maf_1_ami(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_ami1 = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver MAF AMI2 interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver MAF AMI2 interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver MAF AMI2 interrupt
 */
void pal_trx_irq_init_maf_2_ami(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_ami2 = (irq_handler_t)trx_irq_cb;
}


/**
 * @brief Initializes the transceiver MAF AMI3 interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver MAF AMI3 interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver MAF AMI3 interrupt
 */
void pal_trx_irq_init_maf_3_ami(FUNC_PTR trx_irq_cb)
{
    irq_hdl_trx_ami3 = (irq_handler_t)trx_irq_cb;
}

#endif  /* ENABLE_ALL_TRX_IRQS */



#ifdef PAL_XTD_IRQ_API
/**
 * @brief Returns the current callback function for the transceiver RX END interrupt
 *
 * @return Current callback function for the transceiver RX END interrupt
 */
FUNC_PTR pal_trx_irq_get_hdlr_rx_end(void)
{
    return irq_hdl_trx_rx_end;
}


/**
 * @brief Returns the current callback function for the transceiver TX END interrupt
 *
 * @return Current callback function for the transceiver TX END interrupt
 */
FUNC_PTR pal_trx_irq_get_hdlr_tx_end(void)
{
    return irq_hdl_trx_tx_end;
}
#endif  /* PAL_XTD_IRQ_API */


/**
 * @brief ISR for transceiver's transmit end interrupt
 */
ISR(TRX24_TX_END_vect)
{
    irq_hdl_trx_tx_end();
}

/**
 * @brief ISR for transceiver's receive end interrupt
 */
ISR(TRX24_RX_END_vect)
{
    irq_hdl_trx_rx_end();
}

/**
 * @brief ISR for transceiver's CCA/ED measurement done interrupt
 */
ISR(TRX24_CCA_ED_DONE_vect)
{
    irq_hdl_trx_cca_ed();
}

/**
 * @brief ISR for transceiver's Awake interrupt
 */
ISR(TRX24_AWAKE_vect)
{
    irq_hdl_trx_awake();
}

/**
 * @brief ISR for transceiver's rx start interrupt
 *
 * By the time the SFD is detected, the hardware timestamps the
 * current frame in the SCTSR register.
 */
ISR(TRX24_RX_START_vect)
{
#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)
    irq_hdl_trx_tstamp();
#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */
}

/**
 * @brief ISR for transceiver's PLL lock interrupt
 */
ISR(TRX24_PLL_LOCK_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_pll_lock();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's PLL unlock interrupt
 */
ISR(TRX24_PLL_UNLOCK_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_pll_unlock();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's address match interrupt
 */
ISR(TRX24_XAH_AMI_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_ami();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's battery low interrupt
 */
ISR(BAT_LOW_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_batmon();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's AES interrupt
 */
ISR(AES_READY_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_aes_ready();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's TX_START interrupt
 */
ISR(TRX24_TX_START_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_tx_start();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's AMI0 interrupt
 */
ISR(TRX24_AMI0_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_ami0();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's AMI1 interrupt
 */
ISR(TRX24_AMI1_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_ami1();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's AMI2 interrupt
 */
ISR(TRX24_AMI2_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_ami2();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}

/**
 * @brief ISR for transceiver's AMI3 interrupt
 */
ISR(TRX24_AMI3_vect)
{
#ifdef ENABLE_ALL_TRX_IRQS
    irq_hdl_trx_ami3();
#endif  /*  ENABLE_ALL_TRX_IRQS*/
}


#endif /* (PAL_TYPE == ATMEGA256RFR2) */

/* EOF */
