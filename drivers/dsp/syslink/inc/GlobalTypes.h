
/*
*  ======== GlobalTypes.h ========
*  Description:
*      Global HAL definitions
*
*! Revision History:
*! ================
*! 16 Feb 2003 sb: Initial version
*/
#ifndef __GLOBALTYPES_H
#define __GLOBALTYPES_H

/* ==================================================================
* INCLUDE FILES (only if necessary)
* ====================================================================
*/
#ifdef __cplusplus
extern "C"
{
#endif

/* =====================================================================
* EXPORTED DEFINITIONS
* =======================================================================
*/

/* ----------------------------------------------------------------------
* Definition: TRUE, FALSE
*
* DESCRIPTION:  Boolean Definitions
*
* -----------------------------------------------------------------------
*/
#ifndef TRUE
#define FALSE           0
#define TRUE       (!(FALSE))
#endif

/* -----------------------------------------------------------------------
* Definition: NULL
*
* DESCRIPTION:  Invalid pointer
*
* -------------------------------------------------------------------------
*/
#ifndef NULL
#define NULL        (void *)0
#endif

/* -----------------------------------------------------------------------
* Definition: RET_CODE_BASE
*
* DESCRIPTION:  Base value for return code offsets
*
* -------------------------------------------------------------------------
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

/* -----------------------------------------------------------------------
* Definitin: *BIT_SHIFT
*
* DESCRIPTION:  offset in bits from start of 32-bit word.
*
* -------------------------------------------------------------------------
*/
#define LOWER_16BIT_SHIFT          0
#define UPPER_16BIT_SHIFT          16

#define LOWER_8BIT_SHIFT           0
#define LOWER_MIDDLE_8BIT_SHIFT    8
#define UPPER_MIDDLE_8BIT_SHIFT    16
#define UPPER_8BIT_SHIFT           24

#define LOWER_8BIT_OF16_SHIFT      0
#define UPPER_8BIT_OF16_SHIFT      8


/* ------------------------------------------------------------------------
* Definition: LOWER_16BIT_MASK
*
* DESCRIPTION:
*16 bit mask used for inclusion of lower 16 bits i.e. mask out
*the upper 16 bits
*
* ------------------------------------------------------------------------
*/
#define LOWER_16BIT_MASK        0x0000FFFF


/* -----------------------------------------------------------------------
* Definition: LOWER_8BIT_MASK
*
* DESCRIPTION:  8 bit masks used for inclusion of 8 bits i.e. mask out
*               the upper 16 bits
*
* -------------------------------------------------------------------------
*/
#define LOWER_8BIT_MASK           0x000000FF

/* ------------------------------------------------------------------------
* Definition:
*RETURN_32BITS_FROM_16LOWER_AND_16UPPER(lower16Bits, upper16Bits)
*
* DESCRIPTION:
*Returns a 32 bit value given a 16 bit lower value and a 16
* bit upper value
*
* -------------------------------------------------------------------------
*/
#define RETURN_32BITS_FROM_16LOWER_AND_16UPPER(lower16Bits,upper16Bits)\
(((((unsigned long)lower16Bits)  & LOWER_16BIT_MASK)) | \
(((((unsigned long)upper16Bits) & LOWER_16BIT_MASK) << UPPER_16BIT_SHIFT)))


/* ------------------------------------------------------------------------
* Definition: RETURN_16BITS_FROM_8LOWER_AND_8UPPER(lower16Bits, upper16Bits)
*
* DESCRIPTION:  Returns a 16 bit value given a 8 bit lower value and a 8
*               bit upper value
*
* --------------------------------------------------------------------------
*/
#define RETURN_16BITS_FROM_8LOWER_AND_8UPPER(lower8Bits,upper8Bits)\
(((((unsigned long)lower8Bits)  & LOWER_8BIT_MASK)) | \
(((((unsigned long)upper8Bits) & LOWER_8BIT_MASK) \
<< UPPER_8BIT_OF16_SHIFT)))

/* ------------------------------------------------------------------------
* Definition: RETURN_32BITS_FROM_4_8BIT_VALUES(
*                                       lower8Bits,
lowerMiddle8Bits,
lowerUpper8Bits,
upper8Bits)
*
* DESCRIPTION:  Returns a 32 bit value given four 8 bit values
*
* ---------------------------------------------------------------------
*/
#define RETURN_32BITS_FROM_4_8BIT_VALUES(lower8Bits, lowerMiddle8Bits, \
lowerUpper8Bits, upper8Bits) do {\
	(((((unsigned long)lower8Bits) & LOWER_8BIT_MASK)) | \
	(((((unsigned long)lowerMiddle8Bits) & LOWER_8BIT_MASK) \
	<< LOWER_MIDDLE_8BIT_SHIFT)) | \
	(((((unsigned long)lowerUpper8Bits) & LOWER_8BIT_MASK) \
	<< UPPER_MIDDLE_8BIT_SHIFT)) | \
	(((((unsigned long)upper8Bits)  & LOWER_8BIT_MASK) \
	<< UPPER_8BIT_SHIFT))) \
} while (0)


