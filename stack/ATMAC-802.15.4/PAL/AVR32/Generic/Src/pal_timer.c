/**
 * @file pal_timer.c
 *
 * @brief PAL timer related APIs for AVR32 MCUs
 *
 * This file implements PAL timer related APIs for AVR32 MCUs
 *
 * $Id: pal_timer.c 28616 2011-09-20 08:37:55Z uwalter $
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

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "pal_types.h"
#include "return_val.h"
#include "pal.h"
#include "pal_timer.h"
#include "app_config.h"
#include "pal_uart.h"
#include "pal_irq.h"
#include "pal_internal.h"

/* === Macros ============================================================== */

/* === Globals ============================================================== */

/*
 * Check the number of required timers or change the number of timers that are
 * provided by the PAL. This is a kind of error handling and used reducing the
 * number of used timer and therefore used to reduce the RAM usage.
 */
#if (TOTAL_NUMBER_OF_TIMERS > MAX_NO_OF_TIMERS)
#error "Number of used timer are greater than the numbers of timer provided by PAL."
#endif

#if (TOTAL_NUMBER_OF_TIMERS > 0)

/** This is the timer array */
timer_info_t timer_array[TOTAL_NUMBER_OF_TIMERS];

/** This is the counter of all running timers */
static uint8_t running_timers;

/** This is a flag, that should indicate an expired timer */
static volatile bool timer_trigger;

/** This index holds the  pointer to head of running timer queue */
static uint8_t running_timer_queue_head;

/** This index holds the  pointer to head of expired timer queue */
static uint8_t expired_timer_queue_head;

/** This index holds the  pointer to tail of expired timer queue */
static uint8_t expired_timer_queue_tail;

#endif  /* #if (TOTAL_NUMBER_OF_TIMERS > 0) */


/* This variable holds the id of high priority timer */
#ifdef ENABLE_HIGH_PRIO_TMR
static volatile uint8_t high_priority_timer_id;
#endif

/* This flag gives indication of whether compare register is loaded or not */
static volatile bool reg_rc_loaded;

/* === Prototypes =========================================================== */
#if (TOTAL_NUMBER_OF_TIMERS > 0)
static void prog_ocr(void);
static void start_absolute_timer(uint8_t timer_id, uint32_t point_in_time,
                          void *handler_cb, void *parameter);
#endif  /* #if (TOTAL_NUMBER_OF_TIMERS > 0) */

/* === Implementation ======================================================= */
#if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN))
/**
 * @brief Compares two time values
 *
 * This function compares two time values t1 and t2 and returns true if t1 is
 * less than t2. The rollover case is also taken care of.
 *
 * @param t1 Time
 * @param t2 Time
 *
 * @return true If t1 is less than t2 when MSBs are same, false otherwise. If
 * MSBs are not equal it returns true if t1 is greater than t2, flase otherwise
 */
static inline bool pal_compare_time(uint32_t t1, uint32_t t2)
{
    bool comparison_status = false;
    if ((t1 & MSB_MASK) == (t2 & MSB_MASK))
    {
        if (t1 < t2)
        {
            comparison_status = true;
        }
    }
    else
    {
        /* This is a timer roll case in which t2 is ahead of t1. */
        if ((t1 & LSB_MASK) > (t2 & LSB_MASK))
        {
            comparison_status = true;
        }
    }
    return comparison_status;
}
#endif  /* #if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN)) */


#if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN))
/**
 * @brief Timer handling services
 *
 * This Function performs timer handling services.
 * It calls functions which are responsible
 * 1) to put the expired timer into the expired timer queue, and
 * 2) to service expired timers and call the respective callback.
 */
