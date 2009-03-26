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


/*
 *  ======== GlobalTypes.h ========
 *  Description:
 *      Global HW definitions
 *
 *! Revision History:
 *! ================
 *! 16 Feb 2003 sb: Initial version
 */


#ifndef __GLOBALTYPES_H
#define __GLOBALTYPES_H

/*
 * Definition: TRUE, FALSE
 *
 * DESCRIPTION:  Boolean Definitions
 */
#ifndef TRUE
#define FALSE	0
#define TRUE	(!(FALSE))
#endif

/*
 * Definition: NULL
 *
 * DESCRIPTION:  Invalid pointer
 */
#ifndef NULL
#define NULL	(void *)0
#endif


#define CHECK_RETURN_VALUE(actualValue, expectedValue,  returnCodeIfMismatch,\
	spyCodeIfMisMatch)
#define CHECK_RETURN_VALUE_RET(actualValue, expectedValue, returnCodeIfMismatch)
#define CHECK_RETURN_VALUE_RES(actualValue, expectedValue, spyCodeIfMisMatch)
#define CHECK_RETURN_VALUE_RET_VOID(actualValue, expectedValue,\
	spyCodeIfMisMatch)

#define CHECK_INPUT_PARAM(actualValue, invalidValue, returnCodeIfMismatch,\
	spyCodeIfMisMatch)
#define CHECK_INPUT_PARAM_NO_SPY(actualValue, invalidValue,\
	returnCodeIfMismatch)
#define CHECK_INPUT_RANGE(actualValue, minValidValue, maxValidValue,\
	returnCodeIfMismatch, spyCodeIfMisMatch)
#define CHECK_INPUT_RANGE_NO_SPY(actualValue, minValidValue, maxValidValue,\
	returnCodeIfMismatch)
#define CHECK_INPUT_RANGE_MIN0(actualValue, maxValidValue,\
	returnCodeIfMismatch, spyCodeIfMisMatch)
#define CHECK_INPUT_RANGE_NO_SPY_MIN0(actualValue, maxValidValue,\
	returnCodeIfMismatch)

#endif	/* __GLOBALTYPES_H */
