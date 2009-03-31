/** ============================================================================
 *  @file   print.c
 *
 *  @path   $(DSPLINK)/gpp/src/osal/Linux/
 *
 *  @desc   Implementation of OS printf abstraction.
 *
 *  @ver    1.51.00.08
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers         */
#include <linux/autoconf.h>
#include <linux/spinlock.h>
#include <stdarg.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#include <asm/div64.h>

/*  ----------------------------------- DSP/BIOS Link               */
#include <ipc.h>
#include <_ipc.h>
#include <gpptypes.h>
#include <errbase.h>
/*  ----------------------------------- Debug                       */
#include <_trace.h>

/*  ----------------------------------- OSAL Headers                */
#include <osal.h>



/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component ID of this module.
 *  ============================================================================
 */
#define  COMPONENT_ID        ID_OSAL_PRINT

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#if defined DDSP_DEBUG
#define SET_FAILURE_REASON   TRC_SetReason(status, FID_C_OSAL_PRINT, __LINE__)
#else
#define SET_FAILURE_REASON
#endif /* if defined DDSP_DEBUG */


/** ============================================================================
 *  @name   PRINT_IsInitialized
 *
 *  @desc   Flag to track initialization of the sub-component.
 *  ============================================================================
 */
static unsigned long int  PRINT_IsInitialized  = FALSE   ;


/** ============================================================================
 *  @func   PRINT_init
 *
 *  @desc   Initializes the PRINT sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
PRINT_init(void)
{
    signed long int status = DSP_SOK ;

    PRINT_IsInitialized = TRUE ;

    return status ;
}


/** ============================================================================
 *  @func   PRINT_exit
 *
 *  @desc   Releases resources used by this sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
PRINT_exit(void)
{
    signed long int status = DSP_SOK ;

    PRINT_IsInitialized = FALSE ;

    return status ;
}


/** ============================================================================
 *  @func   PRINT_Printf
 *
 *  @desc   Provides standard printf functionality abstraction.
 *
 *  @modif  None
 *  ============================================================================
 */
/*
static void
PRINT_Printf(char *format, ...)
{
    unsigned long int  bufCount     = 0 ;
    va_list args             ;
    char    buffer[512]    ;

    buffer[0] = '<';
    buffer[1] = '1';
    buffer[2] = '>';
    bufCount  =  3 ;

    va_start(args, format) ;
    vsprintf(buffer + bufCount, format, args);
    va_end(args) ;

    printk(buffer) ;
}
*/