void timer_service(void)
{
    /*Check if any timer has expired. */
    ENTER_CRITICAL_REGION();
    internal_timer_handler();
    LEAVE_CRITICAL_REGION();
    /*
     * Process expired timers.
     * Call the callback functions of the expired timers in the order of their
     * expiry.
     */
    timer_expiry_cb_t callback;
    void *callback_param;
    uint8_t next_expired_timer;

    /* Expired timer if any will be processed here */
    while (NO_TIMER != expired_timer_queue_head)
    {
        /*Saving the current interrupt status & disabling the global interrupt */
        ENTER_CRITICAL_REGION();

        next_expired_timer =
            timer_array[expired_timer_queue_head].next_timer_in_queue;

        /*Callback is stored */
        callback = (timer_expiry_cb_t)timer_array[expired_timer_queue_head].timer_cb;

        /*Callback parameter is stored */
        callback_param = timer_array[expired_timer_queue_head].param_cb;

        /*
         * The expired timer's structure elements are updated and the timer
         * is taken out of expired timer queue
         */
        timer_array[expired_timer_queue_head].next_timer_in_queue = NO_TIMER;
        timer_array[expired_timer_queue_head].timer_cb = NULL;
        timer_array[expired_timer_queue_head].param_cb = NULL;

        /*
         * Expired timer queue head is updated to point to next timer in the
         * expired timer queue
         */
        expired_timer_queue_head = next_expired_timer;

        if (NO_TIMER == expired_timer_queue_head)
        {
            expired_timer_queue_tail = NO_TIMER;
        }
        /*Restoring the interrupt status which was stored & enabling the global interrupt */
        LEAVE_CRITICAL_REGION();

        if (NULL != callback)
        {
            /*Callback function is called */
            callback(callback_param);
        }
    }
}
#endif  /* #if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN)) */

#if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN))
/**
 * @brief Start regular timer
 *
 * This function starts a regular timer and installs the corresponding
 * callback function to handle the timeout event.
 *
 * @param timer_id Timer identifier
 * @param timer_count Timeout in microseconds
 * @param timeout_type @ref TIMEOUT_RELATIVE / @ref TIMEOUT_ABSOLUTE
 * @param timer_cb Callback handler invoked upon timer expiry
 * @param param_cb Argument for the callback handler
 *
 * @return
 * - @ref INVALID_ID  if the timer identifier is undefined,
 * - @ref INVALID_PARAMETER if the callback function for this timer is NULL or
 *   timeout_type is invalid,
 * - @ref ALREADY_RUNNING if the timer is already running,
 * - @ref SUCCESS if timer is started, or
 * - @ref INVALID_TIMEOUT if timeout is not within timeout range.
 */
retval_t pal_timer_start(uint8_t timer_id,
                         uint32_t timer_count,
                         timeout_type_t timeout_type,
                         void *timer_cb,
                         void *param_cb)
{
    uint32_t now;
    uint32_t point_in_time;
    uint32_t timeout;

    if (TOTAL_NUMBER_OF_TIMERS <= timer_id)
    {
        return PAL_TMR_INVALID_ID;
    }

    if (NULL == timer_cb)
    {
        return MAC_INVALID_PARAMETER;
    }

    if (NULL != timer_array[timer_id].timer_cb)
    {
        /*
         * Timer is already running if the callback function of the
         * corresponding timer index in the timer array is not NULL.
         */
        return PAL_TMR_ALREADY_RUNNING;
    }

    /*To get the current system time*/
    now = gettime();

    switch(timeout_type)
    {
        /* Timeout Relative */
        case TIMEOUT_RELATIVE:
        {
            if ((timer_count > MAX_TIMEOUT) || (timer_count < MIN_TIMEOUT))
            {
                return PAL_TMR_INVALID_TIMEOUT;
            }

            point_in_time = ADD_TIME(timer_count, now);
        }
        break;
        /* Timeout Absolute */
        case TIMEOUT_ABSOLUTE:
        {
            timeout = SUB_TIME(timer_count, now);
            if ((timeout > MAX_TIMEOUT) || (timeout < MIN_TIMEOUT))
            {
                return PAL_TMR_INVALID_TIMEOUT;
            }
            point_in_time = timer_count;
        }
        break;

        default:
            return MAC_INVALID_PARAMETER;
    }
    /*Starting the absolute timer for the output compare to get fired*/
    start_absolute_timer(timer_id, point_in_time, timer_cb, param_cb);
    return MAC_SUCCESS;
}
#endif  /* #if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN)) */


