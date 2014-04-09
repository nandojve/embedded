/**
 * @file avr32types.h
 *
 * @brief Compatibility definitions for compilers (IAR, GCC)
 *
 * This file contains AVR32 type definitions that enable Atmel's 802.15.4
 * stack implementation to build using multiple compilers. This file
 * contains compiler conditional subsystems for AVR functions such as:
 * interrupts, sleep mode, USARTs, etc.
 *
 * $Id: avr32types.h 28116 2011-08-17 16:31:55Z yogesh.bellan $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> LICENSE.txt
 */

/* Prevent double inclusion */
#ifndef AVR32TYPES_H
#define AVR32TYPES_H

/* === Includes ============================================================= */

/*
 * The if defined(__ICCAVR32__) section below applies to the IAR compiler.
 */
#if defined(__ICCAVR32__)

#include <avr32/io.h>
#include <intrinsics.h>
#include <string.h>
#include <stdint.h>

#define get_sys_reg(reg)        __get_system_register(reg)
#define BEGIN_PACK              PRAGMA(pack(push, 1))
#define END_PACK                PRAGMA(pack(pop))
#define PACK

/*
 * The elif defined(__GNUC__) section below applies to the GNUC compiler.
 */
#elif defined(__GNUC__)

#include <avr32/io.h>
#include <string.h>
#include <stdint.h>

#define get_sys_reg(reg)         __builtin_mfsr(reg)

/*
 * The else section below applies to unsupported compiler. This is where support
 * for compilers other than IAR and GNUC would be placed.
 */
#else
#error Unsupported compiler
#endif /* compiler selection */


/* === Externals ============================================================ */


/* === Macros =============================================================== */

/* Union of pointers to constant volatile 64-, 32-, 16- and 8-bit unsigned integers. */
typedef union
{
  const volatile signed long long int *s64ptr;
  const volatile uint64_t *u64ptr;
  const volatile signed long int *s32ptr;
  const volatile uint32_t *u32ptr;
  const volatile signed short int *s16ptr;
  const volatile uint16_t *u16ptr;
  const volatile signed char  *s8ptr ;
  const volatile uint8_t  *u8ptr ;
} UnionCVPtr;

/* Union of pointers to volatile 64-, 32-, 16- and 8-bit unsigned integers. */
typedef union
{
  volatile signed long long int *s64ptr;
  volatile uint64_t *u64ptr;
  volatile signed long int *s32ptr;
  volatile uint32_t *u32ptr;
  volatile signed short int *s16ptr;
  volatile uint16_t *u16ptr;
  volatile signed char  *s8ptr ;
  volatile uint8_t  *u8ptr ;
} UnionVPtr;


/* Union of pointers to constant 64-, 32-, 16- and 8-bit unsigned integers. */
typedef union
{
  const signed long long int *s64ptr;
  const uint64_t *u64ptr;
  const signed long int *s32ptr;
  const uint32_t *u32ptr;
  const signed short int *s16ptr;
  const uint16_t *u16ptr;
  const signed char  *s8ptr ;
  const uint8_t  *u8ptr ;
} UnionCPtr;

/* Structure of pointers to constant volatile 64-, 32-, 16- and 8-bit unsigned integers. */
typedef struct
{
  const volatile signed long long int *s64ptr;
  const volatile uint64_t *u64ptr;
  const volatile signed long int *s32ptr;
  const volatile uint32_t *u32ptr;
  const volatile signed short int *s16ptr;
  const volatile uint16_t *u16ptr;
  const volatile signed char  *s8ptr ;
  const volatile uint8_t  *u8ptr ;
} StructCVPtr;

/* 64-bit union. */
typedef union
{
  signed long long int s64   ;
  uint64_t u64   ;
  signed long int s32[2];
  uint32_t u32[2];
  signed short int s16[4];
  uint16_t u16[4];
  signed char  s8 [8];
  uint8_t  u8 [8];
} Union64;

/* Reads the bits of a value specified by a given bit-mask. */
#define Rd_bits( value, mask)        ((value) & (mask))

/* Writes the bits of a C lvalue specified by a given bit-mask. */
#define Wr_bits(lvalue, mask, bits)  ((lvalue) = ((lvalue) & ~(mask)) |\
                                                 ((bits  ) &  (mask)))

/* Tests the bits of a value specified by a given bit-mask. */
#define Tst_bits( value, mask)  (Rd_bits(value, mask) != 0)


/* Aligns the number \a val with the lower \a n boundary. */
#define Align_down(val, n     ) ( (val)              & ~((n) - 1UL))