/* -----------------------------------------------------------------------
* Definition: READ_LOWER_16BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 16 lower bits of 32bit value
*
* -------------------------------------------------------------------------
*/
#define READ_LOWER_16BITS_OF_32(value32bits)\
((unsigned short)((unsigned long)(value32bits) & LOWER_16BIT_MASK))

/* -----------------------------------------------------------------
* Definition: READ_UPPER_16BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 16 lower bits of 32bit value
*
* -------------------------------------------------------------------
*/
#define READ_UPPER_16BITS_OF_32(value32bits)\
(((unsigned short)((unsigned long)(value32bits) >> UPPER_16BIT_SHIFT)) \
& LOWER_16BIT_MASK)


/* -----------------------------------------------------------------
* Definition: READ_LOWER_8BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 8 lower bits of 32bit value
*
* --------------------------------------------------------------------
*/
#define READ_LOWER_8BITS_OF_32(value32bits)\
((unsigned char)((unsigned long)(value32bits) & LOWER_8BIT_MASK))

/* ---------------------------------------------------------------------
* Definition: READ_LOWER_MIDDLE_8BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 8 lower middle bits of 32bit value
*
* -----------------------------------------------------------------------
*/
#define READ_LOWER_MIDDLE_8BITS_OF_32(value32bits)\
(((unsigned char)((unsigned long)(value32bits) >> LOWER_MIDDLE_8BIT_SHIFT)) \
& LOWER_8BIT_MASK)

/* --------------------------------------------------------------------
* Definition: READ_LOWER_MIDDLE_8BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 8 lower middle bits of 32bit value
*
* ----------------------------------------------------------------------
*/
#define READ_UPPER_MIDDLE_8BITS_OF_32(value32bits)\
(((unsigned char)((unsigned long)(value32bits) >> LOWER_MIDDLE_8BIT_SHIFT)) \
& LOWER_8BIT_MASK)

/* ---------------------------------------------------------------------
* Definition: READ_UPPER_8BITS_OF_32(value32bits)
*
* DESCRIPTION:  Returns a 8 upper bits of 32bit value
*
* ----------------------------------------------------------------------
*/
#define READ_UPPER_8BITS_OF_32(value32bits)\
(((unsigned char)((unsigned long)(value32bits) >> UPPER_8BIT_SHIFT)) \
& LOWER_8BIT_MASK)


/* ---------------------------------------------------------------------
* Definition: READ_LOWER_8BITS_OF_16(value16bits)
*
* DESCRIPTION:  Returns a 8 lower bits of 16bit value
*
* ------------------------------------------------------------------------
*/
#define READ_LOWER_8BITS_OF_16(value16bits)\
((unsigned char)((unsigned short)(value16bits) & LOWER_8BIT_MASK))

/* ------------------------------------------------------------------------
* Definition: READ_UPPER_8BITS_OF_16(value32bits)
*
* DESCRIPTION:  Returns a 8 upper bits of 16bit value
*
* ---------------------------------------------------------------------------
*/
#define READ_UPPER_8BITS_OF_16(value16bits)\
(((unsigned char)((unsigned long)(value16bits) >> UPPER_8BIT_SHIFT)) \
& LOWER_8BIT_MASK)

/* =======================================================================
* EXPORTED TYPES
* =========================================================================
*/

/* -------------------------------------------------------------------------
* TYPE: unsigned char, UCHAR, WORD8
*
* DESCRIPTION:  8 bit tpyes
*
* ------------------------------------------------------------------------
*/

/* ------------------------------------------------------------------------
* TYPE: unsigned short, WORD16
*
* DESCRIPTION:  16 bit tpyes
*
* -------------------------------------------------------------------------
*/

/* ---------------------------------------------------------------
* TYPE: unsigned long, WORD32
*
* DESCRIPTION:  32 bit tpyes
*
* ----------------------------------------------------------------
*/