#if ((defined ENABLE_HIGH_PRIO_TMR) || defined(DOXYGEN))
/**
 * @brief Starts high priority timer
 *
 * This function starts a high priority timer for the specified timeout.
 *
 * @param timer_id Timer identifier
 * @param timer_count Timeout in microseconds
 * @param timer_cb Callback handler invoked upon timer expiry
 * @param param_cb Argument for the callback handler
 *
 * @return
 * - @ref PAL_TMR_INVALID_ID if the identifier is undefined,
 * - @ref MAC_INVALID_PARAMETER if the callback function for this timer is NULL,
 * - @ref PAL_TMR_ALREADY_RUNNING if the timer is already running, or
 * - @ref MAC_SUCCESS if timer is started successfully.
 */
retval_t pal_start_high_priority_timer(uint8_t timer_id,
                                       uint16_t timer_count,
                                       FUNC_PTR timer_cb,
                                       void *param_cb)
{
    uint32_t timer_ocr = INTIALIZE_TO_ZERO;

    if (TOTAL_NUMBER_OF_TIMERS <= timer_id)
    {
        return PAL_TMR_INVALID_ID;
    }

    if (NULL == timer_cb)
    {
        return MAC_INVALID_PARAMETER;
    }

    if (NULL != timer_array[timer_id].timer_cb)
    {
        /*
         * Irrespective of the type, the timer is already running if the
         * callback function of the corresponding timer index in the timer
         * array is not NULL.
         */
        return PAL_TMR_ALREADY_RUNNING;
    }

    /*
     * A high priority timer can be started, as currently
     * there is no high priority timer running.
     */
    {
        /*Saving the current interrupt status & disabling the global interrupt */
        ENTER_CRITICAL_REGION();

        high_priority_timer_id = timer_id;
        /*
         * The corresponding running timer queue's timer index is updated
         * with the new values.
         */
        timer_array[timer_id].timer_cb = timer_cb;
        timer_array[timer_id].param_cb = param_cb;
        timer_array[timer_id].next_timer_in_queue = NO_TIMER;
        timer_ocr = timer_count;
        timer_array[timer_id].abs_exp_timer = timer_ocr;

        /* Program output compare match */
        HIGH_PRIORITY_TIMER_COMP = timer_ocr;
        /*Clear pending output compare matches */
        HIGH_PRIORITY_TIMER_DISABLE_INT = DISABLE_ALL_TIMER_INTERRUPTS;
        /* Enable output compare match interrupt */
        HIGH_PRIORITY_TIMER_ENABLE_INT  |=  AVR32_TC_IER0_CPAS_MASK;

        /*Restoring the interrupt status which was stored & enabling the global interrupt */
        LEAVE_CRITICAL_REGION();
    }

    return MAC_SUCCESS;
}
#endif /* #if ((defined ENABLE_HIGH_PRIO_TMR) || defined(DOXYGEN)) */


#if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN))
/**
 * @brief Stops a running timer
 *
 * This function stops a running timer with the specified timer_id.
 *
 * @param timer_id Timer identifier
 *
 * @return
 * - @ref MAC_SUCCESS if the timer was stopped successfully,
 * - @ref PAL_TMR_NOT_RUNNING if the specified timer is not running,
 * - @ref PAL_TMR_INVALID_ID if the specified timer id is undefined.
 */
