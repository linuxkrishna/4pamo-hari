/*
 * hw_defs.h
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
 *! Revision History:
 *! ================
 *! 19 Apr 2004 sb: Added generic page size, endianness and element size defns
 *! 16 Feb 2003 sb: Initial version
 */

#ifndef __HW_DEFS_H
#define __HW_DEFS_H

/* ============================================================================
* INCLUDE FILES (only if necessary)
* =============================================================================
*/

#include <GlobalTypes.h>


/* ============================================================================
* EXPORTED DEFINITIONS
* =============================================================================
*/

/* Page size */
#define HW_PAGE_SIZE_4KB   0x1000
#define HW_PAGE_SIZE_8KB   0x2000
#define HW_PAGE_SIZE_64KB  0x10000
#define HW_PAGE_SIZE_1MB   0x100000
#define HW_PAGE_SIZE_16MB  0x1000000

/* ============================================================================
* EXPORTED TYPES
* =============================================================================
*/
/* ----------------------------------------------------------------------------
* TYPE:         HW_STATUS
*
* DESCRIPTION:  return type for HW API
*
* -----------------------------------------------------------------------------
*/
typedef long HW_STATUS;

/* ----------------------------------------------------------------------------
* TYPE:         HW_SetClear_t
*
* DESCRIPTION:  Enumerated Type used to set and clear any bit
*
* -----------------------------------------------------------------------------
*/
enum HW_SetClear_t {
    HW_CLEAR,
    HW_SET
};

/* ----------------------------------------------------------------------------
* TYPE:         HW_Endianism_t
*
* DESCRIPTION:  Enumerated Type used to specify the endianism
*               Do NOT change these values. They are used as bit fields.
*
* -----------------------------------------------------------------------------
*/
enum HW_Endianism_t {
    HW_LITTLE_ENDIAN,
    HW_BIG_ENDIAN

};

/* ----------------------------------------------------------------------------
* TYPE:         HW_ElementSize_t
*
* DESCRIPTION:  Enumerated Type used to specify the element size
*               Do NOT change these values. They are used as bit fields.
*
* -----------------------------------------------------------------------------
*/
enum HW_ElementSize_t {
    HW_ELEM_SIZE_8BIT,
    HW_ELEM_SIZE_16BIT,
    HW_ELEM_SIZE_32BIT,
    HW_ELEM_SIZE_64BIT

};


#endif  /* __HW_DEFS_H */
