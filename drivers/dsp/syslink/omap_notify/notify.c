
/**============================================================================
*@file   notify.c
*
*@path   $(NOTIFY)/dsp/src/Notify/
*
*@desc   Implementation of Notify module
*
*@ver    1.00.00.01
*============================================================================
*Copyright(c) Texas Instruments Incorporated 2002-2008
*
*Use of this software is controlled by the terms and conditions found in the
*license agreement under which this software has been supplied or provided.
*============================================================================
*/

#if !defined(EXPORT_SYMTAB)
#define EXPORT_SYMTAB 1
#endif
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <asm/pgtable.h>


/*----------------------------------- IPC Headers                 */
#include <gpptypes.h>
#include <ipctypes.h>
#include <dbc.h>

/*----------------------------------- OSAL Headers                */
#include <sync.h>

/*----------------------------------- Notify Headers              */
#include <notify.h>
#include <notify_driver.h>
#include <_notifydefs.h>
#include <notifyerr.h>

/*----------------------------------- Generic Headers             */
#include <trc.h>
#include<gen_utils.h>
#include<global_var.h>


/**============================================================================
*@macro  COMPONENT_ID
*
*@desc   Component and Subcomponent Identifier.
*============================================================================
*/
#define  COMPONENT_ID       ID_KNL_NOTIFY

/**============================================================================
*@macro  SET_FAILURE_REASON
*
*@desc   Sets failure reason.
*============================================================================
*/
#if defined(NOTIFY_DEBUG)
#define SET_FAILURE_REASON(status)  (TRC_SetReason \
(status, FID_C_KNL_NOTIFY, __LINE__))
#else
#define SET_FAILURE_REASON(status)
#endif /*if defined(NOTIFY_DEBUG) */

/**============================================================================
*@const  SPINLOCK_START
*
*@desc   Start spinlock protection
*============================================================================
*/
#define SPINLOCK_START()           SYNC_SpinLockStart()

/**============================================================================
*@const  SPINLOCK_END
*
*@desc   End spinlock protection
*============================================================================
*/
#define SPINLOCK_END(irqFlags)     SYNC_SpinLockEnd(irqFlags)




/**----------------------------------------------------------------------------
*@func   _Notify_findDriver
*
*@desc   Find a driver by name in the drivers array and return handle to the
*driver object.
*
*@arg    driverName
*Name of the Notify driver to be found.
*@arg    handle
*Location to receive the handle to the Notify driver.
*
*@ret    NOTIFY_SOK
*The Notify driver has been successfully initialized
*NOTIFY_ENOTFOUND
*
*@enter  driverName must be a valid pointer
*handle must be a valid pointer.
*
*@leave  On success, handle must have a valid Notify driver handle.
*
*@see    None
*----------------------------------------------------------------------------
*/


/**----------------------------------------------------------------------------
*@func   _Notify_isSupportedProc
*
*@desc   Check if specified processor ID is supported by the Notify driver.
*
*@arg    drvHandle
*Handle to the Notify driver
*@arg    procId
*ID of the processor to be checked
*
*@ret    TRUE
*The specified processor ID is supported by the driver
*FALSE
*The specified processor ID is not supported by the driver
*
*@enter  drvHandle must be a valid pointer
*
*@leave  None
*
*@see    None
*----------------------------------------------------------------------------
*/

short int
_Notify_isSupportedProc(IN  struct Notify_DriverHandle *drvHandle,
		IN  unsigned long int         procId) ;


/**============================================================================
*@name   Notify_isInit
*
*@desc   Indicates whether the Notify module has been initialized
*============================================================================
*/
short int  Notify_isInit = FALSE;
EXPORT_SYMBOL(Notify_isInit);

/*Notify_isInit = FALSE;*/


/**============================================================================
*@name   Notify_StateObj
*
*@desc   Notify state object instance
*============================================================================
*/
struct Notify_State  Notify_StateObj ;
EXPORT_SYMBOL(Notify_StateObj);



/**============================================================================
*@func   Notify_init
*
*@desc   This function initializes the Notify module.
*
*@modif  Notify_StateObj, Notify_isInit
*============================================================================
*/