/* ---------------------------------------------------------------
* TYPE: VOL unsigned char, REG_WORD8
*
* DESCRIPTION: 8 bit register types.
*
* ------------------------------------------------------------------
*/

/* -----------------------------------------------------------------
* TYPE: VOL unsigned short, REG_WORD16
*
* DESCRIPTION: 16 bit register types.
*
* --------------------------------------------------------------------
*/

/* ------------------------------------------------------------------
* TYPE: VOL unsigned long, REG_WORD32
*
* DESCRIPTION: 32 bit register types.
*
* ---------------------------------------------------------------------
*/

/* --------------------------------------------------------------------
* TYPE: FLOAT
*
* DESCRIPTION: Type to be used for floating point calculation. Note that
* floating point calculation is very CPU expensive, and you should
* only  use if you absolutely need this.
*
* -----------------------------------------------------------------
*/

/* -----------------------------------------------------------------
* TYPE: enum boolean_label
*
* DESCRIPTION:  Boolean Type True, False
*
* ------------------------------------------------------------------
*/
#include <global_var.h>

#ifndef OMAPBRIDGE_TYPES
enum boolean_label {
False = 0,
True = 1
};

/* Boolean  Definition */
#define BOOL           enum boolean_label
#endif

/* ------------------------------------------------------------------
* TYPE: ReturnCode_t
*
* DESCRIPTION:  Return codes to be returned by all library functions
*
* ---------------------------------------------------------------------
*/
enum ReturnCode_label {
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
};


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
#define RD_MEM_8(addr)	((unsigned char)(*((unsigned char *)(addr))))
#define WR_MEM_8(addr, data)	(*((unsigned char *)(addr)) \
= (unsigned char)(data))

/* ----------------------------------------------------------------------------
* MACRO: RD_MEM_8_VOLATILE, WR_MEM_8_VOLATILE
*
* DESCRIPTION:  8 bit register access macros
*
* -----------------------------------------------------------------------
*/
#define RD_MEM_8_VOLATILE(addr)	\
((unsigned char)(*((REG unsigned char *)(addr))))

#define WR_MEM_8_VOLATILE(addr, data)	\
(*((REG unsigned char *)(addr)) = (unsigned char)(data))


/* ----------------------------------------------------------------------
* MACRO: RD_MEM_16, WR_MEM_16
*
* DESCRIPTION:  16 bit memory access macros
*
* -----------------------------------------------------------------------
*/
#define RD_MEM_16(addr)	\
((unsigned short)(*((unsigned short *)(addr))))

#define WR_MEM_16(addr, data) \
(*((unsigned short *)(addr)) = (unsigned short)(data))

/* ----------------------------------------------------------------------
* MACRO: RD_MEM_16_VOLATILE, WR_MEM_16_VOLATILE
*
* DESCRIPTION:  16 bit register access macros
*
* ------------------------------------------------------------------------
*/
#define RD_MEM_16_VOLATILE(addr) \
((unsigned short)(*((REG unsigned short *)(addr))))

#define WR_MEM_16_VOLATILE(addr, data)	\
(*((REG unsigned short *)(addr)) = (unsigned short)(data))


/* ------------------------------------------------------------------------
* MACRO: RD_MEM_32, WR_MEM_32
*
* DESCRIPTION:  32 bit memory access macros
*
* -------------------------------------------------------------------------
*/
#define RD_MEM_32(addr)	\
((unsigned long)(*((unsigned long *)(addr))))

#define WR_MEM_32(addr, data) \
(*((unsigned long *)(addr)) = (unsigned long)(data))

/* ----------------------------------------------------------------------------
* MACRO: RD_MEM_32_VOLATILE, WR_MEM_32_VOLATILE
*
* DESCRIPTION:  32 bit register access macros
*
* -----------------------------------------------------------------------------
*/
#define RD_MEM_32_VOLATILE(addr) \
((unsigned long)(*((REG unsigned long *)(addr))))

#define WR_MEM_32_VOLATILE(addr, data) \
(*((REG unsigned long *)(addr)) = (unsigned long)(data))


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
#define CHECK_RETURN_VALUE(actualValue, expectedValue, \
returnCodeIfMismatch, spyCodeIfMisMatch) do {\
	if ((expectedValue) != (actualValue)) {\
		RES_Set((spyCodeIfMisMatch));\
		return returnCodeIfMismatch; \
	} \
} while (0)

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
#define CHECK_RETURN_VALUE_RET(actualValue, \
expectedValue, returnCodeIfMismatch) do {\
	if ((expectedValue) != (actualValue)) {\
		return returnCodeIfMismatch; \
	} \
} while (0)

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
do { \
	if ((expectedValue) != (actualValue)) {\
		RES_Set((spyCodeIfMisMatch));\
	} \
} while (0)


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
#define CHECK_RETURN_VALUE_RET_VOID(actualValue, \
expectedValue, spyCodeIfMisMatch) do {\
	if ((expectedValue) != (actualValue)) {\
		RES_Set((spyCodeIfMisMatch));\
		return; \
	} \
} while (0)

