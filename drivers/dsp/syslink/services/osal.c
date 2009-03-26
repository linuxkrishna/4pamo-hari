/** ============================================================================
*  @file   osal.c
*
*  @path   $(DSPLINK)/gpp/src/osal/
*
*  @desc   Implementation of OS Adaptation layer initialization and
*          finalization functions.
*
*  @ver    1.51.00.08
*  ============================================================================
*  Copyright (c) Texas Instruments Incorporated 2002-2008
*
*  Use of this software is controlled by the terms and conditions found in the
*  license agreement under which this software has been supplied or provided.
*  ============================================================================
*/


/*  ----------------------------------- DSP/BIOS Link               */
#include <ipc.h>
#include <_ipc.h>

/*  ----------------------------------- Trace & Debug               */
#include <_trace.h>
#include <print.h>
#include <gpptypes.h>
#include <errbase.h>


/*  ----------------------------------- OSAL Headers                */
#include <osal.h>



/** ============================================================================
*  @macro  COMPONENT_ID
*
*  @desc   Component and Subcomponent Identifier.
*  ============================================================================
*/
#define  COMPONENT_ID       ID_OSAL

/** ============================================================================
*  @macro  SET_FAILURE_REASON
*
*  @desc   Sets failure reason.
*  ============================================================================
*/
#if defined DDSP_DEBUG
#define SET_FAILURE_REASON(status)  TRC_SetReason(status, FID_C_OSAL, __LINE__)
#else
#define SET_FAILURE_REASON(status)
#endif /* if defined (DDSP_DEBUG) */


/** ============================================================================
*  @name   OSAL_initCount
*
*  @desc   Number of times OSAL has been initialized.
*  ============================================================================
*/
static unsigned long int  OSAL_initCount ;


/** ============================================================================
*  @func   OSAL_init
*
*  @desc   Initializes the OS Adaptation layer.
*
*  @modif  None
*  ============================================================================
*/

signed long int
OSAL_init(void)
{
	signed long int status = DSP_SOK ;

	TRC_0ENTER("OSAL_init") ;

	if (OSAL_initCount == 0) {
		status = MEM_Init() ;

		if (DSP_SUCCEEDED(status))
			status = SYNC_Init() ;
		 else
			SET_FAILURE_REASON(status) ;



		if (DSP_SUCCEEDED(status))
			status = DPC_init() ;
		 else
			SET_FAILURE_REASON(status) ;



		if (DSP_SUCCEEDED(status))
			status = PRINT_init() ;
		 else
			SET_FAILURE_REASON(status) ;


		if (DSP_SUCCEEDED(status))
			OSAL_initCount++ ;
		 else
			SET_FAILURE_REASON(status) ;

	} else
		OSAL_initCount++ ;


	TRC_1LEAVE("OSAL_init", status) ;

	return status ;
}


/** ============================================================================
*  @func   OSAL_exit
*
*  @desc   Releases OS adaptation layer resources indicating that they would
*          no longer be used.
*
*  @modif  None
*  ============================================================================
*/

signed long int
OSAL_exit(void)
{
	signed long int status    = DSP_SOK ;
	signed long int tmpStatus = DSP_SOK ;

	TRC_0ENTER("OSAL_exit") ;

	if (OSAL_initCount == 1) {

		status = PRINT_exit();

		if (DSP_FAILED(status))
			SET_FAILURE_REASON(status) ;


		tmpStatus = DPC_exit() ;

		if (DSP_FAILED(tmpStatus) && DSP_SUCCEEDED(status)) {
			status = tmpStatus ;
			SET_FAILURE_REASON(status) ;
		}

		if (DSP_FAILED(tmpStatus) && DSP_SUCCEEDED(status)) {
			status = tmpStatus ;
			SET_FAILURE_REASON(status) ;
		}

		SYNC_Exit() ;

		if (DSP_FAILED(tmpStatus) && DSP_SUCCEEDED(status)) {
			status = tmpStatus ;
			SET_FAILURE_REASON(status) ;
		}
	}

	if (OSAL_initCount > 0) {
		if (DSP_SUCCEEDED(status))
			OSAL_initCount-- ;

	} else {
		status = DSP_EFAIL ;
		SET_FAILURE_REASON(status) ;
	}

	TRC_1LEAVE("OSAL_exit", status) ;

	return status ;
}