/* Tests alignment of the number \a val with the \a n boundary. */
#define Test_align(val, n     ) (!Tst_bits( val, (n) - 1     )   )


/* Gets alignment of the number \a val with respect to the \a n boundary. */
#define Get_align( val, n     ) (  Rd_bits( val, (n) - 1     )   )

/* @brief Takes the minimal value of \a a and \a b.
 *
 * @param a Input value.
 * @param b Input value.
 *
 * @return Minimal value of \a a and \a b.
 *
 * @note More optimized if only used with values known at compile time.
 */
#define Min(a, b)           (((a) < (b)) ?  (a) : (b))

/* @brief Takes the maximal value of \a a and \a b.
 *
 * @param a Input value.
 * @param b Input value.
 *
 * @return Maximal value of \a a and \a b.
 *
 * @note More optimized if only used with values known at compile time.
 */
#define Max(a, b)           (((a) > (b)) ?  (a) : (b))

/* Takes the minimal value of \a a and \a b. */
#if (defined __GNUC__)
  #define min(a, b) \
  (\
    {\
      int __value, __arg_a = (a), __arg_b = (b);\
      __asm__ ("min\t%0, %1, %2" : "=r" (__value) : "r" (__arg_a), "r" (__arg_b));\
      __value;\
    }\
  )
#elif (defined __ICCAVR32__)
  #define min(a, b)   __min(a, b)
#endif

/* @brief Takes the maximal value of \a a and \a b.
 *
 * @param a Input value.
 * @param b Input value.
 *
 * @return Maximal value of \a a and \a b.
 *
 * @note More optimized if only used with values unknown at compile time.
 */
#if (defined __GNUC__)
  #define max(a, b) \
  (\
    {\
      int __value, __arg_a = (a), __arg_b = (b);\
      __asm__ ("max\t%0, %1, %2" : "=r" (__value) : "r" (__arg_a), "r" (__arg_b));\
      __value;\
    }\
  )
#elif (defined __ICCAVR32__)
  #define max(a, b)   __max(a, b)
#endif

#ifndef _BV
/**
 * Bit value -- compute the bitmask for a bit position
 */
#define _BV(x) (1 << (x))
#endif

/* @brief Counts the leading zero bits of the given value considered as a 32-bit integer.
 *
 * @param u Value of which to count the leading zero bits.
 *
 * @return The count of leading zero bits in \a u.
 */
#if (defined __GNUC__)
  #define clz(u)              __builtin_clz(u)
#elif (defined __ICCAVR32__)
  #define clz(u)              __count_leading_zeros(u)
#endif

/**
 * This macro saves the IRQ status and is typically used before cli().
 */
#define save_irq_status() (sreg = SREG)

/**
 * This macro must be used together with save_irq_status() and it will
 * restore the content of the IRQ status.
 */
#define restore_irq_status() (SREG = sreg)


/*
 * This block contains just the documentation for all
 * compiler-dependent macros.
 */
#if defined(DOXYGEN)

/**
 * Start an interrupt handler for vector \c vec.
 */
#define ISR(vec)

/**
 * Globally enable interrupts.
 */
#define sei()
/**
 * Globally disable interrupts.
 */
#define cli()
/**
 * Read contents of EEPROM cell \c addr into variable \c var.
 */
#define EEGET(var, addr)
/**
 * Null operation: just waste one CPU cycle.
 */
#define nop()
/**
 * Clear (i.e. reset to 0) the watchdog timer.
 */
#define WDT_RESET()

/**
 * Put the CPU to sleep, according to the sleep mode selected
 * with \c set_sleep_mode.
 */
#define SLEEP_MODE()
/**
 * Enter idle sleep mode: turn off the CPU, but keep all clocks running.
 */
#define SLEEP_MODE_IDLE
/**
 * Stop the CPU but keep the ADC running.
 */
#define SLEEP_MODE_ADC
/**
 * Stop the CPU and all oscillators.
 */
#define SLEEP_MODE_PWR_DOWN
/**
 * Stop the CPU and all oscillators but the asynchronous one.
 */
#define SLEEP_MODE_PWR_SAVE
/**
 * Stop the CPU but keep the external oscillator running.
 */
#define SLEEP_MODE_STANDBY
/**
 * Like power-save mode but keep the external oscillator running.
 */
#define SLEEP_MODE_EXT_STANDBY
/**
 * All sleep mode bits.
 */
#define SLEEP_MODE_MASK
/**
 * Set the sleep mode to enter upon the next \c sleep_mode call.
 */
#define SET_SLEEP_MODE(x)

/* program memory space abstraction */
/**
 * Declare object \c x to be placed into flash ROM.
 */
