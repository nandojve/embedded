/**
 * @file pal_timer.h
 *
 * @brief PAL timer internal functions prototypes for AVR32 MCUs
 *
 * This header has the timer specific stuctures, macros and
 * internal functions for AVR32 MCUs.
 *
 * $Id: pal_timer.h 25330 2011-02-03 06:48:22Z yogesh.bellan $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */


/* Prevent double inclusion */
#ifndef PAL_TIMER_H
#define PAL_TIMER_H

/* === Includes ============================================================= */


/* === Types ================================================================ */

/*
 * This defines the structure of the time type.
 */
typedef struct timer_info_tag
{
    /* Timeout in microseconds */
    uint32_t abs_exp_timer;

    /* Callback function to be executed on expiry of the timer */
    void *timer_cb;

    /* Parameter to be passed to the callback function of the expired timer */
    void *param_cb;

    /* Next timer which was started or has expired */
    uint8_t next_timer_in_queue;
} timer_info_t;

/*
 * Callback for timer functions
 */
typedef void (*timer_expiry_cb_t)(void *);

/* === Externals ============================================================ */

extern timer_info_t timer_array[];

extern volatile uint16_t sys_time;

/* === Macros ================================================================ */

/*
 * The index to indicate end of timer in the array
 */
#define NO_TIMER                        (0xFF)

/*
 * The timeout indicating that timer is not running
 */
#define TIMER_NOT_RUNNING               (0)

/*
 * Shift mask to obtain the 16-bit system time out of a 32-bit timeout
 */
#define SYS_TIME_SHIFT_MASK             (16)

/*
 * Mask to obtain the 16-bit H/W time out of a 32-bit timeout
 */
#define HW_TIME_MASK                    (0xFFFF)

/*
 * Timer related definitions
 */
#define DISABLE_ALL_TIMER_INTERRUPTS    (0xFF)
#define ONE_MILLISECOND_DELAY           (0x03E8)
#define ONE_MILLISECOND_OVERFLOW        (0x07D0)
#define SET                             (1)
#define NOT_SET                         (0)
#define TEST_OUTPUT_VALUE               (0x00005000)

/**
 * Mask to obtain the most significant bit of a 32 bit variable
 */
#define MSB_MASK                        (0x80000000)

/**
 * Mask to clear the least significant bits of a 32 bit variable
 */
#define LSB_MASK                        (0x7FFFFFFF)


/*
 * Defining values
 */
#define INTIALIZE_TO_ZERO               (0x00)

/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* To initialize the timer module */
void timer_init(void);

/* To initialize the timer module of the specific board combination */
void timer_init_non_generic(void);


/* To Check for expired timers and rearranges the running timer
 * queue head and expired timer queue head and tail
 * if there are any expired timers
 */
void internal_timer_handler(void);


/* To stop the running timer and remove it from the timer que */
bool stop_timer(uint8_t timer_id);


/* To put the expired timer into the expired timer queue and
 * to service expired timers and call the respective callback
 */
void timer_service(void);


/**
 * @brief Checks if timer can be started with specified timeout
 *
 * This function checks whether a timer can be started with the specified
 * timeout.
 *
 * @param timeout Time in microseconds
 *
 * @return true if the timeout is in a range to start timer
 */
static inline bool check_timeout_range(uint32_t timeout)
{
    bool ret = false;

    if ((timeout < MAX_TIMEOUT) && (timeout > MIN_TIMEOUT))
    {
        ret = true;
    }
    return ret;
}

/**
 * @brief Gets actual system time
 *
 * This function is called to get the system time
 *
 * @return Time in microseconds
 */
static inline uint32_t gettime(void)
{
    uint16_t current_sys_time;
    uint32_t current_time;
    do
    {
        current_sys_time = TIMER_HIGH_REGISTER;
        current_time = current_sys_time;
        current_time = current_time << 16;
        current_time = current_time | (uint16_t)TIMER_CHANNEL_COUNT;
        /*
         * This calculation is valid only if the timer has not rolled over.
         * The sys_time variable may have changed in the timer overflow ISR.
         */
    } while (current_sys_time != TIMER_HIGH_REGISTER);
    return current_time;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PAL_TIMER_H */
/* EOF */
