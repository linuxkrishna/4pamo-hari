
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
#include <dbc.h>


/*----------------------------------- Notify Headers              */
#include <notify.h>
#include <notify_driver.h>
#include <notifyerr.h>

/*----------------------------------- Generic Headers             */
#include<global_var.h>
#include<gt.h>


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
#define SET_FAILURE_REASON(status)
#else
#define SET_FAILURE_REASON(status)
#endif /*if defined(NOTIFY_DEBUG) */

/**============================================================================
*@const  SPINLOCK_START
*
*@desc   Start spinlock protection
*============================================================================
*/
#define SPINLOCK_START()           sync_spinlock_start()

/**============================================================================
*@const  SPINLOCK_END
*
*@desc   End spinlock protection
*============================================================================
*/
#define SPINLOCK_END(irqFlags)     sync_spinlock_end(irqFlags)




/**----------------------------------------------------------------------------
*@func   _notify_find_driver
*
*@desc   Find a driver by name in the drivers array and return handle to the
*driver object.
*
*@arg    driver_name
*Name of the Notify driver to be found.
*@arg    handle
*Location to receive the handle to the Notify driver.
*
*@ret    NOTIFY_SOK
*The Notify driver has been successfully initialized
*NOTIFY_ENOTFOUND
*
*@enter  driver_name must be a valid pointer
*handle must be a valid pointer.
*
*@leave  On success, handle must have a valid Notify driver handle.
*
*@see    None
*----------------------------------------------------------------------------
*/


/**----------------------------------------------------------------------------
*@func   _notify_is_support_proc
*
*@desc   Check if specified processor ID is supported by the Notify driver.
*
*@arg    drv_handle
*Handle to the Notify driver
*@arg    proc_id
*ID of the processor to be checked
*
*@ret    TRUE
*The specified processor ID is supported by the driver
*FALSE
*The specified processor ID is not supported by the driver
*
*@enter  drv_handle must be a valid pointer
*
*@leave  None
*
*@see    None
*----------------------------------------------------------------------------
*/

short int
_notify_is_support_proc(IN  struct notify_driver_handle *drv_handle,
		IN  unsigned long int         proc_id) ;


/**============================================================================
*@name   notify_is_init
*
*@desc   Indicates whether the Notify module has been initialized
*============================================================================
*/
short int  notify_is_init = FALSE;
EXPORT_SYMBOL(notify_is_init);

/*notify_is_init = FALSE;*/


/**============================================================================
*@name   notify_state_obj
*
*@desc   Notify state object instance
*============================================================================
*/
struct notify_state  notify_state_obj ;
EXPORT_SYMBOL(notify_state_obj);

#if GT_TRACE
static struct GT_Mask notify_debugmask = { NULL, NULL };  /* GT trace variable */
EXPORT_SYMBOL(notify_debugmask);
#endif


/**============================================================================
*@func   notify_init
*
*@desc   This function initializes the Notify module.
*
*@modif  notify_state_obj, notify_is_init
*============================================================================
*/

signed long int notify_init(IN struct notify_attrs  *attrs)
{
	struct notify_driver_handle *drv_handle ;
	unsigned short int              i ;

	GT_create(&notify_debugmask, "NT");
	gt_1trace(notify_debugmask, GT_ENTER, "notify_init", attrs) ;

	BUG_ON(attrs == NULL) ;
	BUG_ON(notify_is_init == true) ;
	BUG_ON(attrs->maxDrivers > NOTIFY_MAX_DRIVERS);

	notify_state_obj.maxDrivers = attrs->maxDrivers ;
	for (i = 0 ; i < notify_state_obj.maxDrivers ; i++) {
		drv_handle = &(notify_state_obj.drivers[i]) ;
		drv_handle->isInit = FALSE ;
		drv_handle->name[0] = '\0' ;
		drv_handle->notifyId = i ;
		drv_handle->fn_table = NULL ;
		drv_handle->driver_object = NULL ;

	}
	notify_is_init = TRUE ;
	gt_1trace(notify_debugmask, GT_5CLASS, "notify_init", status) ;

	return 0 ;
}
EXPORT_SYMBOL(notify_init);