retval_t pal_timer_stop(uint8_t timer_id)
{
    bool timer_stop_request_status = false;
    uint8_t curr_index;
    uint8_t prev_index;

    if (TOTAL_NUMBER_OF_TIMERS <= timer_id)
    {
        return (PAL_TMR_INVALID_ID);
    }

    /*Saving the current interrupt status & disabling the global interrupt */
    ENTER_CRITICAL_REGION();

    /*Check if any timer has expired. */
    internal_timer_handler();

    /* The requested timer is first searched in the running timer queue */
    if (running_timers > 0)
    {
        uint8_t timer_count = running_timers;
        prev_index = curr_index = running_timer_queue_head;
        while (timer_count > 0)
        {
            if (timer_id == curr_index)
            {
                timer_stop_request_status = true;

                if (timer_id == running_timer_queue_head)
                {
                    running_timer_queue_head =
                        timer_array[timer_id].next_timer_in_queue;
                    /*
                     * The value in OCR corresponds to the timeout pointed
                     * by the 'running_timer_queue_head'. As the head has
                     * changed here, OCR needs to be loaded by the new
                     * timeout value, if any.
                     */
                     prog_ocr();
                }
                else
                {
                    timer_array[prev_index].next_timer_in_queue =
                        timer_array[timer_id].next_timer_in_queue;
                }
                /*
                 * The next timer element of the stopped timer is updated
                 * to its default value.
                 */
                timer_array[timer_id].next_timer_in_queue = NO_TIMER;
                break;
            }
            else
            {
                prev_index = curr_index;
                curr_index = timer_array[curr_index].next_timer_in_queue;
            }
            timer_count--;
        }
        if (timer_stop_request_status)
        {
            running_timers--;
        }
    }

    /*
     * The requested timer is not present in the running timer queue.
     * It will be now searched in the expired timer queue
     */
    if (!timer_stop_request_status)
    {
        prev_index = curr_index = expired_timer_queue_head;
        while (NO_TIMER != curr_index)
        {
            if (timer_id == curr_index)
            {
                if (timer_id == expired_timer_queue_head)
                {
                    /*
                     * The requested timer is the head of the expired timer
                     * queue
                     */
                    if (expired_timer_queue_head == expired_timer_queue_tail)
                    {
                        /* Only one timer in expired timer queue */
                        expired_timer_queue_head = expired_timer_queue_tail =
                            NO_TIMER;
                    }
                    else
                    {
                        /*
                         * The head of the expired timer queue is moved to next
                         * timer in the expired timer queue.
                         */
                        expired_timer_queue_head =
                            timer_array[expired_timer_queue_head].next_timer_in_queue;
                    }
                }
                else
                {
                    /*
                     * The requested timer is present in the middle or at the
                     * end of the expired timer queue.
                     */
                    timer_array[prev_index].next_timer_in_queue =
                        timer_array[timer_id].next_timer_in_queue;

                    /*
                     * If the stopped timer is the one which is at the tail of
                     * the expired timer queue, then the tail is updated.
                     */
                    if (timer_id == expired_timer_queue_tail)
                    {
                        expired_timer_queue_tail = prev_index;
                    }
                }
                timer_stop_request_status = true;
                break;
            }
            else
            {
               prev_index = curr_index;
               curr_index = timer_array[curr_index].next_timer_in_queue;
            }
        }
    }

    if (timer_stop_request_status)
    {
        /*
         * The requested timer is stopped, hence the structure elements of the
         * timer are updated.
         */
        timer_array[timer_id].timer_cb = NULL;
    }

    /*Restoring the interrupt status which was stored & enabling the global interrupt */
    LEAVE_CRITICAL_REGION();

    if (timer_stop_request_status)
    {
        return (MAC_SUCCESS);
    }

    return (PAL_TMR_NOT_RUNNING);
}
#endif  /* #if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN)) */


#if ((defined ENABLE_HIGH_PRIO_TMR) || defined(DOXYGEN))
/**
 * @brief Stops a high priority timer
 *
 * This function stops a high priority timer.
 *
 * @param timer_id Timer identifier
 *
 * @return
 * - @ref PAL_TMR_NOT_RUNNING if the timer id does not match with the high priority
 * timer register, or
 * - @ref MAC_SUCCESS otherwise.
 */
