/*
 *  ======== hal_defs.h ========
 *  Description:
 *      Global HAL definitions
 *
 *! Revision History:
 *! ================
 *! 19 Apr 2004 sb: Added generic page size, endianness and element size defns
 *! 16 Feb 2003 sb: Initial version
 */
#ifndef __HAL_DEFS_H
#define __HAL_DEFS_H

/* ============================================================================
* INCLUDE FILES (only if necessary)
* =============================================================================
*/

#include <GlobalTypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* ============================================================================
* EXPORTED DEFINITIONS
* =============================================================================
*/

/* Page size */
#define HAL_PAGE_SIZE_4KB   0x1000
#define HAL_PAGE_SIZE_8KB   0x2000
#define HAL_PAGE_SIZE_64KB  0x10000
#define HAL_PAGE_SIZE_1MB   0x100000
#define HAL_PAGE_SIZE_16MB  0x1000000

/* ============================================================================
* EXPORTED TYPES
* =============================================================================
*/
/* ----------------------------------------------------------------------------
* TYPE:         HAL_STATUS
*
* DESCRIPTION:  return type for HAL API
*
* -----------------------------------------------------------------------------
*/

/* ----------------------------------------------------------------------------
* TYPE:         HAL_SetClear_t
*
* DESCRIPTION:  Enumerated Type used to set and clear any bit
*
* -----------------------------------------------------------------------------
*/
enum HAL_SetClear_t
{
    HAL_CLEAR,
    HAL_SET
};

/* ----------------------------------------------------------------------------
* TYPE:         HAL_Endianism_t
*
* DESCRIPTION:  Enumerated Type used to specify the endianism
*               Do NOT change these values. They are used as bit fields.
*
* -----------------------------------------------------------------------------
*/
enum HAL_Endianism_t
{
    HAL_LITTLE_ENDIAN,
    HAL_BIG_ENDIAN
};

/* ----------------------------------------------------------------------------
* TYPE:         HAL_ElementSize_t
*
* DESCRIPTION:  Enumerated Type used to specify the element size
*               Do NOT change these values. They are used as bit fields.
*
* -----------------------------------------------------------------------------
*/
enum HAL_ElementSize_t
{
    HAL_ELEM_SIZE_8BIT,
    HAL_ELEM_SIZE_16BIT,
    HAL_ELEM_SIZE_32BIT,
    HAL_ELEM_SIZE_64BIT
};


/* ============================================================================
* EXPORTED VARIABLES
* =============================================================================
*/

/* ============================================================================
* EXPORTED FUNCTIONS
* =============================================================================
*/

#ifdef __cplusplus
}
#endif
#endif  /* __HAL_DEFS_H */

