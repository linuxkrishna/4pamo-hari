/** ============================================================================
*  @file   dpc.c
*
*  @path   $(DSPLINK)/gpp/src/osal/Linux/
*
*  @desc   Implementation of sub-component DPC.
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
#include <linux/sched.h>
#include <linux/interrupt.h>

/*  ----------------------------------- DSP/BIOS Link               */
#include <ipc.h>
#include <_ipc.h>
#include <errbase.h>
/*  ----------------------------------- Trace & Debug               */
#include <_trace.h>

/*  ----------------------------------- OSAL Headers                */
#include <osal.h>


/** ============================================================================
*  @macro  COMPONENT_ID
*
*  @desc   Component and sub-component Identifier.
*  ============================================================================
*/
#define COMPONENT_ID        ID_OSAL_DPC

/** ============================================================================
*  @macro  SET_FAILURE_REASON
*
*  @desc   Sets failure reason.
*  ============================================================================
*/
#if defined DDSP_DEBUG
#define SET_FAILURE_REASON  TRC_SetReason(status, FID_C_OSAL_DPC, __LINE__)
#else
#define SET_FAILURE_REASON
#endif /* if defined (DDSP_DEBUG) */

/** ============================================================================
*  @const  MAX_DPC
*
*  @desc   Maximum number of DPCs supported in DSP/BIOS Link port to mvlinux.
*  ============================================================================
*/
#define MAX_DPC             16

/** ============================================================================
*  @const  INVALID_INDEX
*
*  @desc   Invalid index of a DPC object.
*  ============================================================================
*/
#define INVALID_INDEX       0xFFFFFFFF



/** ============================================================================
*  @name   struct DPC_DpcTaskletInfo
*
*  @desc   This structure defines the association between struct DpcObjects and their
*          corresponding tasklets. It also contains a bitmap for tracking used
*          Dpc objects.
*
*  @field  usedDPCs
*              Bitmap for tracking used DPCs.
*  @field  dpcs
*              Global store for DPCs.
*  @field  dpcTasklet
*              Array of tasklets (linux specific)
*
*  @see    struct DpcObject_tag
*  ============================================================================
*/
struct DPC_DpcTaskletInfo {
unsigned long int  usedDPCs;
struct DpcObject   dpcs[MAX_DPC];
struct tasklet_struct  dpcTasklet[MAX_DPC] ;
};


/** ============================================================================
*  @name   DPC_DpcTaskletMap
*
*  @desc   Subcomponent variable to keep of all the DPCs and associated
*          hisrs created for 'Link.
*  ============================================================================
*/
static struct DPC_DpcTaskletInfo DPC_DpcTaskletMap;

/** ============================================================================
*  @name   DPC_IsInitialized
*
*  @desc   Flag to keep track of initialization of this subcomponent.
*  ============================================================================
*/
static short int DPC_IsInitialized = FALSE;


/** ----------------------------------------------------------------------------
*  @func   DPC_Callback
*
*  @desc   This function gets invoked by the OS when it schedules a tasklet
*          to execute.
*
*  @arg    index
*              Indicates the index in DPC object table.
*
*  @ret    None
*
*  @enter  None
*
*  @leave  None
*
*  @see    DPC_Schedule
*  ----------------------------------------------------------------------------
*/
/*static void DPC_Callback(unsigned long index);*/

/*  ----------------------------------------------------------------------------
*  @func   ResetDpcObj
*
*  @desc   Resets the fields in a Dpc object.
*
*  @arg    dpcObj
*              The DPC object to be reset.
*
*  @ret    None
*
*  @enter  dpcObj must be a valid DPC object.
*
*  @leave  All fields in dpcObj are reset.
*
*  @see    DPC_Callback, DPC_Create
*  ----------------------------------------------------------------------------
*/
static void ResetDpcObj(struct DpcObject *dpcObj);


/** ============================================================================
*  @func   DPC_init
*
*  @desc   This function initializes the DPC module. It initializes the
*          global area (DPC_DpcTaskletMap structure) for holding all the
*          DPC objects and marks the UsedDPCs bitmap to indicate that no
*          DPCs are currently in use.
*
*  @modif  DPC_DpcTaskletMap
*  ============================================================================
*/

signed long int
DPC_init()
{
	signed long int  status = DSP_SOK ;
	unsigned long int      index  = 0       ;

	TRC_0ENTER("DPC_init") ;

	if (DPC_IsInitialized == FALSE) {
		DPC_DpcTaskletMap.usedDPCs = 0 ;

		for (index = 0 ; index < MAX_DPC ; index++)
			ResetDpcObj(&DPC_DpcTaskletMap.dpcs[index]);


		DPC_IsInitialized = TRUE ;
	}

	DBC_Ensure(DPC_IsInitialized == TRUE) ;

	TRC_1LEAVE("DPC_init", status) ;

	return status ;
}


