
/** ============================================================================
 *  @file   print.h
 *
 *  @path   $(DSPLINK)/gpp/src/osal/
 *
 *  @desc   Interface declaration of OS printf abstraction.
 *
 *  @ver    1.51.00.08
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if defined  __cplusplus
extern "C" {
#endif /* defined  __cplusplus */


/*  ----------------------------------- IPC headers */
#include <ipc.h>
#include <_ipc.h>
#include <notifyerr.h>


/** ===========================================================================
 *  @func   PRINT_init
 *
 *  @desc   Initializes the PRINT sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ===========================================================================
 */

signed long int
PRINT_init(void) ;

/** ============================================================================
 *  @deprecated The deprecated function PRINT_Initialize has been replaced
 *              with PRINT_init.
 *  ============================================================================
 */
#define PRINT_Initialize PRINT_init


/** ============================================================================
 *  @func   PRINT_exit
 *
 *  @desc   Releases resources used by this sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */

signed long int
PRINT_exit(void) ;

/** ============================================================================
 *  @deprecated The deprecated function PRINT_Finalize has been replaced
 *              with PRINT_exit.
 *  ============================================================================
 */
#define PRINT_Finalize PRINT_exit


/** ============================================================================
 *  @func   PRINT_Printf
 *
 *  @desc   Provides standard printf functionality abstraction.
 *
 *  @arg    format
 *              Format string.
 *  @arg    ...
 *              Variable argument list.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
#if defined  TRACE_KERNEL

void
PRINT_Printf(char *format, ...) ;
#endif

#if defined  TRACE_USER
/*  ----------------------------------------------------------------------------
 *  Extern declaration for printf to avoid compiler warning.
 *  ----------------------------------------------------------------------------
 */
extern int printf(const char *format, ...) ;

#define PRINT_Printf printf
#endif


#if defined  __cplusplus
}
#endif /* defined  __cplusplus */