signed long int
Notify_init(IN struct Notify_Attrs  *attrs)
{
	signed long int       status = NOTIFY_SOK ;
	struct Notify_DriverHandle *drvHandle ;
	unsigned short int              i ;

	TRC_1ENTER("Notify_init", attrs) ;

	DBC_require(attrs != NULL) ;

	if (Notify_isInit == TRUE) {
		status = NOTIFY_EALREADYINIT ;
		SET_FAILURE_REASON(status) ;
	} else if (attrs == NULL) {
		status = NOTIFY_EPOINTER ;
		SET_FAILURE_REASON(status) ;
	} else if (attrs->maxDrivers > NOTIFY_MAX_DRIVERS) {
		status = NOTIFY_ECONFIG ;
		SET_FAILURE_REASON(status) ;
	} else {
		Notify_StateObj.maxDrivers = attrs->maxDrivers ;
		for (i = 0 ; i < Notify_StateObj.maxDrivers ; i++) {
			drvHandle = &(Notify_StateObj.drivers[i]) ;
			drvHandle->isInit = FALSE ;
			drvHandle->name[0] = '\0' ;
			drvHandle->notifyId = i ;
			drvHandle->fnTable = NULL ;
			drvHandle->driverObj = NULL ;

		}
		Notify_isInit = TRUE ;
	}

	DBC_ensure((NOTIFY_SUCCEEDED(status) && (Notify_isInit == TRUE))
			|| (NOTIFY_FAILED(status))) ;

	TRC_1LEAVE("Notify_init", status) ;

	return status ;
}
EXPORT_SYMBOL(Notify_init);

/**============================================================================
*@func   Notify_exit
*
*@desc   This function finalizes the Notify module.
*
*@modif  Notify_isInit
*============================================================================
*/

signed long int
Notify_exit(void)
{
	signed long int status = NOTIFY_SOK ;

	TRC_0ENTER("Notify_exit") ;

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;
	} else {
		Notify_isInit = FALSE ;
		status = NOTIFY_SEXIT ;
	}

	DBC_ensure((NOTIFY_SUCCEEDED(status) && (Notify_isInit == FALSE))
			/           || (NOTIFY_FAILED(status))) ;

	TRC_1LEAVE("Notify_exit", status) ;
	return status ;
}
EXPORT_SYMBOL(Notify_exit);


/**============================================================================
*@func   Notify_driverInit
*
*@desc   This function initializes and configures the specified Notify driver
*
*@modif  Notify_StateObj
*============================================================================
*/

signed long int
Notify_driverInit(IN  char *driverName,
		IN  struct Notify_Config  *config,
		OUT void  **handle)
{
	signed long int       status = NOTIFY_SOK ;
	struct Notify_DriverHandle *drvHandle ;


	TRC_3ENTER("Notify_driverInit", driverName, config, handle) ;

	DBC_require(Notify_isInit == TRUE) ;
	DBC_require(driverName    != NULL) ;
	DBC_require(config        != NULL) ;
	DBC_require(handle        != NULL) ;

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if ((config == NULL) || (handle == NULL)) {
		status = NOTIFY_EPOINTER ;
		SET_FAILURE_REASON(status) ;

	} else {
		status = _Notify_findDriver(driverName, handle) ;
		if (NOTIFY_SUCCEEDED(status)) {
			drvHandle =
				(struct Notify_DriverHandle *)(*handle) ;

			status = drvHandle->fnTable->notifyDriverInit
					(driverName, config,
					&(drvHandle->driverObj)) ;

			if (NOTIFY_SUCCEEDED(status))
				drvHandle->isInit = TRUE ;
			 else
				SET_FAILURE_REASON(status) ;

		} else
			SET_FAILURE_REASON(status) ;

	}

	DBC_ensure((NOTIFY_SUCCEEDED(status) && (*handle != NULL))
			|| (NOTIFY_FAILED(status))) ;

	TRC_1LEAVE("Notify_driverInit", status) ;

	return status ;
}
EXPORT_SYMBOL(Notify_driverInit);