#define FLASH_DECLARE(x)
/**
 * Declare string \c x to be placed into flash ROM.
 */
#define FLASH_STRING(x)
/**
 * Type of a string that is located in flash ROM.
 */
#define FLASH_STRING_T
/**
 * Read one byte at address \c x from flash ROM.
 */
#define PGM_READ_BYTE(x)
/**
 * Read two bytes at address \c x from flash ROM.
 */
#define PGM_READ_WORD(x)
/**
 * Read a block of \c len bytes at address \c src from
 * flash ROM to RAM address \c dst.
 */
#define PGM_READ_BLOCK(dst, src, len)
/**
 * Determine the string length of a string locatend in
 * flash ROM at address \c x.
 */
#define PGM_STRLEN(x)
/**
 * Copy a string from flash ROM at address \c src to RAM address \c dst.
 */
#define PGM_STRCPY(dst, src)
/**
 * Set to one if the compilation environment supports a vsnprintf_P() function
 * (like vsnprintf() but with the format string in flash ROM).
 */
#define HAS_PGM_VSNPRINTF
/**
 * Define the function to use for printing a string with the format string in
 * flash ROM, parameters are similar to vsnprintf().
 */
#define PGM_VSNPRINTF(dst, n, fmt, ap)
/**
 * Define this to the percent format specifier used to indicate a string that
 * is located in flash ROM rather than in RAM.  Do not define this macro if
 * the compilation environment does not support taking arguments from flash
 * ROM in printf()-like functions.
 */
#define PRINTF_FLASH_STRING

/**
 * Update the 16-bit CCITT \c crc from the next \c data byte, returning
 * the new CRC.
 */
#define CRC_CCITT_UPDATE(crc, data)

/**
 * Attribute to apply to struct tags in order to force it into an 8-bit
 * alignment.
 */
#define ALIGN8BIT

/**
 * Attribute to apply to an enum declaration to force it into the smallest
 * type size required to represent all values.
 */
#define SHORTENUM

/**
 * Attribute to apply to a structure declaration to force it into the smallest
 * type size required to represent all values.
 */
#define PACKED

/**
 * The name of a low-level initialization function that is run before
 * initializing main().
 */
#define ENABLE_XRAM(void)

/**
 * The return value to be returned by ENABLE_XRAM(), if any.
 */
#define RETURN_ENABLE_XRAM

/**
 * Perform a delay of \c us microseconds.
 *
 * The macro F_CPU is supposed to be defined to a
 * constant defining the CPU clock frequency (in Hertz).
 *
 * The maximal possible delay is 262.14 ms / F_CPU in MHz.
 *
 * \note For the IAR compiler, currently F_CPU must be a
 * multiple of 1000000UL (1 MHz).
 */
extern void delay_us(double us);

/**
 * This macros tells whether the compiler can properly initialize C99
 * flexible array members at the end of a struct.  If not, in some
 * situations inflexible arrays must be used which can waste memory.
 */
#define CAN_INITIALIZE_FLEXIBLE_ARRAY_MEMBERS 0

#endif /* defined(DOXYGEN) */


#if defined(__ICCAVR32__)
/*
 * Some preprocessor magic to allow for a header file abstraction of
 * interrupt service routine declarations for the IAR compiler.  This
 * requires the use of the C99 _Pragma() directive (rather than the
 * old #pragma one that could not be used as a macro replacement), as
 * well as two different levels of preprocessor concetanations in
 * order to do both, assign the correct interrupt vector name, as well
 * as construct a unique function name for the ISR.
 *
 * Do *NOT* try to reorder the macros below, or you'll suddenly find
 * out about all kinds of IAR bugs...
 */
#define PRAGMA(x) _Pragma( #x )
#define ISR(vec) PRAGMA( vector = vec ) __interrupt void handler_##vec(void)

#define sei() __enable_interrupt()
#define cli() __disable_interrupt()
#define EEGET(var, addr) __EEGET(var, addr)
#define nop() __no_operation()
#define WDT_RESET() __watchdog_reset()

