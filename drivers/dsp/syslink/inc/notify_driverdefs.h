/*
 * notify_driverdefs.h
 *
 * Notify driver support for OMAP Processors.
 *
 * Copyright (C) 2008-2009 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


#if !defined NOTIFYDRIVERDEFS_H
#define NOTIFYDRIVERDEFS_H

/*  ----------------------------------- Notify */
#include <notifyerr.h>
#include <notifytypes.h>
#include <notifydefs.h>

#define MAX_PROCESSORS 2
/*
 *  const  NOTIFY_SYSTEM_KEY
 *
 *  desc   This key must be provided as the upper 16 bits of the event_no when
 *          registering for an event, if any reserved event numbers are to be
 *          used.
 *
 */

#define NOTIFY_SYSTEM_KEY    (unsigned short int) 0xC1D2

/*
 *  const  NOTIFY_SYSTEM_KEY_MASK
 *
 *  desc   Mask to check for system key.
 *
 */

#define NOTIFY_SYSTEM_KEY_MASK  (unsigned short int) 0xFFFF0000

/*
 *  const  NOTIFY_EVENT_MASK
 *
 *  desc   Mask to check for event ID.
 *
 */

#define NOTIFY_EVENT_MASK      (unsigned short int) 0x0000FFFF


/*
 *  name   unsigned long int
 *
 *  desc   This type defines the Notify ID used to identify the position of the
 *          Notify driver within the Notify module table.
 *
 */


/*
 *  name   Notify_DriverProcInfo
 *
 *  desc   This structure defines information for all processors supported by
 *          the Notify driver.
 *          An instance of this object is provided for all processors handled by
 *          the Notify driver, when registering itself with the Notify module.
 *
 *  field  max_events
 *              Maximum number of events supported by the Notify driver for
 *              communication with this processor.
 *  field .reserved_events
 *              Maximum number of events reserved for usage only by the system
 *              (kernel-side APIs) for this processor. The events from 0 to
 *              .reserved_events -1) are considered as reserved by the Notify
 *              module.
 *  field  event_priority
 *              Indicates whether event prioritization is supported for this
 *              processor.
 *  field  payload_size
 *              Size of payload supported for this processor. Specify 0 if
 *              payload is not supported at all.
 *  field  proc_id
 *              ID of this processor
 *
 *  see    struct notify_driver_attrs, notify_register_driver ()
 *
 */
struct Notify_DriverProcInfo {
	unsigned long int       max_events ;
	unsigned long int      reserved_events ;
	short int         event_priority ;
	unsigned long int       payload_size ;
	unsigned long int proc_id ;
};

/*
 *  name   struct notify_driver_attrs
 *
 *  desc   This structure defines the structure for specifying Notify driver
 *          attributes to the Notify module.
 *          This structure provides information about the Notify driver to the
 *          Notify module. The information is used by the Notify module mainly
 *          for parameter validation. It may also be used by the Notify module
 *          to take appropriate action if required based on the characteristics
 *          of the Notify driver.
 *
 *  field  numProc
 *              Number of processors handled by this Notify driver
 *  field  proc_info
 *              Array of processor information objects for processors supported
 *              by this Notify driver
 *
 *  see    notify_register_driver ()
 *
 */
struct notify_driver_attrs {
	unsigned long int         numProc ;
	struct Notify_DriverProcInfo proc_info[MAX_PROCESSORS] ;
};

/*
 *  name   Notify_DriverObject, struct notify_driver_handle *
 *
 *  desc   This structure defines the Notify driver object and handle used
 *          internally to contain all information required for the Notify driver
 *          This object contains all information for the Notify module to be
 *          able to identify and interact with the Notify driver.
 *
 *  field  isInit
 *              indicates whether the Notify driver has been initialized
 *  field  name
 *              Name of the driver
 *  field  notifyId
 *              ID of the Notify driver. This is the position in the internal
 *              table maintained by the Notify module, and is used for
 *              registering and un-registering the Notify driver.
 *  field  fn_table
 *              Function table pointer for the Notify driver
 *  field  attrs
 *              Information about the registered driver
 *  field  driver_object
 *              This is a driver-specific object and is defined by the Notify
 *              driver implementer.
 *
 *  see    notify_register_driver ()
 *
 */