retval_t pal_stop_high_priority_timer(uint8_t timer_id)
{
    retval_t timer_stop_status = PAL_TMR_NOT_RUNNING;

    /*Saving the current interrupt status & disabling the global interrupt */
    ENTER_CRITICAL_REGION();

    if (timer_id == high_priority_timer_id)
    {
        /* Turn off timer2/channel1 Outpur compare interrupt */
        HIGH_PRIORITY_TIMER_DISABLE_INT = DISABLE_ALL_TIMER_INTERRUPTS;

        /*Clear pending output compare matches */
        HIGH_PRIORITY_TIMER_ENABLE_INT  &=  ~AVR32_TC_IER0_CPAS_MASK;

        timer_array[high_priority_timer_id].next_timer_in_queue = NO_TIMER;
        timer_array[high_priority_timer_id].timer_cb = NULL;
        high_priority_timer_id = NO_TIMER;

        timer_stop_status = MAC_SUCCESS;
    }
    /*Restoring the interrupt status which was stored & enabling the global interrupt */
    LEAVE_CRITICAL_REGION();

    return timer_stop_status;
}
#endif /* #if ((defined ENABLE_HIGH_PRIO_TMR) || defined(DOXYGEN)) */



/*
 * This function is called to initialize the timer module.
 */
void timer_init(void)
{
#if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN))
    /*
     * Initialize the timer resources like, timer arrays
     * queues, timer registers
     */
    uint8_t index;

    running_timers = INTIALIZE_TO_ZERO;
    timer_trigger = false;

    running_timer_queue_head = NO_TIMER;
    expired_timer_queue_head = NO_TIMER;
    expired_timer_queue_tail = NO_TIMER;

    sys_time = 0;
#ifdef ENABLE_HIGH_PRIO_TMR
    high_priority_timer_id = NO_TIMER;
#endif
    for (index = 0; index < TOTAL_NUMBER_OF_TIMERS; index++)
    {
        timer_array[index].next_timer_in_queue = NO_TIMER;
        timer_array[index].timer_cb = NULL;
    }
    /* Do non-generic/PAL specific actions here */
    timer_init_non_generic();
#endif  /* #if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN)) */
}

/**
 * @brief Selects timer clock source
 *
 * This function selects the clock source of the timer.
 *
 * @param source
 * - @ref TMR_CLK_SRC_DURING_TRX_SLEEP if internal source is to be selected, and
 * - @ref EXTERNAL_CLOCK if external clock is required.
 */
void pal_timer_source_select(source_type_t source)
{
    /* This feature is not used in here & it is maintained already in the design */
    if (TMR_CLK_SRC_DURING_TRX_SLEEP == source)
    {
        /*Todo*/
    }
    else
    {
        /*Todo*/
    }
}

/**
 * @brief Gets current time
 *
 * This function returns the current time.
 *
 * @param[out] timer_count Current system time
 */
void pal_get_current_time(uint32_t *timer_count)
{
    *timer_count = gettime();
}



/**
 * @brief Generates blocking delay
 *
 * This functions generates a blocking delay of specified time.
 *
 * @param delay in microseconds
 */
void  pal_timer_delay(uint16_t delay)
{
    uint32_t volatile cur_count;
    /*
     * Any interrupt occurring during the delay calculation will introduce
     * additional delay and can also affect the logic of delay calculation.
     * Hence the delay implementation is put under critical region.
     */

    ENTER_CRITICAL_REGION();

    if (delay > MIN_DELAY_VAL)
    {
        /* Use one of the CPU counters to wait delay microseconds. */
        uint32_t ticks = CPU_COUNTER * delay;
        ticks += get_sys_reg(AVR32_COUNT);
        do
        {
            cur_count = get_sys_reg(AVR32_COUNT);
        } while (cur_count < ticks);
    }

    LEAVE_CRITICAL_REGION();
}


#if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN))
/**
 * @brief Checks if the timer of requested timer identifier is running
 *
 * This function checks if the timer of requested timer identifier is running.
 *
 * @param timer_id Timer identifier
 *
 * @return
 * - @ref true if timer with requested timer id is running,
 * - @ref false otherwise.
 */
bool pal_is_timer_running(uint8_t timer_id)
{
    if (NULL == timer_array[timer_id].timer_cb)
    {
        return false;
    }
    return true;
}
#endif  /* TOTAL_NUMBER_OF_TIMERS > 0 */


