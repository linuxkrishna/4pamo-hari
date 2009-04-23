/*
 * notify.h
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


#if !defined NOTIFY_H
#define NOTIFY_H


/*  ----------------------------------- Notify */
#include <notifydefs.h>
#include <notifyerr.h>


/*
 *  func   notify_init
 *
 *  desc   This function initializes the Notify module.
 *          The user must call this function before invoking any other API from
 *          the Notify module.
 *
 *  arg    attrs
 *              Attributes for initialization of the Notify module.
 *
 *  ret    NOTIFY_SOK
 *              The Notify module has been successfully initialized
 *          NOTIFY_SALREADYINIT
 *              The Notify module has already been initialized by another
 *              process. This process can now start using the Notify module.
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_EALREADYINIT
 *              Notify module has already been initialized
 *          NOTIFY_ECONFIG
 *              Invalid configuration information passed
 *          NOTIFY_EPOINTER
 *              Invalid pointer passed
 *          NOTIFY_EINVALIDARG
 *              Invalid arguments
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The attrs parameter must be valid.
 *
 *  leave  On success, the Notify module must be initialized.
 *
 *  see    struct notify_attrs, notify_exit ()
 *
 */

signed long int notify_init(struct notify_attrs *attrs) ;


/*
 *  func   notify_exit
 *
 *  desc   This function finalizes the Notify module.
 *          Once this function is called, the user must not invoke any other API
 *          from the Notify module.
 *
 *  arg    None
 *
 *  ret    NOTIFY_SOK
 *              The Notify module has been successfully finalized
 *          NOTIFY_SEXIT
 *              This is the last call to notify_exit and has resulted in
 *              finalizing the Notify module
 *          NOTIFY_EINIT
 *              The Notify module was not initialized
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_EFAIL
 *              General failure.
 *
 *  enter  Notify module must have been initialized before this call
 *
 *  leave  On success, the Notify module must be finalized.
 *
 *  see    notify_init ()
 *
 */

signed long int notify_exit(void) ;


/*
 *  func   notify_driver_init
 *
 *  desc   This function initializes and configures the specified Notify driver
 *          This function is called by the user to initialize a specific Notify
 *          driver. There may be multiple Notify drivers on each processor for
 *          communication with multiple processors. This function must be called
 *          for each Notify driver. One implementation of the Notify driver
 *          (identified by the driver name) may be used for one or more
 *          processors. This completely depends on the implementation of the
 *          Notify driver and the supported hardware.
 *          The handle received after a successful call to this function must be
 *          used for further API calls to this Notify driver.
 *
 *          The Notify driver as specified by the driver name must be registered
 *          with the Notify module before the user calls this API. If this is
 *          not done, this API shall return an error indicating that the
 *          specified driver was not found.
 *
 *  arg    driver_name
 *              Name of the Notify driver to be initialized.
 *  arg    config
 *              Configuration information for the Notify driver.
 *              This contains generic information as well as information
 *              specific to the type of Notify driver, as defined by the driver
 *              provider.
 *  arg    handle
 *              Location to receive the handle to the Notify driver.
 *
 *  ret    NOTIFY_SOK
 *              The Notify driver has been successfully initialized
 *          NOTIFY_SALREADYINIT
 *              The Notify driver has already been initialized by another
 *              process. This process can now start using the Notify driver
 *          NOTIFY_EINIT
 *              The Notify module was not initialized
 *          NOTIFY_ENOTFOUND
 *              The specified name for the Notify driver does not match an
 *              existing registered driver
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
 *          handle must be a valid pointer.
 *
 *  leave  On success, the driver must be initialized.
 *
 *  see    struct notify_config, notify_driver_exit ()
 *
 */

signed long int notify_driver_init(char *driver_name,
		struct notify_config *config,
		void **handle) ;


/*
 *  func   notify_driver_exit
 *
 *  desc   This function finalizes the specified Notify driver.
 *          This function is called by the user to finalize the specified Notify
 *          driver. There may be multiple Notify drivers on each processor for
 *          communication with multiple processors. This function must be called
 *          for each Notify driver. One implementation of the Notify driver
 *          (identified by the driver name) may be used for one or more
 *          processors. This completely depends on the implementation of the
 *          Notify driver and the supported hardware.
 *
 *          Once this API is called, no further calls may be made using the
 *          handle for this Notify driver.
 *
 *  @arg    attrs
 *              Attributes for initialization of the Notify module.
 *
 *  @ret    NOTIFY_SOK
 *              The Notify driver has been successfully finalized
 *          NOTIFY_SEXIT
 *              This is the last call to notify_driver_exit and has resulted in
 *              finalizing the Notify driver
 *          NOTIFY_EINIT
 *              The Notify module was not initialized
 *          NOTIFY_EDRIVERINIT
 *              The Notify driver was not initialized
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  @enter  The Notify module and driver must be initialized before calling
 *          this function.
 *          handle must be a valid Notify driver handle
 *
 *  @leave  On success, the driver must be finalized.
 *
 *  @see    notify_driver_init ()
 *
 */