/**============================================================================
*@func   Notify_driverExit
*
*@desc   This function finalizes the specified Notify driver.
*
*@modif  Notify_StateObj
*============================================================================
*/

signed long int
Notify_driverExit(IN void  *handle)
{

	signed long int       status = NOTIFY_SOK ;
	struct Notify_DriverHandle *drvHandle =
			(struct Notify_DriverHandle *) handle ;


	TRC_1ENTER("Notify_driverExit", handle) ;

	DBC_require(Notify_isInit == TRUE) ;
	DBC_require(drvHandle      != NULL) ;
	DBC_require((drvHandle != NULL) && (drvHandle->isInit == TRUE)) ;

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else {
		status = drvHandle->fnTable->notifyDriverExit(drvHandle) ;
		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;


		drvHandle->isInit = FALSE ;
	}

	DBC_ensure((NOTIFY_SUCCEEDED(status) && (drvHandle->isInit == FALSE))
			|| (NOTIFY_FAILED(status))) ;

	TRC_1LEAVE("Notify_driverExit", status) ;

	return status ;
}
EXPORT_SYMBOL(Notify_driverExit);
/**============================================================================
*@func   Notify_registerEvent
*
*@desc   This function registers a callback for a specific event with the
*Notify module.
*
*@modif  None
*============================================================================
*/

