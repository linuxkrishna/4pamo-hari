/** ============================================================================
 *  @file   ipc.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Defines data types and structures used by IPC modules.
 *
 *  @ver    1.51.00.08
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined IPC_H
#define IPC_H


/*  ----------------------------------- DSP/BIOS Link                   */
#include <gpptypes.h>
#include <ipctypes.h>
#include <errbase.h>
#include <constants.h>
#include <shmdefs.h>


#if defined __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  WAIT_FOREVER
 *
 *  @desc   Wait indefinitely.
 *  ============================================================================
 */
#define WAIT_FOREVER           (~((unsigned long int) 0u))

/** ============================================================================
 *  @const  WAIT_NONE
 *
 *  @desc   Do not wait.
 *  ============================================================================
 */
#define WAIT_NONE              ((unsigned long int) 0u)


/** ============================================================================
 *  @macro  IS_GPPID
 *
 *  @desc   Is the GPP ID valid.
 *  ============================================================================
 */
#define IS_GPPID(id)        (id == ID_GPP)


#if defined __cplusplus
}
#endif


#endif /* !defined (IPC_H) */