signed long int notify_driver_exit(void *handle) ;


/*
 *  func   notify_register_event
 *
 *  desc   This function registers a callback for a specific event with the
 *          Notify module.
 *          The user registers their callback function and optional argument to
 *          a specific event associated with the Notify driver. In case a Notify
 *          driver supports multiple processors, the processor ID is also
 *          required, and hence must be always passed to this function.
 *          Multiple callbacks may be registered for a single event. The
 *          callbacks shall be invoked in the order in which they were
 *          registered.
 *          In case the user has provided a callback argument to be associated
 *          with the registration, this argument comes as a parameter to the
 *          callback function in each invocation. This is useful if, for
 *          example, the same callback function is being used from multiple
 *          processes. In such cases, the argument may be a pointer to a state
 *          object relevant to that process, such as a process-specific
 *          semaphore.
 *
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    proc_id
 *              ID of the processor from which notifications can be received on
 *              this event.
 *  arg    eventno
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
 *          NOTIFY_EINIT
 *              The Notify module was not initialized
 *          NOTIFY_EDRIVERINIT
 *              The Notify driver was not initialized
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EINVALIDEVENT
 *              Invalid event number specified
 *          NOTIFY_ENOTSUPPORTED
 *              Unsupported feature requested (e.g. unsupported processor ID for
 *              this Notify driver).
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module and driver must be initialized before calling
 *          this function.
 *          handle must be a valid Notify driver handle
 *          fn_notify_cbck must be a valid pointer.
 *          The event must be supported by the Notify driver.
 *
 *  leave  On success, the event must be registered with the Notify module
 *
 *  see    notify_unregister_event ()
 *
 */

signed long int notify_register_event(void *handle,
		unsigned long int  proc_id,
		unsigned long int  eventno,
		fn_notify_cbck  fn_notify_cbck,
		void *cbck_arg) ;


/*
 *  func   notify_unregister_event
 *
 *  desc   This function un-registers the callback for the specific event with
 *          the Notify module.
 *          Multiple callbacks may be registered for a single event. It is
 *          possible that the same callback function may be registered from
 *          multiple sources, with the only difference being the callback
 *          argument. Hence the callback argument must also be provided to the
 *          notify_unregister_event () API in order for the Notify module to be
 *          able to differentiate between the registered callbacks.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    proc_id
 *              ID of the processor from which notifications can be received on
 *              this event.
 *  arg    eventno
 *              Event number to be un-registered.
 *  arg    fn_notify_cbck
 *              Callback function to be un-registered for the specified event.
 *  arg    cbck_arg
 *              Optional argument to the callback function to be un-registered
 *              for the specified event.
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINIT
 *              The Notify module was not initialized
 *          NOTIFY_EDRIVERINIT
 *              The Notify driver was not initialized
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EINVALIDEVENT
 *              Invalid event number specified
 *          NOTIFY_ENOTFOUND
 *              Specified event registration was not found
 *          NOTIFY_ERESERVEDEVENT
 *              The specified event is reserved for system usage
 *          NOTIFY_ENOTSUPPORTED
 *              Unsupported feature requested (e.g. unsupported processor ID for
 *              this Notify driver).
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
 *  leave  On success, the event must be unregistered with the Notify module.
 *
 *  see    notify_register_event ()
 *
 */

signed long int notify_unregister_event(void *handle,
		unsigned long int  proc_id,
		unsigned long int   eventno,
		fn_notify_cbck  fn_notify_cbck,
		void *cbck_arg) ;



/*
 *  func   notify_sendevent
 *
 *  desc   This function sends a notification to the specified event.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    proc_id
 *              ID of the processor to which the notification is to be sent.
 *  arg    eventno
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
 *          NOTIFY_EINIT
 *              The Notify module was not initialized
 *          NOTIFY_EDRIVERINIT
 *              The Notify driver was not initialized
 *          NOTIFY_ENOTREADY
 *              Other side is not ready to receive an event. This can be due to
 *              one of two reasons:
 *              1. No client is registered for this event on the other side
 *              2. Other side has disabled the event
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EINVALIDEVENT
 *              Invalid event number specified
 *          NOTIFY_ENOTSUPPORTED
 *              Unsupported feature requested (e.g. unsupported processor ID for
 *              this Notify driver).
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
 *  see    notify_register_event ()
 *
 */