struct notify_driver_handle {
	short int      isInit ;
	char           name[NOTIFY_MAX_NAMELEN] ;
	unsigned long int         notifyId ;
	struct notify_interface *fn_table ;
	struct notify_driver_attrs   attrs ;
	void *driver_object ;
};


/*
 *  name   Notify_DriverObject, struct notify_driver_handle *
 *
 *  desc   Forward declaration of Notify_DriverObject and definition of handle
 *          to Notify driver.
 *
 */


/*
 *  name   FnNotifyDriverInit
 *
 *  desc   This type defines the initialization function for the Notify driver.
 *          This function gets called internally from the notify_driver_init ()
 *          API. The notify_driver_init () function checks for a registered
 *          Notify driver of the specified name and passes on the initialization
 *          request into the Notify driver, if found.
 *
 *  arg    driver_name
 *              Name of the Notify driver to be initialized.
 *  arg    config
 *              Configuration information for the Notify driver. This contains
 *              generic information as well as information specific to the type
 *              of Notify driver, as defined by the driver provider.
 *  arg    driver_object
 *              Location to receive the pointer to the Notify-driver specific
 *              object.
 *
 *  ret    NOTIFY_SOK
 *              The Notify driver has been successfully initialized
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_EPOINTER
 *              Invalid pointer passed
 *          NOTIFY_ECONFIG
 *              Invalid configuration information passed
 *          NOTIFY_EINVALIDARG
 *              Invalid arguments
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  Notify module must have been initialized before this call
 *          driver_name must be a valid pointer
 *          config must be valid
 *          driver_object must be a valid pointer.
 *
 *  leave  On success, the driver must be initialized.
 *
 *  see    struct notify_interface, notify_driver_init ()
 *
 */
typedef signed long int (*FnNotifyDriverInit) (char *driver_name,
				struct notify_config *config,
				void **driver_object) ;

/*
 *  name   FnNotifyDriverExit
 *
 *  desc   This type defines the finalization function for the Notify driver.
 *          This function gets called internally from the notify_driver_exit ()
 *          API. The notify_driver_exit () function passes on the initialization
 *          request into the Notify driver identified by the Notify Handle
 *          passed.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *
 *  ret    NOTIFY_SOK
 *              The Notify driver has been successfully initialized
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EINVALIDARG
 *              Invalid arguments
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module and driver must be initialized before calling
 *          this function.
 *          handle must be a valid Notify driver handle
 *
 *  leave  On success, the driver must be initialized.
 *
 *  see    struct notify_interface, notify_driver_exit ()
 *
 */
typedef signed long int (*FnNotifyDriverExit) (
			struct notify_driver_handle  *handle) ;

/*
 *  func   FnNotifyRegisterEvent
 *
 *  desc   This type defines the function to register a callback for an event
 *          with the Notify driver.
 *          This function gets called internally from the notify_register_event
 *          API. The notify_register_event () function passes on the
 *          initialization request into the Notify driver identified by the
 *          Notify Handle passed.
 *
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    proc_id
 *              ID of the processor from which notifications can be received on
 *              this event.
 *  arg    event_no
 *              Event number to be registered.
 *  arg    fn_notify_cbck
 *              Callback function to be registered for the specified event.
 *  arg    cbck_arg
 *              Optional argument to the callback function to be registered for
 *              the specified event. This argument shall be passed to each
 *              invocation of the callback function.
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module and driver must be initialized before calling
 *          this function.
 *          handle must be a valid Notify driver handle
 *          fn_notify_cbck must be a valid pointer.
 *          The event must be supported by the Notify driver.
 *
 *  leave  On success, the event must be registered with the Notify driver
 *
 *  see    struct notify_interface, notify_register_event ()
 *
 */
typedef signed long int (*FnNotifyRegisterEvent) (
				struct notify_driver_handle *handle,
				unsigned long int        proc_id,
				unsigned long int        event_no,
				fn_notify_cbck   fn_notify_cbck,
				void *cbck_arg) ;

