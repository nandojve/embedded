/**
 * @file vendor_stack_config.h
 *
 * @brief Vendor stack configuration parameters
 *
 * This file defines a customer defined stack and needs to be adjusted
 * accordingly.
 *
 * $Id: vendor_stack_config.h,v 1.4 2011/06/30 15:00:24 dam Exp $
 */

/* Prevent double inclusion */
#ifndef VENDOR_STACK_CONFIG_H
#define VENDOR_STACK_CONFIG_H

/* The following defines a customer specific stack layer name. This layer
 * is residing on top of the MAC and thus needs a stack ID which is
 * the increment ID of the MAC layer.
 */
#define ROM                                  (MAC + 1)
#define SLOW                                 (ROM + 1) /* SIXLOWPAN = SixLOWpan = SLOW */


#if (HIGHEST_STACK_LAYER == ROM)
    /* Reduce the header file dependency by using hard-coded values */
    #define LARGE_BUFFER_SIZE               (160)
    #define SMALL_BUFFER_SIZE               (0)

#include "vendor_nwk_config.h"
#define NUMBER_OF_TOTAL_STACK_TIMERS        (NUMBER_OF_TAL_TIMERS + NUMBER_OF_MAC_TIMERS + NUMBER_OF_NWK_TIMERS)
#define LAST_STACK_TIMER_ID                 (NWK_LAST_TIMER_ID)
//#define NUMBER_OF_LARGE_STACK_BUFS          (7) // highest value used in MAC layer + 1 more for additional frame transmission (like DropChild or Sleeping)
#define NUMBER_OF_LARGE_STACK_BUFS          (20)
#define NUMBER_OF_SMALL_STACK_BUFS          (0)
#endif  /* (HIGHEST_STACK_LAYER == ROM) */

#if (HIGHEST_STACK_LAYER == SLOW)
    /* Reduce the header file dependency by using hard-coded values */
    #define LARGE_BUFFER_SIZE               (160)
    #define SMALL_BUFFER_SIZE               (0)

#include "vendor_nwk_config.h"
#define NUMBER_OF_TOTAL_STACK_TIMERS        (NUMBER_OF_TAL_TIMERS + NUMBER_OF_MAC_TIMERS + NUMBER_OF_NWK_TIMERS + NUMBER_OF_SLOW_TIMERS)
#define LAST_STACK_TIMER_ID                 (SLOW_LAST_TIMER_ID)
#define NUMBER_OF_LARGE_STACK_BUFS          (7) /* same value as used on NWK layer */
#define NUMBER_OF_SMALL_STACK_BUFS          (0)
#endif  /* (HIGHEST_STACK_LAYER == SIXLOWPAN) */

#endif /* VENDOR_STACK_CONFIG_H */
/* EOF */