signed long int
Notify_registerEvent(IN     void  *handle,
		IN     unsigned long int  procId,
		IN     unsigned long int        eventNo,
		IN     FnNotifyCbck  fnNotifyCbck,
		IN OPT void *cbckArg)
{
	signed long int       status = NOTIFY_SOK ;

	struct Notify_DriverHandle *drvHandle =
		(struct Notify_DriverHandle *) handle ;

	TRC_5ENTER("Notify_registerEvent",
			handle,
			procId,
			eventNo,
			fnNotifyCbck,
			cbckArg) ;

	DBC_require(Notify_isInit == TRUE) ;
	DBC_require(drvHandle      != NULL) ;

	DBC_require((drvHandle != NULL)
			&& (drvHandle->isInit == TRUE));

	DBC_require(fnNotifyCbck != NULL) ;

	DBC_require(_Notify_isSupportedProc
				(drvHandle, procId) == TRUE) ;

	DBC_require((drvHandle != NULL)
		&& ((eventNo & NOTIFY_EVENT_MASK)
		< drvHandle->attrs.procInfo[procId].maxEvents));

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (fnNotifyCbck == NULL) {
		status = NOTIFY_EPOINTER ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (_Notify_isSupportedProc
			(drvHandle, procId)	== FALSE) {

		status = NOTIFY_ENOTSUPPORTED ;
		SET_FAILURE_REASON(status) ;
	} else if ((eventNo & NOTIFY_EVENT_MASK)
			>= drvHandle->attrs.procInfo[procId].maxEvents) {
		status = NOTIFY_EINVALIDEVENT ;
		SET_FAILURE_REASON(status) ;
	}
	/*
	   else if (((eventNo & NOTIFY_EVENT_MASK)
	   <  drvHandle->attrs.procInfo[procId].reservedEvents)
	   && (((eventNo & NOTIFY_SYSTEM_KEY_MASK)
		>> sizeof(unsigned short int))
		!= NOTIFY_SYSTEM_KEY)) {
	   status = NOTIFY_ERESERVEDEVENT ;
	   SET_FAILURE_REASON(status) ;
	   }*/
	else {
		status = drvHandle->fnTable->notifyRegisterEvent
				(drvHandle,	procId,
				(eventNo & NOTIFY_EVENT_MASK),
				fnNotifyCbck,
				cbckArg) ;

		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;

	}

	TRC_1LEAVE("Notify_registerEvent", status) ;
	return status ;
}
EXPORT_SYMBOL(Notify_registerEvent);

/**============================================================================
*@func   Notify_unregisterEvent
*
*@desc   This function un-registers the callback for the specific event with
*the Notify module.
*
*@modif  None
*============================================================================
*/

signed long int
Notify_unregisterEvent(IN     void  *handle,
		IN     unsigned long int  procId,
		IN     unsigned long int        eventNo,
		IN     FnNotifyCbck  fnNotifyCbck,
		IN OPT void *cbckArg)
{
	signed long int       status = NOTIFY_SOK ;

	struct Notify_DriverHandle *drvHandle =
			(struct Notify_DriverHandle *) handle;

	TRC_5ENTER("Notify_unregisterEvent",
			handle,
			procId,
			eventNo,
			fnNotifyCbck,
			cbckArg) ;

	DBC_require(Notify_isInit == TRUE) ;
	DBC_require(drvHandle      != NULL) ;

	DBC_require((drvHandle != NULL)
			&& (drvHandle->isInit == TRUE)) ;

	DBC_require(fnNotifyCbck != NULL) ;

	DBC_require(_Notify_isSupportedProc
				(drvHandle, procId) == TRUE) ;

	DBC_require((drvHandle != NULL)
		&& ((eventNo & NOTIFY_EVENT_MASK)
		< drvHandle->attrs.procInfo[procId].maxEvents));

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (fnNotifyCbck == NULL) {
		status = NOTIFY_EPOINTER ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (_Notify_isSupportedProc(drvHandle, procId) == FALSE) {
		status = NOTIFY_ENOTSUPPORTED ;
		SET_FAILURE_REASON(status) ;

	} else if ((eventNo & NOTIFY_EVENT_MASK)
			>= drvHandle->attrs.procInfo[procId].maxEvents) {
		status = NOTIFY_EINVALIDEVENT ;
		SET_FAILURE_REASON(status) ;

	} else if (((eventNo & NOTIFY_EVENT_MASK)
		< drvHandle->attrs.procInfo[procId].reservedEvents)
		&& (((eventNo & NOTIFY_SYSTEM_KEY_MASK)
			>> sizeof(unsigned short int))
				!= NOTIFY_SYSTEM_KEY)) {
		status = NOTIFY_ERESERVEDEVENT ;
		SET_FAILURE_REASON(status) ;

	} else {
		status =
		drvHandle->fnTable->notifyUnregisterEvent(drvHandle,
				procId,
				(eventNo & NOTIFY_EVENT_MASK),
				fnNotifyCbck,
				cbckArg) ;
		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;

	}

	TRC_1LEAVE("Notify_unregisterEvent", status) ;
	return status ;
}
EXPORT_SYMBOL(Notify_unregisterEvent);

/**============================================================================
*@func   Notify_sendEvent
*
*@desc   This function sends a notification to the specified event.
*
*@modif  None
*============================================================================
*/

signed long int
Notify_sendEvent(IN void  *handle,
		IN unsigned long int  procId,
		IN unsigned long int        eventNo,
		IN unsigned long int        payload,
		IN short int          waitClear)
{

	signed long int       status = NOTIFY_SOK ;

	struct Notify_DriverHandle *drvHandle =
			(struct Notify_DriverHandle *) handle ;

	TRC_5ENTER("Notify_sendEvent",
			handle,
			procId,
			eventNo,
			payload,
			waitClear) ;

	DBC_require(Notify_isInit == TRUE) ;

	DBC_require(drvHandle      != NULL) ;

	DBC_require((drvHandle != NULL)
			&& (drvHandle->isInit == TRUE)) ;

	DBC_require(_Notify_isSupportedProc
				(drvHandle, procId) == TRUE) ;

	DBC_require((drvHandle != NULL)
	&& ((eventNo & NOTIFY_EVENT_MASK)
	< drvHandle->attrs.procInfo[procId].maxEvents)) ;

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (_Notify_isSupportedProc
			(drvHandle, procId) == FALSE) {
		status = NOTIFY_ENOTSUPPORTED ;
		SET_FAILURE_REASON(status) ;

	} else if ((eventNo & NOTIFY_EVENT_MASK)
		>= drvHandle->attrs.procInfo[procId].maxEvents) {
		status = NOTIFY_EINVALIDEVENT ;
		SET_FAILURE_REASON(status) ;
	}

	/*else if (((eventNo & NOTIFY_EVENT_MASK)
	  <  drvHandle->attrs.procInfo[procId].reservedEvents)
	  && (((eventNo & NOTIFY_SYSTEM_KEY_MASK)
		>> sizeof(unsigned short int))
	  != NOTIFY_SYSTEM_KEY)) {
	  status = NOTIFY_ERESERVEDEVENT ;
	  SET_FAILURE_REASON(status) ;
	  }*/

	else {


		status = drvHandle->fnTable->notifySendEvent
				(drvHandle,
				procId,
				(eventNo & NOTIFY_EVENT_MASK),
				payload,
				waitClear) ;

		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;

	}

	TRC_1LEAVE("Notify_sendEvent", status) ;

	return status ;
}
EXPORT_SYMBOL(Notify_sendEvent);
/**======================================================
*@func   Notify_disable
*
*@desc   This function disables all events. This is equivalent to global
*interrupt disable, however restricted within interrupts handled by
*the Notify module. All callbacks registered for all events are
*disabled with this API. It is not possible to disable a specific
*callback.
*
*@modif  Notify_StateObj.disableFlags
*========================================================
*/

void *
Notify_disable(void)
{
	struct Notify_DriverHandle *drvHandle ;
	unsigned short int              i ;
	TRC_0ENTER("Notify_disable") ;
	DBC_require(Notify_isInit == TRUE) ;

	if (Notify_isInit == TRUE) {
		for (i = 0 ; i < Notify_StateObj.maxDrivers ; i++) {
			drvHandle = &(Notify_StateObj.drivers[i]) ;
		/*TBD: Current implementation only supports single non-nested
		 *disable/restore pair. To support nested calls, will need to
		 *have a first-in-last-out queue of disableFlags.
		 */

			Notify_StateObj.disableFlags[i] =
				drvHandle->fnTable->notifyDisable(drvHandle) ;
		}
	}

	TRC_1PRINT(TRC_LEVEL1, "    flags[%d]",
				&(Notify_StateObj.disableFlags)) ;
	TRC_0LEAVE("Notify_disable") ;

	return (void *) &(Notify_StateObj.disableFlags) ;
}
EXPORT_SYMBOL(Notify_disable);

/**=========================================================
*@func   Notify_restore
*
*@desc   This function restores the Notify module to the state before the
*last Notify_disable() was called. This is equivalent to global
*interrupt restore, however restricted within interrupts handled by
*the Notify module. All callbacks registered for all events as
*specified in the flags are enabled with this API. It is not possible
*to enable a specific callback.
*
*@modif  None
*===========================================================
*/

signed long int
Notify_restore(IN void *flags)
{
	signed long int       tmpStatus = NOTIFY_SOK ;
	signed long int       status = NOTIFY_SOK ;
	struct Notify_DriverHandle *drvHandle ;
	void **driverFlags ;
	unsigned short int              i ;



	TRC_1ENTER("Notify_restore", flags) ;

	DBC_require(Notify_isInit == TRUE) ;
	DBC_require(flags          != NULL) ;

	DBC_require(Notify_isInit == TRUE) ;

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;
	} else if (flags == NULL) {
		status = NOTIFY_EINVALIDARG ;
		SET_FAILURE_REASON(status) ;
	} else {
		driverFlags = (void **) flags ;

		for (i = 0 ; i < Notify_StateObj.maxDrivers ; i++) {
			drvHandle = &(Notify_StateObj.drivers[i]) ;
		/*TBD: Current implementation only supports single non-nested
		 *disable/restore pair. To support nested calls, will need to
		 *have a first-in-last-out queue of disableFlags.
		 */
			tmpStatus = drvHandle->fnTable->notifyRestore
					(drvHandle,
					driverFlags[i]) ;
	/*Save the failure status even if any of the driver calls fail */
			if (NOTIFY_SUCCEEDED(status)
						&& NOTIFY_FAILED(tmpStatus)) {
				status = tmpStatus ;
				SET_FAILURE_REASON(status) ;
			}
		}
	}

	TRC_1LEAVE("Notify_restore", status) ;
	return status ;
}
EXPORT_SYMBOL(Notify_restore);
/**============================================================================
*@func   Notify_disableEvent
*
*@desc   This function disables a specific event. All callbacks registered
*for the specific event are disabled with this API. It is not
*possible to disable a specific callback.
*
*@modif  None
*============================================================================
*/

signed long int
Notify_disableEvent(IN void  *handle,
		IN unsigned long int  procId,
		IN unsigned long int        eventNo)
{
	signed long int       status = NOTIFY_SOK ;
	struct Notify_DriverHandle *drvHandle =
			(struct Notify_DriverHandle *) handle ;

	TRC_3ENTER("Notify_disableEvent", handle, procId, eventNo) ;

	DBC_require(Notify_isInit == TRUE) ;
	DBC_require(drvHandle      != NULL) ;
	DBC_require((drvHandle != NULL)
				&& (drvHandle->isInit == TRUE)) ;

	DBC_require(_Notify_isSupportedProc
			(drvHandle, procId) == TRUE) ;

	DBC_require((drvHandle != NULL)
		&& ((eventNo & NOTIFY_EVENT_MASK)
		< drvHandle->attrs.procInfo[procId].maxEvents)) ;

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (_Notify_isSupportedProc
			(drvHandle, procId) == FALSE) {
		status = NOTIFY_ENOTSUPPORTED ;
		SET_FAILURE_REASON(status) ;

	} else if ((eventNo & NOTIFY_EVENT_MASK)
		>= drvHandle->attrs.procInfo[procId].maxEvents) {
		status = NOTIFY_EINVALIDEVENT ;
		SET_FAILURE_REASON(status) ;

	} else if (((eventNo & NOTIFY_EVENT_MASK)
			<  drvHandle->attrs.procInfo[procId].reservedEvents)
			&& (((eventNo & NOTIFY_SYSTEM_KEY_MASK) >>
			sizeof(unsigned short int))
				!= NOTIFY_SYSTEM_KEY)) {
		status = NOTIFY_ERESERVEDEVENT ;
		SET_FAILURE_REASON(status) ;

	} else {
		status = drvHandle->fnTable->notifyDisableEvent
				(drvHandle,
				procId,
				(eventNo & NOTIFY_EVENT_MASK)) ;

		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;

	}

	TRC_1LEAVE("Notify_disableEvent", status) ;
	return status ;
}
EXPORT_SYMBOL(Notify_disableEvent);
/**============================================================================
*@func   Notify_enableEvent
*
*@desc   This function enables a specific event. All callbacks registered for
*this specific event are enabled with this API. It is not possible to
*enable a specific callback.
*
*@modif  None
*============================================================================
*/

signed long int
Notify_enableEvent(IN void  *handle,
		IN unsigned long int  procId,
		IN unsigned long int        eventNo)
{
	signed long int       status = NOTIFY_SOK ;

	struct Notify_DriverHandle *drvHandle =
			(struct Notify_DriverHandle *) handle ;

	TRC_3ENTER("Notify_enableEvent", handle, procId, eventNo) ;

	DBC_require(Notify_isInit == TRUE) ;
	DBC_require(drvHandle      != NULL) ;

	DBC_require((drvHandle != NULL)
			&& (drvHandle->isInit == TRUE)) ;

	DBC_require(_Notify_isSupportedProc
			(drvHandle, procId) == TRUE) ;

	DBC_require((drvHandle != NULL)
		&& ((eventNo & NOTIFY_EVENT_MASK)
		< drvHandle->attrs.procInfo[procId].maxEvents));

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;
	} else if (drvHandle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drvHandle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (_Notify_isSupportedProc
				(drvHandle, procId) == FALSE) {
		status = NOTIFY_ENOTSUPPORTED ;
		SET_FAILURE_REASON(status) ;

	} else if ((eventNo & NOTIFY_EVENT_MASK)
		>= drvHandle->attrs.procInfo[procId].maxEvents) {
		status = NOTIFY_EINVALIDEVENT ;
		SET_FAILURE_REASON(status) ;
	}
	/*
	   else if (((eventNo & NOTIFY_EVENT_MASK)
	   <  drvHandle->attrs.procInfo[procId].reservedEvents)
	   && (((eventNo & NOTIFY_SYSTEM_KEY_MASK)
		>> sizeof(unsigned short int))
	   != NOTIFY_SYSTEM_KEY)) {
	   status = NOTIFY_ERESERVEDEVENT ;
	   SET_FAILURE_REASON(status) ;
	   }*/
	else {
		status = drvHandle->fnTable->notifyEnableEvent
				(drvHandle,
				procId,
				(eventNo & NOTIFY_EVENT_MASK)) ;
		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;

	}

	TRC_1LEAVE("Notify_enableEvent", status) ;

	return status ;
}
EXPORT_SYMBOL(Notify_enableEvent);
#if defined(NOTIFY_DEBUG)
/**========================================================
*@func   Notify_debug
*
*@desc   This function prints out debug information about
the Notify module.
*
*@modif  None
*==========================================================
*/

signed long int
Notify_debug(IN void  *handle)
{
	signed long int status = NOTIFY_SOK ;
	TRC_1ENTER("Notify_debug", handle) ;

	DBC_require(Notify_isInit == TRUE) ;
	DBC_require(handle         != NULL) ;

	TRC_1LEAVE("Notify_debug", status) ;

	return status ;
}
EXPORT_SYMBOL(Notify_debug);
#endif /*defined(NOTIFY_DEBUG) */


/**--------------------------------------------------------
*@func   _Notify_findDriver
*
*@desc   Find a driver by name in the drivers array and
return handle to the
*driver object.
*
*@modif  None
*-----------------------------------------------------------
*/

signed long int
_Notify_findDriver(IN  char *driverName,
		OUT void  **handle)
{
	signed long int       status = NOTIFY_ENOTFOUND ;
	struct Notify_DriverHandle *drvHandle ;
	unsigned short int              i ;
	TRC_2ENTER("_Notify_findDriver", driverName, handle) ;

	DBC_require(driverName    != NULL) ;
	DBC_require(handle        != NULL) ;

	*handle = NULL ;
	for (i = 0 ; i < Notify_StateObj.maxDrivers ; i++) {
		drvHandle = &(Notify_StateObj.drivers[i]) ;
	/*The function table pointer is used to define whether any driver
	 *is registered in this slot. It is set to TRUE when slot is reserved,
	 *but not yet filled.
	 */
		if ((drvHandle->fnTable != NULL)
				&& (drvHandle->fnTable !=
				(struct Notify_Interface *) TRUE)) {

			if (GEN_strncmp(driverName,
					drvHandle->name, NOTIFY_MAX_NAMELEN)
					== 0) {
				/*Found the driver. */
				*handle = (void *) drvHandle;
				status = NOTIFY_SOK ;
				break ;
			}
		}
	}

	DBC_ensure((NOTIFY_SUCCEEDED(status) && (*handle != NULL))
			|| (NOTIFY_FAILED(status)    && (*handle == NULL))) ;

	TRC_1LEAVE("_Notify_findDriver", status) ;

	return status ;
}
EXPORT_SYMBOL(_Notify_findDriver);

/**----------------------------------------------------------------------------
*@func   _Notify_isSupportedProc
*
*@desc   Check if specified processor ID is supported by the Notify driver.
*
*@modif  None
*----------------------------------------------------------------------------
*/

short int
_Notify_isSupportedProc(IN  struct Notify_DriverHandle *drvHandle,
		IN  unsigned long int         procId)
{
	short int                 found = FALSE ;
	struct Notify_DriverAttrs *attrs ;
	unsigned short int        i ;
	TRC_2ENTER("_Notify_isSupportedProc", drvHandle, procId) ;

	DBC_require(drvHandle != NULL) ;

	attrs = &(drvHandle->attrs) ;
	for (i = 0 ; i < attrs->numProc ; i++) {
		if (attrs->procInfo[i].procId == procId) {
			found = TRUE ;
			break ;
		}
	}

	TRC_1PRINT(TRC_LEVEL1,
			"isSupportedProc[%d]", found) ;

	TRC_0LEAVE("_Notify_isSupportedProc") ;

	return found ;
}
EXPORT_SYMBOL(_Notify_isSupportedProc);


