/**
 * @file nwk_config.h
 *
 * @brief These are all timer IDs which are used in an example NWK stack layer.
 *        This file needs to be adjusted accordingly.
 *
 * $Id: vendor_nwk_config.h,v 1.3 2011/06/30 15:00:24 dam Exp $
 */

/* Prevent double inclusion */
#ifndef VENDOR_NWK_CONFIG_H
#define VENDOR_NWK_CONFIG_H

/* === Includes ============================================================= */

#include "mac_config.h"

/* === Macros =============================================================== */


#define NWK_FIRST_TIMER_ID              (MAC_LAST_TIMER_ID + 1)

#define SLOW_FIRST_TIMER_ID             (NWK_LAST_TIMER_ID + 1)

/* === Types ================================================================ */

/* HIGHEST_STACK_LAYER == ROM */

/* Timer ID's used by NWK Layer */
typedef enum
{
    /* NWK Timers start from NWK_FIRST_TIMER_ID */
    T_Join 	= (NWK_FIRST_TIMER_ID),
    T_Leave = (NWK_FIRST_TIMER_ID + 1)
    /* Add more if required */
} nwk_timer_t;

/* Adjust according to number of timers defined above. */
#define NUMBER_OF_NWK_TIMERS            (2)

#define NWK_LAST_TIMER_ID               (MAC_LAST_TIMER_ID + NUMBER_OF_NWK_TIMERS)


/* HIGHEST_STACK_LAYER == SLOW */

/* Timer ID's used by SLOW (SIXLOWPAN) Layer */
typedef enum
{
    /* SLOW Timers start from SLOW_FIRST_TIMER_ID */
    T_Test  = (SLOW_FIRST_TIMER_ID),
    T_Reass = (SLOW_FIRST_TIMER_ID + 1)
    /* Add more if required */
} slow_timer_t;

/* Adjust according to number of timers defined above. */
#define NUMBER_OF_SLOW_TIMERS            (2)

#define SLOW_LAST_TIMER_ID               (NWK_LAST_TIMER_ID + NUMBER_OF_SLOW_TIMERS)




#ifdef ENABLE_QUEUE_CAPACITY
/**
 * Macro configuring the queue capacities.
 */
/**
 * Capacity of queue between NWK and Next Higher Layer
 */
#define NWK_NHLE_QUEUE_CAPACITY         255
//#define NHLE_NWK_QUEUE_CAPACITY         (x)
//#define MAC_NWK_QUEUE_CAPACITY          (z)
#endif  /* ENABLE_QUEUE_CAPACITY */

/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* VENDOR_NWK_CONFIG_H */
/* EOF */