signed long int notify_sendevent(void *handle,
		unsigned long int  proc_id,
		unsigned long int        eventno,
		unsigned long int        payload,
		short int          wait_clear) ;


/*
 *  func   notify_disable
 *
 *  desc   This function disables all events. This is equivalent to global
 *          interrupt disable, however restricted within interrupts handled by
 *          the Notify module. All callbacks registered for all events are
 *          disabled with this API. It is not possible to disable a specific
 *          callback.
 *          The Notify module passes on this function to all drivers registered
 *          with it. It provides the functionality of disabling all interrupts
 *          used by IPC for the Notify module for achieving protection/
 *          reentrancy.
 *
 *  arg    None
 *
 *  ret    flags
 *              Flags to be provided when notify_restore () is called.
 *
 *  enter  The Notify module must be initialized before calling this function.
 *
 *  leave  On success, all events for all Notify drivers must be disabled
 *
 *  see    notify_restore ()
 *
 */

void *notify_disable(void) ;


/*
 *  func   notify_restore
 *
 *  desc   This function restores the Notify module to the state before the
 *          last notify_disable () was called. This is equivalent to global
 *          interrupt restore, however restricted within interrupts handled by
 *          the Notify module. All callbacks registered for all events as
 *          specified in the flags are enabled with this API. It is not possible
 *          to enable a specific callback.
 *          The Notify module passes on this function to all drivers registered
 *          with it. It provides the functionality of restoring all interrupts
 *          used by IPC for the Notify module for achieving protection/
 *          reentrancy.
 *
 *  arg    flags
 *              Flags returned from the call to last notify_disable () in order
 *              to restore the Notify module to the state before the last
 *              disable call.
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINIT
 *              The Notify module was not initialized
 *          NOTIFY_EDRIVERINIT
 *              The Notify driver was not initialized
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EPOINTER
 *              Invalid pointer passed
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module must be initialized before calling this function.
 *          flags must be the same as what was returned from the previous
 *          notify_disable () call.
 *
 *  leave  On success, all events for all Notify drivers must be restored to
 *          the state as indicated by the provided flags.
 *
 *  see    notify_disable ()
 *
 */

signed long int notify_restore(void *flags) ;


/*
 *  func   notify_disable_event
 *
 *  desc   This function disables a specific event. All callbacks registered
 *          for the specific event are disabled with this API. It is not
 *          possible to disable a specific callback.
 *          Applications/frameworks/IPCs can make use of this function to
 *          provide protection/reentrancy for their event without affecting
 *          others in the system.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    proc_id
 *              ID of the processor.
 *  arg    eventno
 *              Event number to be disabled.
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINIT
 *              The Notify module was not initialized
 *          NOTIFY_EDRIVERINIT
 *              The Notify driver was not initialized
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EINVALIDEVENT
 *              Invalid event number specified
 *          NOTIFY_ENOTSUPPORTED
 *              Unsupported feature requested (e.g. unsupported processor ID for
 *              this Notify driver, or event disable not supported).
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
 *  see    notify_enable_event ()
 *
 */

signed long int notify_disable_event(void *handle,
		unsigned long int  proc_id,
		unsigned long int        eventno) ;


/*
 *  func   notify_enable_event
 *
 *  desc   This function enables a specific event. All callbacks registered for
 *          this specific event are enabled with this API. It is not possible to
 *          enable a specific callback.
 *          Applications/frameworks/IPCs can make use of this function to
 *          provide protection/reentrancy for their event without affecting
 *          others in the system.
 *          At initialization time, all events are automatically enabled by
 *          default.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *  arg    proc_id
 *              ID of the processor.
 *  arg    eventno
 *              Event number to be enabled.
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINIT
 *              The Notify module was not initialized
 *          NOTIFY_EDRIVERINIT
 *              The Notify driver was not initialized
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EINVALIDEVENT
 *              Invalid event number specified
 *          NOTIFY_ENOTSUPPORTED
 *              Unsupported feature requested (e.g. unsupported processor ID for
 *              this Notify driver, or event disable/enable not supported).
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
 *  see    notify_disable_event ()
 *
 */

signed long int notify_enable_event(void *handle,
		unsigned long int  proc_id,
		unsigned long int        event_no) ;


#if defined NOTIFY_DEBUG
/*
 *  func   notify_debug
 *
 *  desc   This function prints out debug information about the Notify module.
 *
 *  arg    handle
 *              Handle to the Notify driver
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINIT
 *              The Notify module was not initialized
 *          NOTIFY_EDRIVERINIT
 *              The Notify driver was not initialized
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
 *  see    None.
 *
 */

signed long int notify_debug(void *handle) ;

#endif /* defined NOTIFY_DEBUG */


#endif  /* !defined NOTIFY_H */