#else

#define CHECK_RETURN_VALUE(actualValue, expectedValue, returnCodeIfMismatch,\
spyCodeIfMisMatch)

#define CHECK_RETURN_VALUE_RET(actualValue, expectedValue, \
returnCodeIfMismatch)

#define CHECK_RETURN_VALUE_RES(actualValue, expectedValue, \
spyCodeIfMisMatch)

#define CHECK_RETURN_VALUE_RET_VOID(actualValue, expectedValue, \
spyCodeIfMisMatch)

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
#define CHECK_INPUT_PARAM(actualValue, invalidValue,  \
returnCodeIfMismatch, spyCodeIfMisMatch) do {\
	if ((invalidValue) == (actualValue)) {\
		RES_Set((spyCodeIfMisMatch));\
		return returnCodeIfMismatch; \
	} \
} while (0)

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
#define CHECK_INPUT_PARAM_NO_SPY(actualValue, invalidValue,\
returnCodeIfMismatch) do {\
	if ((invalidValue) == (actualValue)) {\
		return returnCodeIfMismatch; \
	} \
} while (0)
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
#define CHECK_INPUT_RANGE(actualValue, minValidValue, maxValidValue, \
returnCodeIfMismatch, spyCodeIfMisMatch) do {\
	if (((actualValue) < (minValidValue)) || \
		((actualValue) > (maxValidValue))) {\
		RES_Set((spyCodeIfMisMatch));\
		return returnCodeIfMismatch; \
	} \
} while (0)
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
#define CHECK_INPUT_RANGE_NO_SPY(actualValue, minValidValue, \
maxValidValue, returnCodeIfMismatch) do {\
	if (((actualValue) < (minValidValue)) || \
		((actualValue) > (maxValidValue))) {\
		return returnCodeIfMismatch; \
	} \
} while (0)

/* ----------------------------------------------------------------------------
* MACRO: CHECK_INPUT_RANGE_MIN0
*
* DESCRIPTION:  Checks an input value and returns a
*specified value if not in
* specified range, also writes spy value if error found.
*The minimum
* value is 0.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_INPUT_RANGE_MIN0(actualValue, maxValidValue, \
returnCodeIfMismatch, spyCodeIfMisMatch) do {\
	if ((actualValue) > (maxValidValue)) {\
		RES_Set((spyCodeIfMisMatch));\
		return returnCodeIfMismatch; \
	} \
} while (0)

/* ----------------------------------------------------------------------------
* MACRO: CHECK_INPUT_RANGE_NO_SPY_MIN0
*
* DESCRIPTION:  Checks an input value and
*returns a specified value if not in
* specified range, no spy value written
*if error found. The minimum
* value is 0.
*
* NOTE:         Can be disabled to save HW cycles.
*
* -----------------------------------------------------------------------------
*/
#define CHECK_INPUT_RANGE_NO_SPY_MIN0(actualValue, \
maxValidValue, returnCodeIfMismatch) do {\
	if ((actualValue) > (maxValidValue)) {\
		return returnCodeIfMismatch; \
	} \
} while (0)

#else

#define CHECK_INPUT_PARAM(actualValue, invalidValue, returnCodeIfMismatch,\
spyCodeIfMisMatch)

#define CHECK_INPUT_PARAM_NO_SPY(actualValue, invalidValue, \
returnCodeIfMismatch)

#define CHECK_INPUT_RANGE(actualValue, minValidValue, maxValidValue, \
returnCodeIfMismatch, spyCodeIfMisMatch)

#define CHECK_INPUT_RANGE_NO_SPY(actualValue, minValidValue , \
maxValidValue, returnCodeIfMismatch)

#define CHECK_INPUT_RANGE_MIN0(actualValue, maxValidValue, \
returnCodeIfMismatch, spyCodeIfMisMatch)

#define CHECK_INPUT_RANGE_NO_SPY_MIN0(actualValue, \
maxValidValue, returnCodeIfMismatch)

#endif

#ifdef __cplusplus
}
#endif
#endif	/* __GLOBALTYPES_H */