#if (DEBUG > 0)
/**
 * @brief Checks if all timers are stopped
 *
 * This function checks whether all timers are stopped or not.
 *
 * @return
 * - true if all timers are stopped,
 * - false otherwise.
 */
bool pal_are_all_timers_stopped(void)
{
    uint8_t timer_id;

    for (timer_id = 0; timer_id < TOTAL_NUMBER_OF_TIMERS; timer_id++)
    {
        if (NULL != timer_array[timer_id].timer_cb)
        {
            return false;
        }
    }
    return true;
}
#endif  /* (DEBUG > 0) */


#if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN))
/**
 * @brief Programs the output compare register TCC0 CCA
 *
 * This function programs the TCC0 CCA register with the timeout value of the
 * timer present at the head of the running timer queue.
 */
static void prog_ocr(void)
{
    uint16_t  timeout_high;
    uint16_t  timeout_low;
    uint32_t  timeout;
    uint32_t  timer_status;

    /*Saving the current interrupt status & disabling the global interrupt */
    ENTER_CRITICAL_REGION();

    if (NO_TIMER != running_timer_queue_head)
    {
        timeout = timer_array[running_timer_queue_head].abs_exp_timer;
        timeout_high = (uint16_t)(timeout >> SYS_TIME_SHIFT_MASK);

        if (timeout_high == TIMER_HIGH_REGISTER/*sys_time*/)
        {
            timeout_low = (uint16_t)(timeout & HW_TIME_MASK);

            /*
             * The status register is read to clear any pending compare match
             * interrupts.
             */
            timer_status = TC2_CH0.sr;


            /* Program output compare match */
            TIMER_COMP_REG = timeout_low;

            /* Enable output compare match interrupt */
            TIMER_COMP_ENABLE_REG |= TIMER_COMP_FLAG;

            /*
             * As the timer status register read above, it is necessary to check
             * whether the overflow bit is set. If the counter has overflowed
             * when the execution has just entered the function, no interrupt
             * will be generated, as all the callees of this function call this
             * function when in critical region. Reading the status register
             * will clear the overflow bit and hence it has to be acknowledged
             * here.
             */

            if ((timer_status & AVR32_TC_IDR2_COVFS_MASK) == AVR32_TC_IDR2_COVFS_MASK)
            {
                sys_time++;
            }
            /*
             * This is the only timer running in the timer queue, hence load the
             * OCR.
             */
            reg_rc_loaded = true;

        }
        else
        {
            /* Disable output compare match interrupt */
            TIMER_COMP_DISABLE_REG = TIMER_COMP_FLAG;
            reg_rc_loaded = false;
        }

        /*To get the current system time*/
        uint32_t current_time = gettime();

        /*Safety check: Trigger timer, if next_trigger is in the past */
        if (pal_compare_time(timer_array[running_timer_queue_head].abs_exp_timer,current_time+1))
        {
            timer_trigger = true;
        }
    }
    /*Restoring the interrupt status which was stored & enabling the global interrupt */
    LEAVE_CRITICAL_REGION();
}
#endif  /* #if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN)) */


#if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN))
/**
 * @brief Internal timer handler
 *
 * This function checks for expired timers and rearranges the
 * running timer queue head and expired timer queue head and tail
 * if there are any expired timers.
 */
void internal_timer_handler(void)
{
    /*
     * Flag is set when a timer expires. This flag is set by timer ISR or
     * prog_ocr() function
     */
    if (timer_trigger)
    {
        timer_trigger = false;

        if (running_timers > 0) /* Holds the number of running timers */
        {
            if ((expired_timer_queue_head == NO_TIMER) && (expired_timer_queue_tail == NO_TIMER))
            {
                expired_timer_queue_head = expired_timer_queue_tail = running_timer_queue_head;
            }
            else
            {
                timer_array[expired_timer_queue_tail].next_timer_in_queue = running_timer_queue_head;
                expired_timer_queue_tail = running_timer_queue_head;
            }

            running_timer_queue_head = timer_array[running_timer_queue_head].next_timer_in_queue;
            timer_array[expired_timer_queue_tail].next_timer_in_queue = NO_TIMER;
            running_timers--;

            /*
             * As a timer has expired, the OCR1A is programmed (if possible)
             * with the new timeout value of the timer pointed by running
             * timer queue head
             */
            prog_ocr();
        }
    }
}
#endif  /* #if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN)) */



