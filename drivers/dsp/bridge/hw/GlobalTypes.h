/*
 * GlobalTypes.h
 *
 * DSP-BIOS Bridge driver support functions for TI OMAP processors.
 *
 * Copyright (C) 2007 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __GLOBALTYPES_H
#define __GLOBALTYPES_H

/* ============================================================================
* INCLUDE FILES (only if necessary)
* =============================================================================
*/
#ifdef __cplusplus
extern "C"
{
#endif

/* ============================================================================
* EXPORTED DEFINITIONS
* =============================================================================
*/

/* ----------------------------------------------------------------------------
* Definition: TRUE, FALSE
*
* DESCRIPTION:  Boolean Definitions
*
* -----------------------------------------------------------------------------
*/
#ifndef TRUE
#define FALSE           0
#define TRUE            (!(FALSE))
#endif

/* ----------------------------------------------------------------------------
* Definition: NULL
*
* DESCRIPTION:  Invalid pointer
*
* -----------------------------------------------------------------------------
*/
#ifndef NULL
#define NULL        (void *)0
#endif

/* ----------------------------------------------------------------------------
* Definition: RET_CODE_BASE
*
* DESCRIPTION:  Base value for return code offsets
*
* -----------------------------------------------------------------------------
*/
#define RET_CODE_BASE        0

/* ----------------------------------------------------------------------------
* Definition: *BIT_OFFSET
*
* DESCRIPTION:  offset in bytes from start of 32-bit word.
*
* -----------------------------------------------------------------------------
*/
#define LOWER_16BIT_OFFSET          0
#define UPPER_16BIT_OFFSET          2

#define LOWER_8BIT_OFFSET           0
#define LOWER_MIDDLE_8BIT_OFFSET    1
#define UPPER_MIDDLE_8BIT_OFFSET    2
#define UPPER_8BIT_OFFSET           3

#define LOWER_8BIT_OF16_OFFSET      0
#define UPPER_8BIT_OF16_OFFSET      1

/* ----------------------------------------------------------------------------
* Definition: *BIT_SHIFT
*
* DESCRIPTION:  offset in bits from start of 32-bit word.
*
* -----------------------------------------------------------------------------
*/
#define LOWER_16BIT_SHIFT          0
#define UPPER_16BIT_SHIFT          16

#define LOWER_8BIT_SHIFT           0
#define LOWER_MIDDLE_8BIT_SHIFT    8
#define UPPER_MIDDLE_8BIT_SHIFT    16
#define UPPER_8BIT_SHIFT           24

#define LOWER_8BIT_OF16_SHIFT      0
#define UPPER_8BIT_OF16_SHIFT      8


/* ----------------------------------------------------------------------------
* Definition: LOWER_16BIT_MASK
*
* DESCRIPTION:  16 bit mask used for inclusion of lower 16 bits i.e. mask out
*               the upper 16 bits
*
* -----------------------------------------------------------------------------
*/
#define LOWER_16BIT_MASK        0x0000FFFF


/* ----------------------------------------------------------------------------
* Definition: LOWER_8BIT_MASK
*
* DESCRIPTION:  8 bit masks used for inclusion of 8 bits i.e. mask out
*               the upper 16 bits
*
* -----------------------------------------------------------------------------
*/
#define LOWER_8BIT_MASK           0x000000FF

/* ----------------------------------------------------------------------------
* Definition: RETURN_32BITS_FROM_16LOWER_AND_16UPPER(lower16Bits, upper16Bits)
*
* DESCRIPTION:  Returns a 32 bit value given a 16 bit lower value and a 16
*               bit upper value
*
* -----------------------------------------------------------------------------
*/
#define RETURN_32BITS_FROM_16LOWER_AND_16UPPER(lower16Bits,upper16Bits)\
	(((((u32)lower16Bits)  & LOWER_16BIT_MASK)) | \
	(((((u32)upper16Bits) & LOWER_16BIT_MASK) << UPPER_16BIT_SHIFT)))

/* ----------------------------------------------------------------------------
* Definition: RETURN_16BITS_FROM_8LOWER_AND_8UPPER(lower16Bits, upper16Bits)
*
* DESCRIPTION:  Returns a 16 bit value given a 8 bit lower value and a 8
*               bit upper value
*
* -----------------------------------------------------------------------------
*/
#define RETURN_16BITS_FROM_8LOWER_AND_8UPPER(lower8Bits,upper8Bits)\
	(((((u32)lower8Bits)  & LOWER_8BIT_MASK)) | \
	(((((u32)upper8Bits) & LOWER_8BIT_MASK) << UPPER_8BIT_OF16_SHIFT)))
/* ----------------------------------------------------------------------------
* Definition: RETURN_32BITS_FROM_4_8BIT_VALUES(
*                                       lower8Bits,
*                                        lowerMiddle8Bits,
*                                        lowerUpper8Bits,
*                                        upper8Bits)
*
* DESCRIPTION:  Returns a 32 bit value given four 8 bit values
*
* -----------------------------------------------------------------------------
*/
#define RETURN_32BITS_FROM_4_8BIT_VALUES(lower8Bits, lowerMiddle8Bits, lowerUpper8Bits, upper8Bits)\
	(((((u32)lower8Bits)        & LOWER_8BIT_MASK)) | \
	(((((u32)lowerMiddle8Bits) & LOWER_8BIT_MASK) << LOWER_MIDDLE_8BIT_SHIFT)) | \
	(((((u32)lowerUpper8Bits)  & LOWER_8BIT_MASK) << UPPER_MIDDLE_8BIT_SHIFT)) | \
	(((((u32)upper8Bits)       & LOWER_8BIT_MASK) << UPPER_8BIT_SHIFT)))


/* ----------------------------------------------------------------------------
* Definition: READ_LOWER_16BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 16 lower bits of 32bit value
*
* -----------------------------------------------------------------------------
*/
#define READ_LOWER_16BITS_OF_32(value32bits)\
	((u16)((u32)(value32bits) & LOWER_16BIT_MASK))

/* ----------------------------------------------------------------------------
* Definition: READ_UPPER_16BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 16 lower bits of 32bit value
*
* -----------------------------------------------------------------------------
*/
#define READ_UPPER_16BITS_OF_32(value32bits)\
	(((u16)((u32)(value32bits) >> UPPER_16BIT_SHIFT)) & LOWER_16BIT_MASK)


/* ----------------------------------------------------------------------------
* Definition: READ_LOWER_8BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 8 lower bits of 32bit value
*
* -----------------------------------------------------------------------------
*/
#define READ_LOWER_8BITS_OF_32(value32bits)\
	((u8)((u32)(value32bits) & LOWER_8BIT_MASK))

/* ----------------------------------------------------------------------------
* Definition: READ_LOWER_MIDDLE_8BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 8 lower middle bits of 32bit value
*
* -----------------------------------------------------------------------------
*/
#define READ_LOWER_MIDDLE_8BITS_OF_32(value32bits)\
	(((u8)((u32)(value32bits) >> LOWER_MIDDLE_8BIT_SHIFT)) & LOWER_8BIT_MASK)

/* ----------------------------------------------------------------------------
* Definition: READ_LOWER_MIDDLE_8BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 8 lower middle bits of 32bit value
*
* -----------------------------------------------------------------------------
*/
#define READ_UPPER_MIDDLE_8BITS_OF_32(value32bits)\
	(((u8)((u32)(value32bits) >> LOWER_MIDDLE_8BIT_SHIFT)) & LOWER_8BIT_MASK)

/* ----------------------------------------------------------------------------
* Definition: READ_UPPER_8BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 8 upper bits of 32bit value
*
* -----------------------------------------------------------------------------
*/
#define READ_UPPER_8BITS_OF_32(value32bits)\
	(((u8)((u32)(value32bits) >> UPPER_8BIT_SHIFT)) & LOWER_8BIT_MASK)


/* ----------------------------------------------------------------------------
* Definition: READ_LOWER_8BITS_OF_16(value16bits)
*
* DESCRIPTION:  Returns a 8 lower bits of 16bit value
*
* -----------------------------------------------------------------------------
*/
#define READ_LOWER_8BITS_OF_16(value16bits)\
	((u8)((u16)(value16bits) & LOWER_8BIT_MASK))

/* ----------------------------------------------------------------------------
* Definition: READ_UPPER_8BITS_OF_16(value32bits)
*
* DESCRIPTION:  Returns a 8 upper bits of 16bit value
*
* -----------------------------------------------------------------------------
*/
#define READ_UPPER_8BITS_OF_16(value16bits)\
	(((u8)((u32)(value16bits) >> UPPER_8BIT_SHIFT)) & LOWER_8BIT_MASK)

/* ============================================================================
* EXPORTED TYPES
* =============================================================================
*/

/* ----------------------------------------------------------------------------
* TYPE: u8, UCHAR, WORD8
*
* DESCRIPTION:  8 bit tpyes
*
* -----------------------------------------------------------------------------
*/
#ifndef OMAPBRIDGE_TYPES
typedef unsigned char  UCHAR;
#endif
typedef signed   char  WORD8;

/* ----------------------------------------------------------------------------
* TYPE: u16, WORD16
*
* DESCRIPTION:  16 bit tpyes
*
* -----------------------------------------------------------------------------
*/
typedef          short WORD16;

/* ----------------------------------------------------------------------------
* TYPE: u32, WORD32
*
* DESCRIPTION:  32 bit tpyes
*
* -----------------------------------------------------------------------------
*/
typedef          long  WORD32;

/* ----------------------------------------------------------------------------
* TYPE: REG_u8, REG_WORD8
*
* DESCRIPTION: 8 bit register types.
*
* -----------------------------------------------------------------------------
*/
typedef volatile unsigned char  REG_u8;
typedef volatile signed   char  REG_WORD8;

/* ----------------------------------------------------------------------------
* TYPE: REG_u16, REG_WORD16
*
* DESCRIPTION: 16 bit register types.
*
* -----------------------------------------------------------------------------
*/
#ifndef OMAPBRIDGE_TYPES
typedef volatile unsigned short REG_u16;
#endif
typedef volatile          short REG_WORD16;

/* ----------------------------------------------------------------------------
* TYPE: REG_u32, REG_WORD32
*
* DESCRIPTION: 32 bit register types.
*
* -----------------------------------------------------------------------------
*/
typedef volatile unsigned long  REG_u32;
typedef volatile          long  REG_WORD32;

/* ----------------------------------------------------------------------------
* TYPE: FLOAT
*
* DESCRIPTION: Type to be used for floating point calculation. Note that
*              floating point calculation is very CPU expensive, and you should
*              only  use if you absolutely need this.
*
* -----------------------------------------------------------------------------
*/
#ifndef OMAPBRIDGE_TYPES
typedef float  FLOAT;
#endif

/* ----------------------------------------------------------------------------
* TYPE: boolean_t
*
* DESCRIPTION:  Boolean Type True, False
*
* -----------------------------------------------------------------------------
*/
#ifndef OMAPBRIDGE_TYPES
typedef enum boolean_label {
	False = 0,
	True = 1
} boolean_t, pBoolean_t;

/* Boolean  Definition */
#define BOOL           boolean_t
#endif

/* ----------------------------------------------------------------------------
* TYPE: ReturnCode_t
*
* DESCRIPTION:  Return codes to be returned by all library functions
*
* -----------------------------------------------------------------------------
*/
typedef enum ReturnCode_label {
	RET_OK = 0,
	RET_FAIL = -1,
	RET_BAD_NULL_PARAM = -2,
	RET_PARAM_OUT_OF_RANGE = -3,
	RET_INVALID_ID = -4,
	RET_EMPTY = -5,
	RET_FULL = -6,
	RET_TIMEOUT = -7,
	RET_INVALID_OPERATION = -8,

    /* Add new error codes at end of above list */
	RET_NUM_RET_CODES     /* this should ALWAYS be LAST entry */
} ReturnCode_t, *pReturnCode_t;


/* ============================================================================
* EXPORTED VARIABLES
* =============================================================================
*/


/* ============================================================================
* EXPORTED FUNCTIONS/MACROS
* =============================================================================
*/

/* ----------------------------------------------------------------------------
* MACRO: RD_MEM_8, WR_MEM_8
*
* DESCRIPTION:  32 bit memory access macros
*
* -----------------------------------------------------------------------------
*/
#define RD_MEM_8(addr)	((u8)(*((u8 *)(addr))))
#define WR_MEM_8(addr, data)	(*((u8 *)(addr)) = (u8)(data))

/* ----------------------------------------------------------------------------
* MACRO: RD_MEM_8_VOLATILE, WR_MEM_8_VOLATILE
*
* DESCRIPTION:  8 bit register access macros
*
* -----------------------------------------------------------------------------
*/
#define RD_MEM_8_VOLATILE(addr)	((u8)(*((REG_u8 *)(addr))))
#define WR_MEM_8_VOLATILE(addr, data)	(*((REG_u8 *)(addr)) = (u8)(data))


/* ----------------------------------------------------------------------------
* MACRO: RD_MEM_16, WR_MEM_16
*
* DESCRIPTION:  16 bit memory access macros
*
* -----------------------------------------------------------------------------
*/
#define RD_MEM_16(addr)	((u16)(*((u16 *)(addr))))
#define WR_MEM_16(addr, data)	(*((u16 *)(addr)) = (u16)(data))

/* ----------------------------------------------------------------------------
* MACRO: RD_MEM_16_VOLATILE, WR_MEM_16_VOLATILE
*
* DESCRIPTION:  16 bit register access macros
*
* -----------------------------------------------------------------------------
*/
#define RD_MEM_16_VOLATILE(addr)	((u16)(*((REG_u16 *)(addr))))
#define WR_MEM_16_VOLATILE(addr, data)	(*((REG_u16 *)(addr)) = (u16)(data))


/* ----------------------------------------------------------------------------
* MACRO: RD_MEM_32, WR_MEM_32
*
* DESCRIPTION:  32 bit memory access macros
*
* -----------------------------------------------------------------------------
*/
#define RD_MEM_32(addr)	((u32)(*((u32 *)(addr))))
#define WR_MEM_32(addr, data)	(*((u32 *)(addr)) = (u32)(data))

/* ----------------------------------------------------------------------------
* MACRO: RD_MEM_32_VOLATILE, WR_MEM_32_VOLATILE
*
* DESCRIPTION:  32 bit register access macros
*
* -----------------------------------------------------------------------------
*/
#define RD_MEM_32_VOLATILE(addr)	((u32)(*((REG_u32 *)(addr))))
#define WR_MEM_32_VOLATILE(addr, data)	(*((REG_u32 *)(addr)) = (u32)(data))


/* ----------------------------------------------------------------------------
* Not sure if this all belongs here.
* -----------------------------------------------------------------------------
*/

#ifdef CHECK_RETURN_VALUES

/* ----------------------------------------------------------------------------
* MACRO: CHECK_RETURN_VALUE
*
* DESCRIPTION:  Checks a return code and returns a specified value if code not
*               as expected, also writes spy value if not as expected.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_RETURN_VALUE(actualValue, expectedValue, returnCodeIfMismatch, spyCodeIfMisMatch) \
	if ((expectedValue) != (actualValue)) {\
		RES_Set((spyCodeIfMisMatch));\
		return returnCodeIfMismatch; \
	}

/* ----------------------------------------------------------------------------
* MACRO: CHECK_RETURN_VALUE_RET
*
* DESCRIPTION:  Checks a return code and returns a specified value if code not
*               as expected, no spy value is written.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_RETURN_VALUE_RET(actualValue, expectedValue, returnCodeIfMismatch) \
	if ((expectedValue) != (actualValue)) {\
		return returnCodeIfMismatch; \
	}

/* ----------------------------------------------------------------------------
* MACRO: CHECK_RETURN_VALUE_RES
*
* DESCRIPTION:  Checks a return code and writes spy value if code not as
*               expected.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_RETURN_VALUE_RES(actualValue, expectedValue, spyCodeIfMisMatch) \
	if ((expectedValue) != (actualValue)) {\
		RES_Set((spyCodeIfMisMatch));\
	}

/* ----------------------------------------------------------------------------
* MACRO: CHECK_RETURN_VALUE_RES_RET_VOID
*
* DESCRIPTION:  Checks a return code and writes spy value if code not as
*               expected and return void;
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_RETURN_VALUE_RET_VOID(actualValue, expectedValue, spyCodeIfMisMatch) \
	if ((expectedValue) != (actualValue)) {\
		RES_Set((spyCodeIfMisMatch));\
		return; \
	}

#else

#define CHECK_RETURN_VALUE(actualValue, expectedValue,\
returnCodeIfMismatch, spyCodeIfMisMatch)
#define CHECK_RETURN_VALUE_RET(actualValue,\
expectedValue, returnCodeIfMismatch)
#define CHECK_RETURN_VALUE_RES(actualValue, expectedValue, spyCodeIfMisMatch)
#define CHECK_RETURN_VALUE_RET_VOID(actualValue, expectedValue, spyCodeIfMisMatch)

#endif


#ifdef CHECK_INPUT_PARAMS
/* ----------------------------------------------------------------------------
* MACRO: CHECK_INPUT_PARAMS
*
* DESCRIPTION:  Checks an input code and returns a specified value if code is
*               invalid value, also writes spy value if error found.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_INPUT_PARAM(actualValue, invalidValue,  returnCodeIfMismatch, spyCodeIfMisMatch) \
	if ((invalidValue) == (actualValue)) {\
		RES_Set((spyCodeIfMisMatch));\
		return returnCodeIfMismatch; \
	}

/* ----------------------------------------------------------------------------
* MACRO: CHECK_INPUT_PARAM_NO_SPY
*
* DESCRIPTION:  Checks an input code and returns a specified value if code is
*               invalid value, no spy value written.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_INPUT_PARAM_NO_SPY(actualValue, invalidValue,  returnCodeIfMismatch) \
	if ((invalidValue) == (actualValue)) {\
		return returnCodeIfMismatch; \
	}

/* ----------------------------------------------------------------------------
* MACRO: CHECK_INPUT_RANGE
*
* DESCRIPTION:  Checks an input value and returns a specified value if not in
*               specified range, also writes spy value if error found.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_INPUT_RANGE(actualValue, minValidValue, maxValidValue, returnCodeIfMismatch, spyCodeIfMisMatch) \
	if (((actualValue) < (minValidValue)) || ((actualValue) > (maxValidValue))) {\
		RES_Set((spyCodeIfMisMatch));\
		return returnCodeIfMismatch; \
	}

/* ----------------------------------------------------------------------------
* MACRO: CHECK_INPUT_RANGE_NO_SPY
*
* DESCRIPTION:  Checks an input value and returns a specified value if not in
*               specified range, no spy value written if error found.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_INPUT_RANGE_NO_SPY(actualValue, minValidValue, maxValidValue, returnCodeIfMismatch) \
	if (((actualValue) < (minValidValue)) || ((actualValue) > (maxValidValue))) {\
		return returnCodeIfMismatch; \
	}

/* ----------------------------------------------------------------------------
* MACRO: CHECK_INPUT_RANGE_MIN0
*
* DESCRIPTION:  Checks an input value and returns a specified value if not in
*               specified range, also writes spy value if error found. The minimum
*               value is 0.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_INPUT_RANGE_MIN0(actualValue, maxValidValue, returnCodeIfMismatch, spyCodeIfMisMatch) \
	if ((actualValue) > (maxValidValue)) {\
		RES_Set((spyCodeIfMisMatch));\
		return returnCodeIfMismatch; \
	}

/* ----------------------------------------------------------------------------
* MACRO: CHECK_INPUT_RANGE_NO_SPY_MIN0
*
* DESCRIPTION:  Checks an input value and returns a specified value if not in
*               specified range, no spy value written if error found. The minimum
*               value is 0.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_INPUT_RANGE_NO_SPY_MIN0(actualValue, maxValidValue, returnCodeIfMismatch) \
	if ((actualValue) > (maxValidValue)) {\
		return returnCodeIfMismatch; \
	}

#else

#define CHECK_INPUT_PARAM(actualValue, invalidValue, returnCodeIfMismatch, spyCodeIfMisMatch)
#define CHECK_INPUT_PARAM_NO_SPY(actualValue, invalidValue,  returnCodeIfMismatch)
#define CHECK_INPUT_RANGE(actualValue, minValidValue, maxValidValue, returnCodeIfMismatch, spyCodeIfMisMatch)
#define CHECK_INPUT_RANGE_NO_SPY(actualValue, minValidValue, maxValidValue, returnCodeIfMismatch)
#define CHECK_INPUT_RANGE_MIN0(actualValue, maxValidValue, returnCodeIfMismatch, spyCodeIfMisMatch)
#define CHECK_INPUT_RANGE_NO_SPY_MIN0(actualValue, maxValidValue, returnCodeIfMismatch)

#endif

#ifdef __cplusplus
}
#endif
#endif	/* __GLOBALTYPES_H */
