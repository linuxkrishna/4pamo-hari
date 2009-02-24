
/** ============================================================================
 *  @file   osal.h
 *
 *  @path   $(DSPLINK)/gpp/src/osal/
 *
 *  @desc   Defines the interfaces for initializing and finalizing OSAL.
 *
 *  @ver    1.51.00.08
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (OSAL_H)
#define OSAL_H


/*  ----------------------------------- IPC headers */
#include <ipc.h>
#include <_ipc.h>


/*  ----------------------------------- OSAL Headers                */
#include <mem_os.h>
#include <mem.h>
#include <dpc.h>
#include <sync.h>
#include <errbase.h>


#if defined (__cplusplus)
extern "C" {
#endif


/** ============================================================================
 *  @func   OSAL_init
 *
 *  @desc   Initializes the OS Adaptation layer.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory error.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    OSAL_exit
 *  ============================================================================
 */

signed long int
OSAL_init (void) ;

/** ============================================================================
 *  @deprecated The deprecated function OSAL_Initialize has been replaced
 *              with OSAL_init.
 *  ============================================================================
 */
#define OSAL_Initialize OSAL_init


/** ============================================================================
 *  @func   OSAL_exit
 *
 *  @desc   Releases OS adaptation layer resources indicating that they would
 *          no longer be used.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory error.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  Subcomponent must be initialized.
 *
 *  @leave  None
 *
 *  @see    OSAL_init
 *  ============================================================================
 */

signed long int
OSAL_exit (void) ;

/** ============================================================================
 *  @deprecated The deprecated function OSAL_Finalize has been replaced
 *              with OSAL_exit.
 *  ============================================================================
 */
#define OSAL_Finalize OSAL_exit


#if defined (__cplusplus)
}
#endif


#endif /* !defined (OSAL_H) */