/** ============================================================================
*  @func   DPC_exit
*
*  @desc   This function provides an interface to exit from the DPC module.
*          It frees up all the used DPCs and releases all the resources used by
*          this module.
*
*  @modif  DPC_DpcTaskletMap
*  ============================================================================
*/

signed long int
DPC_exit()
{
	signed long int  status = DSP_SOK ;
	unsigned long int      i      = 0       ;

	TRC_0ENTER("DPC_exit") ;

	DBC_Require(DPC_IsInitialized == TRUE) ;

	if (DPC_IsInitialized == TRUE) {
		/*
		* Get all the used DPCs objects from the usedDPC bitmask
		* and release them.
		*/

		for (i = 0 ; i < MAX_DPC ; i++) {
			if (TEST_BIT(DPC_DpcTaskletMap.usedDPCs, i) == 1u) {

				tasklet_kill(&(DPC_DpcTaskletMap.
						dpcTasklet[i]));


				ResetDpcObj(&DPC_DpcTaskletMap.dpcs[i]);
			}
		}

		DPC_IsInitialized = FALSE ;
	}

	DPC_DpcTaskletMap.usedDPCs = 0 ; /* reset the used DPC bitmask */

	DBC_Ensure(DPC_IsInitialized == FALSE) ;
	DBC_Ensure(DPC_DpcTaskletMap.usedDPCs == 0) ;

	TRC_1LEAVE("DPC_exit", status) ;

	return status ;
}


/** ============================================================================
*  @func   DPC_Create
*
*  @desc   Creates a DPC object and returns it after populating
*          relevant fields. It sets a bit in UsedDPCs bitmask.
*
*  @modif  DPC_DpcTaskletMap
*  ============================================================================
*/
/*
static signed long int
NDPC_Create(IN  FnDpcProc userDPCFn,
IN  void *dpcArgs,
OUT struct DpcObject **dpcObj)
{
	signed long int  status = DSP_SOK ;
	unsigned long int      index  = 0 ;
	short int        found  = FALSE   ;

	TRC_3ENTER("DPC_Create", userDPCFn, dpcArgs, dpcObj) ;

	DBC_Require(DPC_IsInitialized == TRUE) ;
	DBC_Require(userDPCFn != NULL) ;
	DBC_Require(dpcObj != NULL) ;

	if ((userDPCFn == NULL) || (dpcObj == NULL)) {
		status = DSP_EINVALIDARG ;
		SET_FAILURE_REASON ;
	} else {
		index = 0 ;
		found = FALSE ;

		while ((found == FALSE) && (index < MAX_DPC)) {
			if (TEST_BIT(DPC_DpcTaskletMap.usedDPCs,
							index) == 0u)
				found = TRUE ;
			else
				index++ ;

		}

		if (found == FALSE) {
			status = DSP_ERESOURCE ;
			SET_FAILURE_REASON ;
		} else {

			SET_BIT(DPC_DpcTaskletMap.usedDPCs, index) ;
			DPC_DpcTaskletMap.dpcs[index].signature    = SIGN_DPC  ;
			DPC_DpcTaskletMap.dpcs[index].index        = index     ;
			DPC_DpcTaskletMap.dpcs[index].paramData    = dpcArgs   ;
			DPC_DpcTaskletMap.dpcs[index].userDPCFn    = userDPCFn ;
			DPC_DpcTaskletMap.dpcs[index].state        = TRUE      ;
			atomic_set(&DPC_DpcTaskletMap.dpcs[index].pendCount, 0);
*/
			/*
			*  Initialize the tasklet structure.
			*/
/*			tasklet_init(&(DPC_DpcTaskletMap.dpcTasklet[index]),
			DPC_Callback,
			index) ;

			*dpcObj = &DPC_DpcTaskletMap.dpcs[index] ;
		}
	}

	DBC_Ensure(((DSP_SUCCEEDED(status))
	&& (IS_OBJECT_VALID(*dpcObj, SIGN_DPC)))
	|| (DSP_FAILED(status))) ;

	TRC_1LEAVE("DPC_Create", status) ;

	return status ;
}
*/

/** ============================================================================
*  @func   DPC_Delete
*
*  @desc   Destroys the DPC object by resetting the UsedDPCs bitmask.
*
*  @modif  DPC_DpcTaskletMap
*  ============================================================================
*/