/* program memory space abstraction */
#define FLASH_DECLARE(x) x
#define FLASH_STRING(x) ((const char *)(x))
#define FLASH_STRING_T  char const *
#define PGM_READ_BYTE(x) *(x)
#define PGM_READ_WORD(x) *(x)
#define memcpy_P    memcpy
#define strcpy_P    strcpy
#define strlen_P    strlen
#define printf_P    printf
#define PUTS(s)  { static const char  c[]= s; printf_P(c); }
#define PRINTF(fmt, ...) { static const char c[]= fmt; printf_P(c, __VA_ARGS__); }
#define PGM_READ_BLOCK(dst, src, len) memcpy_P((dst), (src), (len))
#define PGM_STRLEN(x) strlen_P(x)
#define PGM_STRCPY(dst, src) strcpy_P((dst), (src))
/* IAR has no vsnprintf_P(), and no %S format. */
#define HAS_PGM_VSNPRINTF 0
#define PGM_VSNPRINTF(dst, n, fmt, ap) vsnprintf((dst), (n), (fmt), (ap))
#undef PRINTF_FLASH_STRING

#define ALIGN8BIT /**/
#define SHORTENUM /**/
#define PACKED    /**/

#define ENABLE_XRAM(void) \
char __low_level_init(void)
#define RETURN_ENABLE_XRAM return 1

#define delay_us(us)   __delay_cycles((F_CPU/1000000UL) * us)

/*Program Memory Space Storage abstraction definition*/
#define CMD_ID_OCTET    (3)

#endif /* defined(__ICCAVR32__) */

#define CAN_INITIALIZE_FLEXIBLE_ARRAY_MEMBERS 0

#ifndef BIG_ENDIAN
#define BIG_ENDIAN
#endif

/* Converts a 8 Byte array into a 64-Bit value */
static inline uint64_t convert_byte_array_to_64_bit(uint8_t *data)
{
    union
    {
        uint64_t u64;
        uint8_t u8[8];
    }long_addr;
    uint8_t index;
    for (index = 0; index <= 7; index++)
    {
        long_addr.u8[index] = *data++;
    }

    return long_addr.u64;
}

/* Converts a 64-Bit value into a 2 Byte array */
#define convert_64_bit_to_byte_array(value, data) \
    memcpy((data), (&(value)), sizeof(uint64_t))


/* Converts a 2 Byte array into a 16-Bit value */
static inline uint16_t convert_byte_array_to_16_bit(uint8_t *data)
{
    return (data[1] | ((uint16_t)data[0] << 8));
}

/* Converts a 16-Bit value into a 2 Byte array */
static inline void convert_16_bit_to_byte_array(uint16_t value, uint8_t *data)
{
    data[1] = value & 0xFF;
    data[0] = (value >> 8) & 0xFF;
}

/* Converts a 8 Byte array into a 32-Bit value */
static inline uint32_t convert_byte_array_to_32_bit(uint8_t *data)
{
    union
    {
        uint32_t u32;
        uint8_t u8[8];
    }long_addr;
    uint8_t index;
    for (index = 0; index <= 4; index++)
    {
        long_addr.u8[index] = *data++;
    }
    return long_addr.u32;
}

/* Converts a 32-Bit value into a 2 Byte array */
#define convert_32_bit_to_byte_array(value, data) \
    memcpy((data), (&(value)), sizeof(uint32_t))

/* Converts a 16-Bit value into a 2 Byte array */
static inline void convert_spec_16_bit_to_byte_array(uint16_t value, uint8_t *data)
{
    data[0] = value & 0xFF;
    data[1] = (value >> 8) & 0xFF;
}

/* Converts a 16-Bit value into a 2 Byte array */
static inline void convert_16_bit_to_byte_address(uint64_t value, uint8_t *data)
{
    data[1] = (value >> 48) & 0xFF;
    data[0] = (value >> 56) & 0xFF;
}

/* Various macroses to swap bytes */
// Toggles the endianism of u16 (by swapping its bytes).
#if (defined __GNUC__)
  #define swap16(x) ((uint16_t)__builtin_bswap_16((uint16_t)(x)))
#elif (defined __ICCAVR32__)
  #define swap16(x) ((uint16_t)__swap_bytes_in_halfwords((uint16_t)(x)))
#else
  #error unknown compiler
#endif

// Toggles the endianism of u32 (by swapping its bytes).
#if (defined __GNUC__)
  #define swap32(x) ((uint32_t)__builtin_bswap_32((uint32_t)(x)))
#elif (defined __ICCAVR32__)
  #define swap32(x) ((uint32_t)__swap_bytes((uint32_t)(x)))
#else
  #error unknown compiler
#endif

// Toggles the endianism of u64 (by swapping its bytes).
#define swap64(x) ((uint64_t)(((uint64_t)swap32((uint64_t)(x) >> 32)) | ((uint64_t)swap32((uint64_t)(x)) << 32)))



#if defined(__GNUC__)

#define __GNUAVR32__

#define volatile            __volatile__