#if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN))
/**
 * @brief Start a timer by setting its absolute expiration time
 *
 * This function starts a timer which should expire at the
 * point_in_time value and upon timer expiry the function
 * held by the handler is called.
 *
 * @param timer_id Timer identifier
 * @param point_in_time Absolute expiration time in microseconds
 * @param handler_cb Function called upon timer expiry
 * @param parameter Parameter passed to the expired timer handler
 */
static void start_absolute_timer(uint8_t timer_id,
                          uint32_t point_in_time,
                          void *handler_cb,
                          void *parameter)
{

    /*Saving the current interrupt status & disabling the global interrupt */
    ENTER_CRITICAL_REGION();

    /*Check is done to see if any timer has expired */
    internal_timer_handler();

    bool load_ocr = false;

    if (NO_TIMER == running_timer_queue_head)
    {
        running_timer_queue_head = timer_id;
        timer_array[timer_id].next_timer_in_queue = NO_TIMER;
        /*
         * This is the only timer running in the timer queue, hence load the
         * OCR.
         */
        load_ocr = true;
    }
    else
    {
        uint8_t i;
        bool timer_inserted = false;
        uint8_t next_index = running_timer_queue_head;
        uint8_t prev_index = running_timer_queue_head;

        for (i = 0; i < running_timers; i++)
        {
            if (NO_TIMER != next_index)
            {
                if (pal_compare_time(timer_array[next_index].abs_exp_timer,
                                     point_in_time))
                {
                    /*
                     * Requested absolute time value is greater than the time
                     * value pointed by the next_index in the timer array
                     */
                    prev_index = next_index;
                    next_index = timer_array[next_index].next_timer_in_queue;
                }
                else
                {
                    timer_array[timer_id].next_timer_in_queue = next_index;
                    if (running_timer_queue_head == next_index)
                    {
                        /* Insertion at the head of the timer queue */
                        running_timer_queue_head = timer_id;
                        /*
                         * Timer is inserted at the head of the queue, hence
                         * load the OCR.
                         */
                        load_ocr = true;
                    }
                    else
                    {
                        timer_array[prev_index].next_timer_in_queue = timer_id;
                    }
                    timer_inserted = true;
                    break;
                }
            }
        }
        if (!timer_inserted)
        {
            /* Insertion at the tail of the timer queue */
            timer_array[prev_index].next_timer_in_queue = timer_id;
            timer_array[timer_id].next_timer_in_queue = NO_TIMER;
        }
    }
    timer_array[timer_id].abs_exp_timer = point_in_time;
    timer_array[timer_id].timer_cb = (void *)handler_cb;
    timer_array[timer_id].param_cb = parameter;
    running_timers++;

    /*
     * If there is only one timer in the timer queue
     * the timeout should be loaded immediately
     */
    if (load_ocr)
    {
        prog_ocr();
    }

    /*Restoring the interrupt status which was stored & enabling the global interrupt */
    LEAVE_CRITICAL_REGION();
}
#endif  /* #if ((TOTAL_NUMBER_OF_TIMERS > 0) || defined(DOXYGEN)) */


#if ((PAL_TYPE != AT32UC3B1128) && (PAL_TYPE != AT32UC3B164))
#if defined(DOXYGEN)
/**
 * @brief Timer Overflow ISR
 *
 * This is the interrupt service routine for timer0 overflow.
 * It is triggered every 65536 * 8 us, if clk to timer is 1MHz.
 * The software portion of the system timer is incremented now.
 */
void TimerCounter1_isr(void);
#else  /* !DOXYGEN */

#if (defined __GNUC__)
    __attribute__((__interrupt__))
#elif (defined __ICCAVR32__)
    __interrupt