signed long int
DPC_Delete(IN struct DpcObject *dpcObj)
{
	signed long int status = DSP_SOK ;
	unsigned long int     index  = 0       ;

	TRC_1ENTER("DPC_Delete", dpcObj) ;

	DBC_Require(DPC_IsInitialized == TRUE) ;
	DBC_Require(dpcObj != NULL) ;
	DBC_Require(IS_OBJECT_VALID(dpcObj, SIGN_DPC)) ;

	if (IS_OBJECT_VALID(dpcObj, SIGN_DPC) == FALSE) {
		status = DSP_EPOINTER ;
		SET_FAILURE_REASON;
	} else {
		index = dpcObj->index ;
		if (index != INVALID_INDEX) {
			CLEAR_BIT(DPC_DpcTaskletMap.usedDPCs, index) ;
			tasklet_kill(&(DPC_DpcTaskletMap.dpcTasklet[index]));
			ResetDpcObj(dpcObj) ;
		} else {
			status = DSP_EINVALIDARG ;
			SET_FAILURE_REASON ;
		}
	}

	DBC_Ensure((DSP_SUCCEEDED(status) && (dpcObj->signature == SIGN_NULL))
	|| DSP_FAILED(status));

	TRC_1LEAVE("DPC_Delete", status) ;

	return status ;
}


/** ============================================================================
*  @func   DPC_Cancel
*
*  @desc   Cancels the DPC.
*
*  @modif  None
*  ============================================================================
*/

signed long int
DPC_Cancel(IN struct DpcObject *dpcObj)
{
	signed long int  status       = DSP_SOK ;
	unsigned long int      index        = 0       ;

	TRC_1ENTER("DPC_Cancel", dpcObj) ;

	DBC_Require(DPC_IsInitialized == TRUE) ;
	DBC_Require(dpcObj != NULL) ;
	DBC_Require(IS_OBJECT_VALID(dpcObj, SIGN_DPC)) ;

	if (IS_OBJECT_VALID(dpcObj, SIGN_DPC) == FALSE) {
		status = DSP_EPOINTER ;
		SET_FAILURE_REASON ;
	} else {
		index  = dpcObj->index ;
		if (index != INVALID_INDEX) {
			/*
			*  Blocks till Scheduled DPC, if any, is completed.
			*/
			tasklet_disable(&(DPC_DpcTaskletMap.
						dpcTasklet[index]));

			atomic_set(&DPC_DpcTaskletMap.dpcs[index].
						pendCount, 0) ;


			/*
			*  Re-enable the tasklet
			*/
			tasklet_enable(&(DPC_DpcTaskletMap.dpcTasklet[index]));
		} else {
			status = DSP_EINVALIDARG ;
			SET_FAILURE_REASON ;
		}
	}

	TRC_1LEAVE("DPC_Cancel", status) ;

	return status ;
}


/** ============================================================================
*  @func   DPC_Schedule
*
*  @desc   This function schedules the DPC object, causing the user defined
*          function associated with it to be invoked at a later point of time.
*          This function is called from ISR context.
*
*  @modif  None
*  ============================================================================
*/

signed long int
NDPC_Schedule(IN  struct DpcObject *dpcObj)
{
	signed long int  status   = DSP_SOK;
	unsigned long int      index    = 0          ;

	DBC_Require(DPC_IsInitialized == TRUE) ;
	DBC_Require(dpcObj != NULL) ;
	DBC_Require(IS_OBJECT_VALID(dpcObj, SIGN_DPC)) ;
	DBC_Require((dpcObj != NULL) && (dpcObj->index != INVALID_INDEX)) ;

	if (IS_OBJECT_VALID(dpcObj, SIGN_DPC) == FALSE) {
		status = DSP_EPOINTER ;
		SET_FAILURE_REASON ;
	} else {
		index = dpcObj->index ;
		if (index != INVALID_INDEX) {
			/*
			*  Increment the pend counter.
			*/
			atomic_inc(&DPC_DpcTaskletMap.dpcs[index].pendCount);

				if (DPC_DpcTaskletMap.dpcs[index].
							state == TRUE) {

					tasklet_schedule(&(DPC_DpcTaskletMap.
							dpcTasklet[index])) ;
				}
		} else {
			status = DSP_EINVALIDARG ;
			SET_FAILURE_REASON ;
		}
	}

	return status ;
}


/** ============================================================================
*  @func   DPC_Disable
*
*  @desc   Disables execution of DPCs.
*
*  @modif  None
*  ============================================================================
*/

