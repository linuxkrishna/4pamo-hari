//  ===========================================================================
//  File    hal_ocp.h
//
//  Path    $ (PROJROOT)\driver\mailbox
//
//  Desc    API declarations for generic OCP Socket system registers for Mailbox
//
//  Rev     0.1.0
//
//  This computer program is copyright to Texas Instruments Incorporated.
//  The program may not be used without the written permission of
//  Texas Instruments Incorporated or against the terms and conditions
//  stipulated in the agreement under which this program has been supplied.
//
//  (c) Texas Instruments Incorporated 2008
//
//  ===========================================================================

#ifndef __HAL_OCP_H
#define __HAL_OCP_H

/* ============================================================================
* INCLUDE FILES (only if necessary)
* =============================================================================
*/

#ifdef __cplusplus
extern "C"
{

#endif

#include <GlobalTypes.h>
#include <hal_ocp.h>
#include <hal_defs.h>
#include <MBXRegAcM.h>
#include <MBXAccInt.h>


/* ============================================================================
* EXPORTED DEFINITIONS
* =============================================================================
*/


/* ============================================================================
* EXPORTED TYPES
* =============================================================================
*/

/* ----------------------------------------------------------------------------
* TYPE:         HAL_IdleMode_t
*
* DESCRIPTION:  Enumerated Type for idle modes in OCP SYSCONFIG register
*
* -----------------------------------------------------------------------------
*/
enum HAL_OCPIdleMode_t
{
    HAL_OCP_FORCE_IDLE,
    HAL_OCP_NO_IDLE,
    HAL_OCP_SMART_IDLE
};

/* ============================================================================
* EXPORTED VARIABLES
* =============================================================================
*/


/* ============================================================================
* EXPORTED FUNCTIONS
* =============================================================================
*/

extern long HAL_OCP_SoftReset (const unsigned long baseAddress);

extern long HAL_OCP_SoftResetIsDone (const unsigned long baseAddress,
                                           unsigned long *resetIsDone);

extern long HAL_OCP_IdleModeSet (const unsigned long baseAddress,
                                       enum HAL_OCPIdleMode_t idleMode);

extern long HAL_OCP_IdleModeGet (const unsigned long baseAddress,
                                       enum HAL_OCPIdleMode_t *idleMode);

extern long HAL_OCP_AutoIdleSet (const unsigned long baseAddress,
                                       enum HAL_SetClear_t autoIdle);

extern long HAL_OCP_AutoIdleGet (const unsigned long baseAddress,
                                       enum HAL_SetClear_t *autoIdle);

#ifdef __cplusplus
}
#endif

#endif  /* __HAL_OCP_H */