#endif
void TC_isr(void)
{
    uint32_t status = TC_CH2.sr;
    status &= TC_CH2.imr;

#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)
        /* Input Caprture interrupt check. */
    if((status & AVR32_TC_LDRAS_MASK) == AVR32_TC_LDRAS_MASK)
    {
        /* Timer InputCapture on channel 2. */
        read_timestamp();
    }
#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */
}
#endif
#endif



#if defined(DOXYGEN)
/**
 * @brief Timer Overflow ISR
 *
 * This is the interrupt service routine for timer0 overflow.
 * It is triggered every 65536 * 8 us, if clk to timer is 1MHz.
 * The software portion of the system timer is incremented now.
 */
void TimerCounter2_isr(void);
#else  /* !DOXYGEN */

#if (defined __GNUC__)
    __attribute__((__interrupt__))
#elif (defined __ICCAVR32__)
    __interrupt
#endif
void TC2_isr(void)
{
    uint32_t status_c0 = TC2_CH0.sr;
    uint32_t status_c1 = TC2_CH1.sr;
#if ((PAL_TYPE == AT32UC3B1128) || (PAL_TYPE == AT32UC3B164))
    uint32_t status_c2 = TC2_CH2.sr;
#endif
    status_c0 &= TC2_CH0.imr;
    status_c1 &= TC2_CH1.imr;
#if ((PAL_TYPE == AT32UC3B1128) || (PAL_TYPE == AT32UC3B164))
    status_c2 &= TC2_CH2.imr;
#endif
        /* Output compare interrupt is checked. */
    if ((status_c0 & AVR32_TC_IDR2_CPAS_MASK) == AVR32_TC_IDR2_CPAS_MASK)
    {
#if (TOTAL_NUMBER_OF_TIMERS > 0)
            /*Compare channel on channel 0.  */
        if ((reg_rc_loaded) && (running_timers > 0))
        {
            reg_rc_loaded = false;
            timer_trigger = true;
        }
#endif
    }
        /* Overflow interrupt is checked.*/
    if ((status_c0 & AVR32_TC_IDR2_COVFS_MASK) == AVR32_TC_IDR2_COVFS_MASK)
    {
        sys_time++;
        /* Overflow on channel 0.  */
#if (TOTAL_NUMBER_OF_TIMERS > 0)
        prog_ocr();
#endif
    }

#if ((PAL_TYPE == AT32UC3B1128) || (PAL_TYPE == AT32UC3B164))
#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)
    /* Input Caprture interrupt check. */
    if((status_c2 & AVR32_TC_LDRAS_MASK) == AVR32_TC_LDRAS_MASK)
    {
        /* Timer InputCapture on channel 2. */
        read_timestamp();
    }
#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */
#endif

    /* Output compare channel interrupt is checked. */
#if ((PAL_TYPE == AT32UC3B1128) || (PAL_TYPE == AT32UC3B164))
    if ((status_c2 & AVR32_TC_IDR2_CPAS_MASK) == AVR32_TC_IDR2_CPAS_MASK)
#else
    if ((status_c1 & AVR32_TC_IDR2_CPAS_MASK) == AVR32_TC_IDR2_CPAS_MASK)
#endif
    {
#ifdef ENABLE_HIGH_PRIO_TMR
        /* Turn off timer2 channel1 interrupt */
        HIGH_PRIORITY_TIMER_ENABLE_INT  &=  ~AVR32_TC_IER0_CPAS_MASK;
        timer_expiry_cb_t callback = (timer_expiry_cb_t)(timer_array[high_priority_timer_id].timer_cb);
        void *param = timer_array[high_priority_timer_id].param_cb;
        timer_array[high_priority_timer_id].timer_cb = NULL;
        timer_array[high_priority_timer_id].next_timer_in_queue = NO_TIMER;
        high_priority_timer_id = NO_TIMER;
        ASSERT(NULL != callback);
        /* The callback function registered for this timer is called */
        callback(param);
#endif /* defined(ENABLE_HIGH_PRIO_TMR) */
    }
}
#endif
/* EOF */

