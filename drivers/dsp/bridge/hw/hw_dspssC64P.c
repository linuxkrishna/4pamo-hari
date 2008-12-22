/*
 * hw_dspssC64P.c
 *
 * DSP-BIOS Bridge driver support functions for TI OMAP processors.
 *
 * Copyright (C) 2005-2006 Texas Instruments, Inc.
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
 *  ======== hw_dspss64P.c ========
 *  Description:
 *      API definitions to configure DSP Subsystem modules like IPI
 *
 *! Revision History:
 *! ================
 *! 19 Apr 2004 sb: Implemented HW_DSPSS_IPIEndianismSet
 *! 16 Feb 2003 sb: Initial version
 */


/* ============================================================================
* STANDARD INCLUDE FILES
* =============================================================================
*/

/* ============================================================================
* PROJECT SPECIFIC INCLUDE FILES
* =============================================================================
*/
#include <GlobalTypes.h>
#include <hw_defs.h>
#include <hw_dspssC64P.h>

/* ============================================================================
* GLOBAL VARIABLES DECLARATIONS
* =============================================================================
*/

/* ============================================================================
* LOCAL TYPES AND DEFINITIONS
* =============================================================================
*/

/* ============================================================================
* LOCAL VARIABLES DECLARATIONS
* =============================================================================
*/

/* ============================================================================
* LOCAL FUNCTIONS PROTOTYPES
* =============================================================================
*/

/* ============================================================================
* HW FUNCTIONS
* =============================================================================
*/
HW_STATUS HW_DSPSS_BootModeSet (const u32 baseAddress, enum  HW_DSPBootMode_t bootMode)
{
	HW_STATUS status = RET_OK;
	/* TO DO: Use Macro instead of hard-coded value*/
	*((REG_u32 *)((u32)baseAddress+0x404)) = bootMode;

	return status;
}