#define nop() do { __asm__ __volatile__ ("nop"); } while (0)
#define EEGET(var, addr)    nop()
#define ALIGN8BIT /**/
#define PROGMEM
#define RAMFUNCTION
#define WDT_RESET() wdt_reset()
#define printf_P    printf
#define PUTS(s) { static const char c[] PROGMEM = s; printf_P(c); }
#define PRINTF(fmt, ...) { static const char c[] PROGMEM = fmt; printf_P(c, __VA_ARGS__); }

/*
 * Provided by GCC to set the alignment of members of structure and union to 1.
 */
#define SHORTENUM           __attribute__((packed))
#define PACKED              __attribute__((packed))

/* program memory space abstraction */
#define FLASH_EXTERN(x) extern const x
#define FLASH_DECLARE(x)  const x
#define FUNC_PTR void *
#define FLASH_STRING(x) ((const char *)(x))
#define FLASH_STRING_T char const *
#define PGM_READ_BYTE(x) *(x)
#define PGM_READ_WORD(x) *(x)
#define PGM_READ_BLOCK(dst, src, len) memcpy((dst), (src), (len))
#define PGM_STRLEN(x) strlen(x)
#define PGM_STRCPY(dst, src) strcpy((dst), (src))


/*Program Memory Space Storage abstraction definition*/
#define CMD_ID_OCTET    (0)

#define STRINGZ(x)                                #x
/**
 * @brief Sets the MCU in the specified sleep mode.
 *
 * @param mode Sleep mode:
 * @arg AVR32_PM_SMODE_IDLE: Idle;
 * @arg AVR32_PM_SMODE_FROZEN: Frozen;
 * @arg AVR32_PM_SMODE_STANDBY: Standby;
 * @arg AVR32_PM_SMODE_STOP: Stop;
 * @arg AVR32_PM_SMODE_DEEP_STOP: DeepStop;
 * @arg AVR32_PM_SMODE_STATIC: Static.
 */
#define SLEEP(mode)   {__asm__ __volatile__ ("sleep "STRINGZ(mode));}

#endif /* defined(__GNUC__) */


/* Converting of values from CPU endian to little endian. */
#define CPU_ENDIAN_TO_LE16(x) swap16(x)
#define CPU_ENDIAN_TO_LE32(x) swap32(x)
#define CPU_ENDIAN_TO_LE64(x) swap64(x)

/* Converting of values from little endian to CPU endian. */
#define LE16_TO_CPU_ENDIAN(x) swap16(x)
#define LE32_TO_CPU_ENDIAN(x) swap32(x)
#define LE64_TO_CPU_ENDIAN(x) swap64(x)

/* Converting of constants from CPU endian to little endian. */
#define CCPU_ENDIAN_TO_LE16(x) ((uint16_t)(\
  (((uint16_t)(x) & (uint16_t)0x00ffU) << 8) | \
  (((uint16_t)(x) & (uint16_t)0xff00U) >> 8)))

#define CCPU_ENDIAN_TO_LE32(x) ((uint32_t)(\
  (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
  (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) | \
  (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) | \
  (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

#define CCPU_ENDIAN_TO_LE64(x) ((uint64_t)(\
  (((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) | \
  (((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) | \
  (((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) | \
  (((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) | \
  (((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) | \
  (((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) | \
  (((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) | \
  (((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56)))

/* Converting of constants from little endian to CPU endian. */
#define CLE16_TO_CPU_ENDIAN(x) CCPU_ENDIAN_TO_LE16(x)
#define CLE32_TO_CPU_ENDIAN(x) CCPU_ENDIAN_TO_LE32(x)
#define CLE64_TO_CPU_ENDIAN(x) CCPU_ENDIAN_TO_LE64(x)

/**
 * Address copy from the source to the Destination Memory
 */
#define ADDR_COPY_DST_SRC_16(dst, src)  memcpy((&(dst)), (&(src)), sizeof(uint16_t))
#define ADDR_COPY_DST_SRC_64(dst, src)  do {dst=src;}while(0)

#define MEMCPY_ENDIAN memcpy_be

/* === Types ================================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef cplusplus
} /* extern "C" */
#endif

/* Copy char s2[n] to s1[n] in any order */
static inline void *memcpy_be(void *s1, const void *s2, char n)
{
   char *su1 = (char *)s1;
   const char *su2 = (const char *)s2;
   signed char count=0x00,count1=0x00;
   if((n-1) == 0)
   {
        *(su1+count1) = *(su2+count);
   }
   for (count = (n-1),count1 = 0; count >= 0;)
   {
        *(su1+count1++) = *(su2+count--);
   }
   return (s1);
}
#endif /* AVRTYPES_H */
/* EOF */
