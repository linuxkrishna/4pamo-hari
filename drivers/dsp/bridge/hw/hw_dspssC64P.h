/*
 * hw_dspssC64P.h
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
*  ======== hw_dspss.h ========
*Description:
*      DSP Subsystem API declarations
*
*! Revision History:
*! ================
*! 19-Apr-2004 sb: Removed redundant argument from HW_DSPSS_IPIEndianismSet
*Moved endianness and element size to generic hw_defs.h
*! 16 Feb 2003 sb: Initial version
*/

#ifndef __HW_DSPSS_H
#define __HW_DSPSS_H

/* ============================================================================
* INCLUDE FILES (only if necessary)
* =============================================================================
*/
#include <linux/types.h>


/* ----------------------------------------------------------------------------
* TYPE:         HW_DSPIPI_DspBootMode_t
*
* DESCRIPTION:  Enumerated Type used to specify the DSP boot mode
*
* -----------------------------------------------------------------------------
*/
enum HW_DSPBootMode_t {
	HW_DSP_DIRECT_BOOT = 0x0,
	HW_DSP_IDLE_BOOT = 0x1,
	HW_DSP_SELF_LOOP = 0x2,
	HW_DSP_USER_BOOT = 0x3,
	HW_DSP_CONTEXT_RESTORE = 0x4,
};
/* ============================================================================
* EXPORTED VARIABLES
* =============================================================================
*/


/* ============================================================================
* EXPORTED FUNCTIONS
* =============================================================================
*/
extern HW_STATUS HW_DSPSS_BootModeSet(const u32 baseAddress,
	enum  HW_DSPBootMode_t bootMode);
#endif  /* __HW_DSPSS_H */