void
DPC_Disable()
{
	signed short int i ;

	for (i = 0 ; i < MAX_DPC ; i++) {
		if (IS_OBJECT_VALID(&(DPC_DpcTaskletMap.dpcs[i]), SIGN_DPC)) {

			DPC_DpcTaskletMap.dpcs[i].state = FALSE ;
			tasklet_disable(&(DPC_DpcTaskletMap.dpcTasklet[i]));
		}
	}
}


/** ============================================================================
*  @func   DPC_Enable
*
*  @desc   Enables execution of DPCs.
*
*  @modif  None
*  ============================================================================
*/

void
DPC_Enable()
{
	signed short int i ;

	for (i = 0 ; i < MAX_DPC ; i++) {
		if (IS_OBJECT_VALID(&(DPC_DpcTaskletMap.dpcs[i]), SIGN_DPC)) {

			tasklet_enable(&(DPC_DpcTaskletMap.dpcTasklet[i])) ;
			DPC_DpcTaskletMap.dpcs[i].state = TRUE ;

			if (atomic_read(&DPC_DpcTaskletMap.dpcs[i].
							pendCount) > 0) {

				tasklet_schedule(&(DPC_DpcTaskletMap.
							dpcTasklet[i]));
			}
		}
	}
}


#if defined DDSP_DEBUG
/** ============================================================================
*  @func   DPC_Debug
*
*  @desc   This function prints the current status of DPC objects in
*          the 'Link.
*
*  @modif  None
*  ============================================================================
*/

void
DPC_Debug()
{
	unsigned long int i = 0 ;

	TRC_0ENTER("DPC_Debug") ;

	TRC_1PRINT(TRC_LEVEL1, "DPC_DpcTaskletMap.usedDPCs[0x%x]\n",
	DPC_DpcTaskletMap.usedDPCs) ;

	for (i = 0 ; i < MAX_DPC ; i++) {
		TRC_1PRINT(TRC_LEVEL1, "DPC_DpcTaskletMap.dpcs[0x%x]\n", i);
		TRC_1PRINT(TRC_LEVEL1, "   signature[0x%x]\n",
		DPC_DpcTaskletMap.dpcs[i].signature) ;
		TRC_1PRINT(TRC_LEVEL1, "   userDPCFn[0x%x]\n",
		DPC_DpcTaskletMap.dpcs[i].userDPCFn) ;
		TRC_1PRINT(TRC_LEVEL1, "   paramData[0x%x]\n",
		DPC_DpcTaskletMap.dpcs[i].paramData) ;
	}

	TRC_0LEAVE("DPC_Debug") ;
}
#endif /* defined DDSP_DEBUG */


/*  ----------------------------------------------------------------------------
*  @func   DPC_Callback
*
*  @desc   This function gets invoked when a tasklet is scheduled to run
*          by kernel. It services all the pending DPC requests.
*
*  @modif  None
*  ----------------------------------------------------------------------------
*/
/*
static
void
DPC_Callback(unsigned long index)
{
	signed long int  status = DSP_SOK ;
	struct DpcObject *dpcObj = NULL ;

	TRC_0ENTER("DPC_Callback");

	DBC_Require(index < MAX_DPC);



	dpcObj = &DPC_DpcTaskletMap.dpcs[index];

	if (IS_OBJECT_VALID(dpcObj, SIGN_DPC)) {
		while (atomic_read(&DPC_DpcTaskletMap.dpcs[index].pendCount)
						&& DSP_SUCCEEDED(status)) {

			if (*dpcObj->userDPCFn != NULL) {

				(*dpcObj->userDPCFn)(dpcObj->paramData);

				atomic_dec(&DPC_DpcTaskletMap.dpcs[index].
								pendCount);
			} else {
				status = DSP_EPOINTER ;
				SET_FAILURE_REASON ;
			}
		}
	} else {
		status = DSP_EPOINTER ;
		SET_FAILURE_REASON ;
	}



	TRC_0LEAVE("DPC_Callback") ;
}
*/

/*  ----------------------------------------------------------------------------
*  @func   ResetDpcObj
*
*  @desc   Resets a DPC object.
*
*  @modif  None
*  ----------------------------------------------------------------------------
*/
static
void
ResetDpcObj(struct DpcObject *dpcObj)
{
	DBC_Require(dpcObj != NULL) ;

	TRC_1ENTER("ResetDpcObj", dpcObj) ;

	dpcObj->signature    = SIGN_NULL     ;
	dpcObj->index        = INVALID_INDEX ;
	dpcObj->paramData    = NULL          ;
	dpcObj->userDPCFn    = NULL          ;
	dpcObj->state        = FALSE         ;

	atomic_set(&dpcObj->pendCount, 0);

	TRC_0LEAVE("ResetDpcObj") ;
}