/**============================================================================
*@func   notify_exit
*
*@desc   This function finalizes the Notify module.
*
*@modif  notify_is_init
*============================================================================
*/

signed long int notify_exit(void)
{
	int status = 0 ;

	gt_0trace(notify_debugmask, GT_ENTER, "notify_exit") ;
	WARN_ON(notify_is_init == false);

	if (notify_is_init == FALSE)
		status = -EFAULT;
	gt_1trace(notify_debugmask, GT_ENTER, "notify_exit status = %d\n",
							status);
	return status ;
}
EXPORT_SYMBOL(notify_exit);


/**============================================================================
*@func   notify_driver_init
*
*@desc   This function initializes and configures the specified Notify driver
*
*@modif  notify_state_obj
*============================================================================
*/

signed long int notify_driver_init(IN  char *driver_name,
		IN  struct notify_config  *config,
		OUT void  **handle)
{
	signed long int       status = NOTIFY_SOK ;
	struct notify_driver_handle *drv_handle ;


	gt_3trace(notify_debugmask, GT_ENTER,
		"notify_driver_init", driver_name, config, handle) ;

	BUG_ON(notify_is_init != true) ;
	BUG_ON(driver_name == NULL) ;
	BUG_ON(config == NULL) ;
	BUG_ON(handle == NULL) ;

	if (notify_is_init == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if ((config == NULL) || (handle == NULL)) {
		status = NOTIFY_EPOINTER ;
		SET_FAILURE_REASON(status) ;

	} else {
		status = _notify_find_driver(driver_name, handle) ;
		if (NOTIFY_SUCCEEDED(status)) {
			drv_handle =
				(struct notify_driver_handle *)(*handle) ;

			status = drv_handle->fn_table->notifyDriverInit
					(driver_name, config,
					&(drv_handle->driver_object)) ;

			if (NOTIFY_SUCCEEDED(status))
				drv_handle->isInit = TRUE ;
			 else
				SET_FAILURE_REASON(status) ;

		} else{
			SET_FAILURE_REASON(status) ;
		}

	}

	DBC_ensure((NOTIFY_SUCCEEDED(status) && (*handle != NULL))
			|| (NOTIFY_FAILED(status))) ;

	gt_1trace(notify_debugmask, GT_5CLASS, "notify_driver_init", status) ;

	return status ;
}
EXPORT_SYMBOL(notify_driver_init);

/**============================================================================
*@func   notify_driver_exit
*
*@desc   This function finalizes the specified Notify driver.
*
*@modif  notify_state_obj
*============================================================================
*/

signed long int notify_driver_exit(IN void  *handle)
{

	signed long int       status = NOTIFY_SOK ;
	struct notify_driver_handle *drv_handle =
			(struct notify_driver_handle *) handle ;


	gt_1trace(notify_debugmask, GT_ENTER, "notify_driver_exit", handle);

	BUG_ON(notify_is_init != true);
	BUG_ON(drv_handle == NULL) ;
	BUG_ON((drv_handle == NULL) || (drv_handle->isInit != true));

	if (notify_is_init == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else {
		status = drv_handle->fn_table->notify_drv_exit(drv_handle) ;
		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;


		drv_handle->isInit = FALSE ;
	}

	DBC_ensure((NOTIFY_SUCCEEDED(status) && (drv_handle->isInit == FALSE))
			|| (NOTIFY_FAILED(status))) ;

	gt_1trace(notify_debugmask, GT_5CLASS, "notify_driver_exit", status) ;

	return status ;
}
EXPORT_SYMBOL(notify_driver_exit);
/**============================================================================
*@func   notify_register_event
*
*@desc   This function registers a callback for a specific event with the
*Notify module.
*
*@modif  None
*============================================================================
*/

signed long int notify_register_event(IN     void  *handle,
		IN     unsigned long int  proc_id,
		IN     unsigned long int        event_no,
		IN     fn_notify_cbck  fn_notify_cbck,
		IN OPT void *cbck_arg)
{
	signed long int       status = NOTIFY_SOK ;

	struct notify_driver_handle *drv_handle =
		(struct notify_driver_handle *) handle ;

	gt_5trace(notify_debugmask, GT_ENTER, "notify_register_event",
			handle,
			proc_id,
			event_no,
			fn_notify_cbck,
			cbck_arg) ;

	BUG_ON(notify_is_init != true) ;
	BUG_ON(drv_handle == NULL) ;

	BUG_ON(drv_handle->isInit != true);

	BUG_ON(fn_notify_cbck == NULL) ;

	BUG_ON(_notify_is_support_proc
				(drv_handle, proc_id) != true) ;

	BUG_ON((event_no & NOTIFY_EVENT_MASK)
		>= drv_handle->attrs.proc_info[proc_id].max_events);

	if (notify_is_init == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (fn_notify_cbck == NULL) {
		status = NOTIFY_EPOINTER ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (_notify_is_support_proc
			(drv_handle, proc_id)	== FALSE) {

		status = NOTIFY_ENOTSUPPORTED ;
		SET_FAILURE_REASON(status) ;
	} else if ((event_no & NOTIFY_EVENT_MASK)
			>= drv_handle->attrs.proc_info[proc_id].max_events) {
		status = NOTIFY_EINVALIDEVENT ;
		SET_FAILURE_REASON(status) ;
	}
	/*
	   else if (((event_no & NOTIFY_EVENT_MASK)
	   <  drv_handle->attrs.proc_info[proc_id].reserved_events)
	   && (((event_no & NOTIFY_SYSTEM_KEY_MASK)
		>> sizeof(unsigned short int))
		!= NOTIFY_SYSTEM_KEY)) {
	   status = NOTIFY_ERESERVEDEVENT ;
	   SET_FAILURE_REASON(status) ;
	   }*/
	else {
		status = drv_handle->fn_table->notify_register_event
				(drv_handle,	proc_id,
				(event_no & NOTIFY_EVENT_MASK),
				fn_notify_cbck,
				cbck_arg) ;

		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;

	}

	gt_1trace(notify_debugmask, GT_5CLASS, "notify_register_event", status) ;
	return status ;
}
EXPORT_SYMBOL(notify_register_event);

/**============================================================================
*@func   notify_unregister_event
*
*@desc   This function un-registers the callback for the specific event with
*the Notify module.
*
*@modif  None
*============================================================================
*/

signed long int notify_unregister_event(IN     void  *handle,
		IN     unsigned long int  proc_id,
		IN     unsigned long int        event_no,
		IN     fn_notify_cbck  fn_notify_cbck,
		IN OPT void *cbck_arg)
{
	signed long int       status = NOTIFY_SOK ;

	struct notify_driver_handle *drv_handle =
			(struct notify_driver_handle *) handle;

	gt_5trace(notify_debugmask, GT_ENTER, "notify_unregister_event",
			handle,
			proc_id,
			event_no,
			fn_notify_cbck,
			cbck_arg) ;

	BUG_ON(notify_is_init != TRUE) ;
	BUG_ON(drv_handle == NULL) ;

	BUG_ON((drv_handle == NULL)
			|| (drv_handle->isInit == true)) ;

	BUG_ON(fn_notify_cbck == NULL) ;

	BUG_ON(_notify_is_support_proc
				(drv_handle, proc_id) != true) ;

	BUG_ON((drv_handle == NULL)
		|| ((event_no & NOTIFY_EVENT_MASK)
		>= drv_handle->attrs.proc_info[proc_id].max_events));

	if (notify_is_init == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (fn_notify_cbck == NULL) {
		status = NOTIFY_EPOINTER ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (_notify_is_support_proc(drv_handle, proc_id) == FALSE) {
		status = NOTIFY_ENOTSUPPORTED ;
		SET_FAILURE_REASON(status) ;

	} else if ((event_no & NOTIFY_EVENT_MASK)
			>= drv_handle->attrs.proc_info[proc_id].max_events) {
		status = NOTIFY_EINVALIDEVENT ;
		SET_FAILURE_REASON(status) ;

	} else if (((event_no & NOTIFY_EVENT_MASK)
		< drv_handle->attrs.proc_info[proc_id].reserved_events)
		&& (((event_no & NOTIFY_SYSTEM_KEY_MASK)
			>> sizeof(unsigned short int))
				!= NOTIFY_SYSTEM_KEY)) {
		status = NOTIFY_ERESERVEDEVENT ;
		SET_FAILURE_REASON(status) ;

	} else {
		status =
		drv_handle->fn_table->notify_unregister_event(drv_handle,
				proc_id,
				(event_no & NOTIFY_EVENT_MASK),
				fn_notify_cbck,
				cbck_arg) ;
		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;

	}

	gt_1trace(notify_debugmask, GT_5CLASS,
		"notify_unregister_event", status) ;
	return status ;
}
EXPORT_SYMBOL(notify_unregister_event);

/**============================================================================
*@func   notify_sendevent
*
*@desc   This function sends a notification to the specified event.
*
*@modif  None
*============================================================================
*/

signed long int notify_sendevent(IN void  *handle,
		IN unsigned long int  proc_id,
		IN unsigned long int        event_no,
		IN unsigned long int        payload,
		IN short int          wait_clear)
{

	signed long int       status = NOTIFY_SOK ;

	struct notify_driver_handle *drv_handle =
			(struct notify_driver_handle *) handle ;

	gt_5trace(notify_debugmask, GT_ENTER, "notify_sendevent",
			handle,
			proc_id,
			event_no,
			payload,
			wait_clear) ;

	BUG_ON(notify_is_init != TRUE) ;

	BUG_ON(drv_handle == NULL) ;

	BUG_ON((drv_handle == NULL)
			|| (drv_handle->isInit != TRUE)) ;

	BUG_ON(_notify_is_support_proc
				(drv_handle, proc_id) != TRUE) ;

	BUG_ON((drv_handle == NULL)
	&& ((event_no & NOTIFY_EVENT_MASK)
	< drv_handle->attrs.proc_info[proc_id].max_events)) ;

	if (notify_is_init == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (_notify_is_support_proc
			(drv_handle, proc_id) == FALSE) {
		status = NOTIFY_ENOTSUPPORTED ;
		SET_FAILURE_REASON(status) ;

	} else if ((event_no & NOTIFY_EVENT_MASK)
		>= drv_handle->attrs.proc_info[proc_id].max_events) {
		status = NOTIFY_EINVALIDEVENT ;
		SET_FAILURE_REASON(status) ;
	}

	/*else if (((event_no & NOTIFY_EVENT_MASK)
	  <  drv_handle->attrs.proc_info[proc_id].reserved_events)
	  && (((event_no & NOTIFY_SYSTEM_KEY_MASK)
		>> sizeof(unsigned short int))
	  != NOTIFY_SYSTEM_KEY)) {
	  status = NOTIFY_ERESERVEDEVENT ;
	  SET_FAILURE_REASON(status) ;
	  }*/

	else {


		status = drv_handle->fn_table->notify_sendevent
				(drv_handle,
				proc_id,
				(event_no & NOTIFY_EVENT_MASK),
				payload,
				wait_clear) ;

		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;

	}

	gt_1trace(notify_debugmask, GT_5CLASS, "notify_sendevent", status) ;

	return status ;
}
EXPORT_SYMBOL(notify_sendevent);
/**======================================================
*@func   notify_disable
*
*@desc   This function disables all events. This is equivalent to global
*interrupt disable, however restricted within interrupts handled by
*the Notify module. All callbacks registered for all events are
*disabled with this API. It is not possible to disable a specific
*callback.
*
*@modif  notify_state_obj.disable_flags
*========================================================
*/

void *notify_disable(void)
{
	struct notify_driver_handle *drv_handle ;
	unsigned short int              i ;
	gt_0trace(notify_debugmask, GT_ENTER, "notify_disable") ;
	BUG_ON(notify_is_init != true) ;

	if (notify_is_init == TRUE) {
		for (i = 0 ; i < notify_state_obj.maxDrivers ; i++) {
			drv_handle = &(notify_state_obj.drivers[i]) ;
		/*TBD: Current implementation only supports single non-nested
		 *disable/restore pair. To support nested calls, will need to
		 *have a first-in-last-out queue of disable_flags.
		 */

			notify_state_obj.disable_flags[i] =
			drv_handle->fn_table->notify_disable(drv_handle);
		}
	}

	gt_1trace(notify_debugmask, GT_1CLASS, "    flags[%d]",
				&(notify_state_obj.disable_flags)) ;
	gt_0trace(notify_debugmask, GT_5CLASS, "notify_disable") ;

	return (void *) &(notify_state_obj.disable_flags) ;
}
EXPORT_SYMBOL(notify_disable);

/**=========================================================
*@func   notify_restore
*
*@desc   This function restores the Notify module to the state before the
*last notify_disable() was called. This is equivalent to global
*interrupt restore, however restricted within interrupts handled by
*the Notify module. All callbacks registered for all events as
*specified in the flags are enabled with this API. It is not possible
*to enable a specific callback.
*
*@modif  None
*===========================================================
*/

signed long int notify_restore(IN void *flags)
{
	signed long int       tmp_status = NOTIFY_SOK ;
	signed long int       status = NOTIFY_SOK ;
	struct notify_driver_handle *drv_handle ;
	void **driver_flags ;
	unsigned short int              i ;



	gt_1trace(notify_debugmask, GT_ENTER, "notify_restore", flags) ;

	BUG_ON(notify_is_init != TRUE) ;
	BUG_ON(flags == NULL) ;

	BUG_ON(notify_is_init != TRUE) ;

	if (notify_is_init == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;
	} else if (flags == NULL) {
		status = NOTIFY_EINVALIDARG ;
		SET_FAILURE_REASON(status) ;
	} else {
		driver_flags = (void **) flags ;

		for (i = 0 ; i < notify_state_obj.maxDrivers ; i++) {
			drv_handle = &(notify_state_obj.drivers[i]) ;
		/*TBD: Current implementation only supports single non-nested
		 *disable/restore pair. To support nested calls, will need to
		 *have a first-in-last-out queue of disable_flags.
		 */
			tmp_status = drv_handle->fn_table->notify_restore
					(drv_handle,
					driver_flags[i]) ;
	/*Save the failure status even if any of the driver calls fail */
			if (NOTIFY_SUCCEEDED(status)
						&& NOTIFY_FAILED(tmp_status)) {
				status = tmp_status ;
				SET_FAILURE_REASON(status) ;
			}
		}
	}

	gt_1trace(notify_debugmask, GT_5CLASS, "notify_restore", status) ;
	return status ;
}
EXPORT_SYMBOL(notify_restore);
/**============================================================================
*@func   notify_disable_event
*
*@desc   This function disables a specific event. All callbacks registered
*for the specific event are disabled with this API. It is not
*possible to disable a specific callback.
*
*@modif  None
*============================================================================
*/

signed long int notify_disable_event(IN void  *handle,
		IN unsigned long int  proc_id,
		IN unsigned long int        event_no)
{
	signed long int       status = NOTIFY_SOK ;
	struct notify_driver_handle *drv_handle =
			(struct notify_driver_handle *) handle ;

	gt_3trace(notify_debugmask, GT_ENTER,
		"notify_disable_event", handle, proc_id, event_no) ;

	BUG_ON(notify_is_init != TRUE) ;
	BUG_ON(drv_handle == NULL) ;
	BUG_ON((drv_handle == NULL)
				&& (drv_handle->isInit == true)) ;

	BUG_ON(_notify_is_support_proc
			(drv_handle, proc_id) != true) ;

	BUG_ON((drv_handle == NULL)
		|| ((event_no & NOTIFY_EVENT_MASK)
		>= drv_handle->attrs.proc_info[proc_id].max_events)) ;

	if (notify_is_init == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (_notify_is_support_proc
			(drv_handle, proc_id) == FALSE) {
		status = NOTIFY_ENOTSUPPORTED ;
		SET_FAILURE_REASON(status) ;

	} else if ((event_no & NOTIFY_EVENT_MASK)
		>= drv_handle->attrs.proc_info[proc_id].max_events) {
		status = NOTIFY_EINVALIDEVENT ;
		SET_FAILURE_REASON(status) ;

	} else if (((event_no & NOTIFY_EVENT_MASK)
			<  drv_handle->attrs.proc_info[proc_id].reserved_events)
			&& (((event_no & NOTIFY_SYSTEM_KEY_MASK) >>
			sizeof(unsigned short int))
				!= NOTIFY_SYSTEM_KEY)) {
		status = NOTIFY_ERESERVEDEVENT ;
		SET_FAILURE_REASON(status) ;

	} else {
		status = drv_handle->fn_table->notify_disable_event
				(drv_handle,
				proc_id,
				(event_no & NOTIFY_EVENT_MASK)) ;

		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;

	}

	gt_1trace(notify_debugmask, GT_5CLASS, "notify_disable_event", status) ;
	return status ;
}
EXPORT_SYMBOL(notify_disable_event);
/**============================================================================
*@func   notify_enable_event
*
*@desc   This function enables a specific event. All callbacks registered for
*this specific event are enabled with this API. It is not possible to
*enable a specific callback.
*
*@modif  None
*============================================================================
*/

signed long int notify_enable_event(IN void  *handle,
		IN unsigned long int  proc_id,
		IN unsigned long int        event_no)
{
	signed long int       status = NOTIFY_SOK ;

	struct notify_driver_handle *drv_handle =
			(struct notify_driver_handle *) handle ;

	gt_3trace(notify_debugmask, GT_ENTER,
		"notify_enable_event", handle, proc_id, event_no) ;

	BUG_ON(notify_is_init != true) ;
	BUG_ON(drv_handle == NULL) ;

	BUG_ON((drv_handle == NULL)
			|| (drv_handle->isInit != true)) ;

	BUG_ON(_notify_is_support_proc
			(drv_handle, proc_id) != true) ;

	BUG_ON((drv_handle == NULL)
		|| ((event_no & NOTIFY_EVENT_MASK)
		>= drv_handle->attrs.proc_info[proc_id].max_events));

	if (notify_is_init == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;
	} else if (drv_handle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;

	} else if (drv_handle->isInit == FALSE) {
		status = NOTIFY_EDRIVERINIT ;
		SET_FAILURE_REASON(status) ;

	} else if (_notify_is_support_proc
				(drv_handle, proc_id) == FALSE) {
		status = NOTIFY_ENOTSUPPORTED ;
		SET_FAILURE_REASON(status) ;

	} else if ((event_no & NOTIFY_EVENT_MASK)
		>= drv_handle->attrs.proc_info[proc_id].max_events) {
		status = NOTIFY_EINVALIDEVENT ;
		SET_FAILURE_REASON(status) ;
	}
	/*
	   else if (((event_no & NOTIFY_EVENT_MASK)
	   <  drv_handle->attrs.proc_info[proc_id].reserved_events)
	   && (((event_no & NOTIFY_SYSTEM_KEY_MASK)
		>> sizeof(unsigned short int))
	   != NOTIFY_SYSTEM_KEY)) {
	   status = NOTIFY_ERESERVEDEVENT ;
	   SET_FAILURE_REASON(status) ;
	   }*/
	else {
		status = drv_handle->fn_table->notify_enable_event
				(drv_handle,
				proc_id,
				(event_no & NOTIFY_EVENT_MASK)) ;
		if (NOTIFY_FAILED(status))
			SET_FAILURE_REASON(status) ;

	}

	gt_1trace(notify_debugmask, GT_5CLASS, "notify_enable_event", status) ;

	return status ;
}
EXPORT_SYMBOL(notify_enable_event);
#if defined(NOTIFY_DEBUG)
/**========================================================
*@func   notify_debug
*
*@desc   This function prints out debug information about
the Notify module.
*
*@modif  None
*==========================================================
*/

signed long int notify_debug(IN void  *handle)
{
	signed long int status = NOTIFY_SOK ;
	gt_1trace(notify_debugmask, GT_ENTER, "notify_debug", handle) ;

	BUG_ON(notify_is_init != true) ;
	BUG_ON(handle == NULL) ;

	gt_1trace(notify_debugmask, GT_5CLASS, "notify_debug", status) ;

	return status ;
}
EXPORT_SYMBOL(notify_debug);
#endif /*defined(NOTIFY_DEBUG) */


/**--------------------------------------------------------
*@func   _notify_find_driver
*
*@desc   Find a driver by name in the drivers array and
return handle to the
*driver object.
*
*@modif  None
*-----------------------------------------------------------
*/

signed long int _notify_find_driver(IN  char *driver_name,
		OUT void  **handle)
{
	signed long int       status = NOTIFY_ENOTFOUND ;
	struct notify_driver_handle *drv_handle ;
	unsigned short int              i ;
	gt_2trace(notify_debugmask, GT_ENTER,
		"_notify_find_driver", driver_name, handle) ;

	BUG_ON(driver_name == NULL) ;
	BUG_ON(handle == NULL) ;

	*handle = NULL ;
	for (i = 0 ; i < notify_state_obj.maxDrivers ; i++) {
		drv_handle = &(notify_state_obj.drivers[i]) ;
	/*The function table pointer is used to define whether any driver
	 *is registered in this slot. It is set to TRUE when slot is reserved,
	 *but not yet filled.
	 */
		if ((drv_handle->fn_table != NULL)
				&& (drv_handle->fn_table !=
				(struct notify_interface *) TRUE)) {

			if (strncmp(driver_name,
					drv_handle->name, NOTIFY_MAX_NAMELEN)
					== 0) {
				/*Found the driver. */
				*handle = (void *) drv_handle;
				status = NOTIFY_SOK ;
				break ;
			}
		}
	}

	DBC_ensure((NOTIFY_SUCCEEDED(status) && (*handle != NULL))
			|| (NOTIFY_FAILED(status)    && (*handle == NULL))) ;

	gt_1trace(notify_debugmask, GT_5CLASS, "_notify_find_driver", status) ;

	return status ;
}
EXPORT_SYMBOL(_notify_find_driver);

/**----------------------------------------------------------------------------
*@func   _notify_is_support_proc
*
*@desc   Check if specified processor ID is supported by the Notify driver.
*
*@modif  None
*----------------------------------------------------------------------------
*/

short int _notify_is_support_proc(IN  struct notify_driver_handle *drv_handle,
				IN  unsigned long int  proc_id)
{
	short int                 found = FALSE ;
	struct notify_driver_attrs *attrs ;
	unsigned short int        i ;
	gt_2trace(notify_debugmask, GT_ENTER,
		"_notify_is_support_proc", drv_handle, proc_id) ;

	BUG_ON(drv_handle == NULL) ;

	attrs = &(drv_handle->attrs) ;
	for (i = 0 ; i < attrs->numProc ; i++) {
		if (attrs->proc_info[i].proc_id == proc_id) {
			found = TRUE ;
			break ;
		}
	}

	gt_1trace(notify_debugmask, GT_1CLASS,
			"isSupportedProc[%d]", found) ;

	gt_0trace(notify_debugmask, GT_5CLASS, "_notify_is_support_proc") ;

	return found ;
}
EXPORT_SYMBOL(_notify_is_support_proc);