/*
 *  func   FnNotifyUnregisterEvent
 *
 *  desc   This type defines the function to unregister a callback for an event
 *          with the Notify driver.
 *          This function gets called internally from the notify_unregister_
 *          API. The notify_unregister_event () function passes on the
 *          initialization request into the Notify driver identified by the
 *          Notify Handle passed.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    proc_id
 *              ID of the processor from which notifications can be received on
 *              this event.
 *  arg    event_no
 *              Event number to be un-registered.
 *  arg    fn_notify_cbck
 *              Callback function to be un-registered for the specified event.
 *  arg    cbck_arg
 *              Optional argument to the callback function to be un-registered
 *              for the specified event.
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_ENOTFOUND
 *              Specified event registration was not found
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module and driver must be initialized before calling this
 *          function.
 *          handle must be a valid Notify driver handle
 *          fn_notify_cbck must be a valid pointer.
 *          The event must be supported by the Notify driver.
 *          The event must have been registered with the Notify driver earlier.
 *
 *  leave  On success, the event must be unregistered with the Notify driver.
 *
 *  see    struct notify_interface, notify_unregister_event ()
 *
 */
typedef signed long int (*FnNotifyUnregisterEvent) (
				struct notify_driver_handle *handle,
				unsigned long int        proc_id,
				unsigned long int        event_no,
				fn_notify_cbck        fn_notify_cbck,
				void *cbck_arg) ;

/*
 *  func   FnNotifySendEvent
 *
 *  desc   This type defines the function to send a notification event to the
 *          registered users for this notification on the specified processor.
 *          This function gets called internally from the notify_sendevent ()
 *          API. The notify_sendevent () function passes on the initialization
 *          request into the Notify driver identified by the Notify Handle
 *          passed.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    proc_id
 *              ID of the processor to which the notification is to be sent.
 *  arg    event_no
 *              Event number to be used.
 *  arg    payload
 *              Data to be sent along with the event.
 *  arg    wait_clear
 *              Indicates whether the function should wait for previous event
 *              to be cleared, or sending single event is sufficient without
 *              payload.
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EDRIVERINIT
 *              Remote Notify driver is not setup to receive events.
 *          NOTIFY_ENOTREADY
 *              Other side is not ready to receive an event. This can be due to
 *              one of two reasons:
 *              1. No client is registered for this event on the other side
 *              2. Other side has disabled the event
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module and driver must be initialized before calling this
 *          function.
 *          handle must be a valid Notify driver handle
 *          The event must be supported by the Notify driver.
 *
 *  leave  On success, the event must be sent to the specified processor.
 *
 *  see    struct notify_interface, notify_sendevent ()
 *
 */
typedef signed long int (*FnNotifySendEvent) (
				struct notify_driver_handle *handle,
				unsigned long int        proc_id,
				unsigned long int              event_no,
				unsigned long int              payload,
				short int                wait_clear) ;

/*
 *  func   FnNotifyDisable
 *
 *  desc   This type defines the function to disable all events for this
 *          Notify driver.
 *          This function gets called internally from the notify_disable () API.
 *          The notify_disable () function passes on the disable call into all
 *          Notify drivers registered with it.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *
 *  ret    flags
 *              Flags to be provided when FnNotifyRestore is called.
 *
 *  enter  The Notify module and driver must be initialized before calling this
 *          function.
 *          handle must be a valid Notify driver handle
 *
 *  leave  On success, all events for the Notify driver must be disabled
 *
 *  see    struct notify_interfacej, notify_disable ()
 *
 */
typedef void * (*FnNotifyDisable) (struct notify_driver_handle *handle) ;

/*
 *  func   FnNotifyRestore
 *
 *  desc   This type defines the function to restore the Notify driver to the
 *          state before the last disable was called.
 *          This function gets called internally from the notify_restore () API.
 *          The notify_restore () function passes on the restore call into all
 *          Notify drivers registered with it.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    flags
 *              Flags returned from the call to last FnNotifyDisable in order
 *              to restore the Notify driver to the state before the last
 *              disable call.
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EPOINTER
 *              Invalid pointer passed
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module and driver must be initialized before calling this
 *          function.
 *          handle must be a valid Notify driver handle
 *          flags must be the same as what was returned from the previous
 *          FnNotifyDisable call.
 *
 *  leave  On success, all events for the Notify driver must be restored to
 *          the state as indicated by the provided flags.
 *
 *  see    struct notify_interface, notify_restore ()
 *
 */
typedef signed long int (*FnNotifyRestore) (
				struct notify_driver_handle *handle,
				void *flags) ;

/*
 *  func   FnNotifyDisableEvent
 *
 *  desc   This type defines the function to disable a specific event for this
 *          Notify driver.
 *          This function gets called internally from the notify_disabl
 *          API. The notify_disable_event () function passes on the call into
 *          Notify driver identified by the Notify handle passed.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    proc_id
 *              ID of the processor.
 *  arg    event_no
 *              Event number to be disabled.
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module and driver must be initialized before calling this
 *          function.
 *          handle must be a valid Notify driver handle
 *          The event must be supported by the Notify driver.
 *
 *  leave  On success, the event must be disabled.
 *
 *  see    struct notify_interface, notify_disable_event ()
 *
 */
typedef signed long int (*FnNotifyDisableEvent) (
			struct notify_driver_handle *handle,
			unsigned long int        proc_id,
			unsigned long int       event_no) ;

/*
 *  func   FnNotifyEnableEvent
 *
 *  desc   This type defines the function to enable a specific event for this
 *          Notify driver.
 *          This function gets called internally from the notify_enable_event ()
 *          API. The notify_enable_event () function passes on the call into the
 *          Notify driver identified by the handle passed.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    proc_id
 *              ID of the processor.
 *  arg    event_no
 *              Event number to be enabled.
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module and driver must be initialized before calling this
 *          function.
 *          handle must be a valid Notify driver handle
 *          The event must be supported by the Notify driver.
 *
 *  leave  On success, the event must be enabled.
 *
 *  see    struct notify_interface, notify_enable_event ()
 *
 */
typedef signed long int (*FnNotifyEnableEvent) (
				struct notify_driver_handle *handle,
				unsigned long int        proc_id,
				unsigned long int        event_no) ;

#if defined NOTIFY_DEBUG
/*
 *  func   FnNotifyDebug
 *
 *  desc   This type defines the function to print debug information for the
 *          Notify module and driver.
 *          This function gets called internally from the notify_debug () API.
 *          The notify_debug () function first prints Notify module specific
 *          information and then passes on the initialization request into the
 *          Notify driver identified by the Notify Handle passed.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module and driver must be initialized before calling this
 *          function.
 *          handle must be a valid Notify driver handle
 *
 *  leave  None.
 *
 *  see    struct notify_interface, notify_debug ()
 *
 */
typedef signed long int (*FnNotifyDebug) (
			struct notify_driver_handle *handle) ;

#endif /* defined (NOTIFY_DEBUG) */


/*
 *  name   struct notify_interface
 *
 *  desc   This structure defines the function table interface for the Notify
 *          driver.
 *          This function table interface must be implemented by each Notify
 *          driver and registered with the Notify module.
 *
 *  field  notifyDriverInit
 *              Function to initialize the Notify driver
 *  field  notify_drv_exit
 *              Function to finalize the Notify driver
 *  field  notify_register_event
 *              Function to register a callback for an event with the Notify
 *              driver
 *  field  notify_unregister_event
 *              Function to un-register a callback for an event with the Notify
 *              driver
 *  field  notify_sendevent
 *              Function to send a notification event to the specified processor
 *              using the Notify driver
 *  field  notify_disable
 *              Function to disable all events for this Notify driver
 *  field  notify_restore
 *              Function to restore Notify driver to the state before the last
 *              disable call
 *  field  notify_disable_event
 *              Function to disable a specific event for this Notify driver
 *  field  notify_enable_event
 *              Function to enable a specific event for this Notify driver
 *  field  notifyDebug
 *              Function to print debug information for the Notify driver.
 *              Only defined for debug build
 *
 *  see    notify_register_driver ()
 *
 */
struct notify_interface {
	FnNotifyDriverInit      notifyDriverInit ;
	FnNotifyDriverExit      notify_drv_exit ;
	FnNotifyRegisterEvent   notify_register_event ;
	FnNotifyUnregisterEvent notify_unregister_event ;
	FnNotifySendEvent       notify_sendevent ;
	FnNotifyDisable         notify_disable ;
	FnNotifyRestore         notify_restore ;
	FnNotifyDisableEvent    notify_disable_event ;
	FnNotifyEnableEvent     notify_enable_event ;
#if defined NOTIFY_DEBUG
	FnNotifyDebug           notifyDebug ;
#endif /* defined (NOTIFY_DEBUG) */
};


#endif  /* !defined (NOTIFYDRIVERDEFS_H) */
