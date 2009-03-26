
/** ============================================================================
*  @file   notify.h
*
*  @path   $(NOTIFY)/include
*
*  @desc   Defines interfaces exposed by Notify module
*
*  @ver    1.00.00.01
*  ============================================================================
*  Copyright (c) Texas Instruments Incorporated 2002-2008
*
*  Use of this software is controlled by the terms and conditions found in the
*  license agreement under which this software has been supplied or provided.
*  ============================================================================
*/


#if !defined NOTIFY_H
#define NOTIFY_H


/*  ----------------------------------- IPC */
#include <ipctypes.h>

/*  ----------------------------------- Notify */
#include <notifydefs.h>
#include <notifyerr.h>


#if defined  __cplusplus
EXTERN "C" {
#endif /* defined __cplusplus */


/** ==========================================================================
*  @func   Notify_init
*
*  @desc   This function initializes the Notify module.
*          The user must call this function before invoking any other API from
*          the Notify module.
*
*  @arg    attrs
*              Attributes for initialization of the Notify module.
*
*  @ret    NOTIFY_SOK
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
*  @enter  The attrs parameter must be valid.
*
*  @leave  On success, the Notify module must be initialized.
*
*  @see    struct Notify_Attrs, Notify_exit ()
*  ===========================================================================
*/

signed long int
Notify_init(IN struct Notify_Attrs *attrs) ;


/** ===========================================================================
*  @func   Notify_exit
*
*  @desc   This function finalizes the Notify module.
*          Once this function is called, the user must not invoke any other API
*          from the Notify module.
*
*  @arg    None
*
*  @ret    NOTIFY_SOK
*              The Notify module has been successfully finalized
*          NOTIFY_SEXIT
*              This is the last call to Notify_exit and has resulted in
*              finalizing the Notify module
*          NOTIFY_EINIT
*              The Notify module was not initialized
*          NOTIFY_EMEMORY
*              Operation failed due to a memory error
*          NOTIFY_EFAIL
*              General failure.
*
*  @enter  Notify module must have been initialized before this call
*
*  @leave  On success, the Notify module must be finalized.
*
*  @see    Notify_init ()
*  ============================================================================
*/

signed long int
Notify_exit(void) ;


/** ============================================================================
*  @func   Notify_driverInit
*
*  @desc   This function initializes and configures the specified Notify driver
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
*  @arg    driverName
*              Name of the Notify driver to be initialized.
*  @arg    config
*              Configuration information for the Notify driver.
*              This contains generic information as well as information
*              specific to the type of Notify driver, as defined by the driver
*              provider.
*  @arg    handle
*              Location to receive the handle to the Notify driver.
*
*  @ret    NOTIFY_SOK
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
*  @enter  Notify module must have been initialized before this call
*          driverName must be a valid pointer
*          config must be valid
*          handle must be a valid pointer.
*
*  @leave  On success, the driver must be initialized.
*
*  @see    struct Notify_Config, Notify_driverExit ()
*  ============================================================================
*/

signed long int
Notify_driverInit(IN  char *driverName,
IN  struct Notify_Config *config,
OUT void **handle) ;


/** ============================================================================
*  @func   Notify_driverExit
*
*  @desc   This function finalizes the specified Notify driver.
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
*              This is the last call to Notify_driverExit and has resulted in
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
*  @see    Notify_driverInit ()
*  ============================================================================
*/

signed long int
Notify_driverExit(IN void *handle) ;


/** ============================================================================
*  @func   Notify_registerEvent
*
*  @desc   This function registers a callback for a specific event with the
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
*  @arg    handle
*              Handle to the Notify driver
*  @arg    procId
*              ID of the processor from which notifications can be received on
*              this event.
*  @arg    eventno
*              Event number to be registered.
*  @arg    fnNotifyCbck
*              Callback function to be registered for the specified event.
*  @arg    cbckArg
*              Optional argument to the callback function to be registered for
*              the specified event. This argument shall be passed to each
*              invocation of the callback function.
*
*  @ret    NOTIFY_SOK
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
*  @enter  The Notify module and driver must be initialized before calling
*          this function.
*          handle must be a valid Notify driver handle
*          fnNotifyCbck must be a valid pointer.
*          The event must be supported by the Notify driver.
*
*  @leave  On success, the event must be registered with the Notify module
*
*  @see    Notify_unregisterEvent ()
*  ============================================================================
*/

signed long int
Notify_registerEvent(IN     void *handle,
IN     unsigned long int  procId,
IN     unsigned long int  eventno,
IN     FnNotifyCbck  fnNotifyCbck,
IN OPT void *cbckArg) ;


/** ============================================================================
*  @func   Notify_unregisterEvent
*
*  @desc   This function un-registers the callback for the specific event with
*          the Notify module.
*          Multiple callbacks may be registered for a single event. It is
*          possible that the same callback function may be registered from
*          multiple sources, with the only difference being the callback
*          argument. Hence the callback argument must also be provided to the
*          Notify_unregisterEvent () API in order for the Notify module to be
*          able to differentiate between the registered callbacks.
*
*  @arg    handle
*              Handle to the Notify driver
*  @arg    procId
*              ID of the processor from which notifications can be received on
*              this event.
*  @arg    eventno
*              Event number to be un-registered.
*  @arg    fnNotifyCbck
*              Callback function to be un-registered for the specified event.
*  @arg    cbckArg
*              Optional argument to the callback function to be un-registered
*              for the specified event.
*
*  @ret    NOTIFY_SOK
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
*  @enter  The Notify module and driver must be initialized before calling this
*          function.
*          handle must be a valid Notify driver handle
*          fnNotifyCbck must be a valid pointer.
*          The event must be supported by the Notify driver.
*          The event must have been registered with the Notify driver earlier.
*
*  @leave  On success, the event must be unregistered with the Notify module.
*
*  @see    Notify_registerEvent ()
*  ============================================================================
*/

signed long int
Notify_unregisterEvent(IN     void *handle,
IN     unsigned long int  procId,
IN     unsigned long int   eventno,
IN     FnNotifyCbck  fnNotifyCbck,
IN OPT void *cbckArg) ;



/** ============================================================================
*  @func   Notify_sendEvent
*
*  @desc   This function sends a notification to the specified event.
*
*  @arg    handle
*              Handle to the Notify driver
*  @arg    procId
*              ID of the processor to which the notification is to be sent.
*  @arg    eventno
*              Event number to be used.
*  @arg    payload
*              Data to be sent along with the event.
*  @arg    waitClear
*              Indicates whether the function should wait for previous event
*              to be cleared, or sending single event is sufficient without
*              payload.
*
*  @ret    NOTIFY_SOK
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
*  @enter  The Notify module and driver must be initialized before calling this
*          function.
*          handle must be a valid Notify driver handle
*          The event must be supported by the Notify driver.
*
*  @leave  On success, the event must be sent to the specified processor.
*
*  @see    Notify_registerEvent ()
*  ============================================================================
*/

signed long int
Notify_sendEvent(IN void *handle,
IN unsigned long int  procId,
IN unsigned long int        eventno,
IN unsigned long int        payload,
IN short int          waitClear) ;


/** ============================================================================
*  @func   Notify_disable
*
*  @desc   This function disables all events. This is equivalent to global
*          interrupt disable, however restricted within interrupts handled by
*          the Notify module. All callbacks registered for all events are
*          disabled with this API. It is not possible to disable a specific
*          callback.
*          The Notify module passes on this function to all drivers registered
*          with it. It provides the functionality of disabling all interrupts
*          used by IPC for the Notify module for achieving protection/
*          reentrancy.
*
*  @arg    None
*
*  @ret    flags
*              Flags to be provided when Notify_restore () is called.
*
*  @enter  The Notify module must be initialized before calling this function.
*
*  @leave  On success, all events for all Notify drivers must be disabled
*
*  @see    Notify_restore ()
*  ============================================================================
*/

void *
Notify_disable(void) ;


/** ============================================================================
*  @func   Notify_restore
*
*  @desc   This function restores the Notify module to the state before the
*          last Notify_disable () was called. This is equivalent to global
*          interrupt restore, however restricted within interrupts handled by
*          the Notify module. All callbacks registered for all events as
*          specified in the flags are enabled with this API. It is not possible
*          to enable a specific callback.
*          The Notify module passes on this function to all drivers registered
*          with it. It provides the functionality of restoring all interrupts
*          used by IPC for the Notify module for achieving protection/
*          reentrancy.
*
*  @arg    flags
*              Flags returned from the call to last Notify_disable () in order
*              to restore the Notify module to the state before the last
*              disable call.
*
*  @ret    NOTIFY_SOK
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
*  @enter  The Notify module must be initialized before calling this function.
*          flags must be the same as what was returned from the previous
*          Notify_disable () call.
*
*  @leave  On success, all events for all Notify drivers must be restored to
*          the state as indicated by the provided flags.
*
*  @see    Notify_disable ()
*  ============================================================================
*/

signed long int
Notify_restore(IN void *flags) ;


/** ============================================================================
*  @func   Notify_disableEvent
*
*  @desc   This function disables a specific event. All callbacks registered
*          for the specific event are disabled with this API. It is not
*          possible to disable a specific callback.
*          Applications/frameworks/IPCs can make use of this function to
*          provide protection/reentrancy for their event without affecting
*          others in the system.
*
*  @arg    handle
*              Handle to the Notify driver
*  @arg    procId
*              ID of the processor.
*  @arg    eventno
*              Event number to be disabled.
*
*  @ret    NOTIFY_SOK
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
*  @enter  The Notify module and driver must be initialized before calling this
*          function.
*          handle must be a valid Notify driver handle
*          The event must be supported by the Notify driver.
*
*  @leave  On success, the event must be disabled.
*
*  @see    Notify_enableEvent ()
*  ============================================================================
*/

signed long int
Notify_disableEvent(IN void *handle,
IN unsigned long int  procId,
IN unsigned long int        eventno) ;


/** ============================================================================
*  @func   Notify_enableEvent
*
*  @desc   This function enables a specific event. All callbacks registered for
*          this specific event are enabled with this API. It is not possible to
*          enable a specific callback.
*          Applications/frameworks/IPCs can make use of this function to
*          provide protection/reentrancy for their event without affecting
*          others in the system.
*          At initialization time, all events are automatically enabled by
*          default.
*
*  @arg    handle
*              Handle to the Notify driver
*  @arg    procId
*              ID of the processor.
*  @arg    eventno
*              Event number to be enabled.
*
*  @ret    NOTIFY_SOK
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
*  @enter  The Notify module and driver must be initialized before calling this
*          function.
*          handle must be a valid Notify driver handle
*          The event must be supported by the Notify driver.
*
*  @leave  On success, the event must be enabled.
*
*  @see    Notify_disableEvent ()
*  ============================================================================
*/

signed long int
Notify_enableEvent(IN void *handle,
IN unsigned long int  procId,
IN unsigned long int        eventNo) ;


#if defined NOTIFY_DEBUG
/** ============================================================================
*  @func   Notify_debug
*
*  @desc   This function prints out debug information about the Notify module.
*
*  @arg    handle
*              Handle to the Notify driver
*
*  @ret    NOTIFY_SOK
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
*  @enter  The Notify module and driver must be initialized before calling this
*          function.
*          handle must be a valid Notify driver handle
*
*  @leave  None.
*
*  @see    None.
*  ============================================================================
*/

signed long int
Notify_debug(IN void *handle) ;
#endif /* defined NOTIFY_DEBUG */


#if defined __cplusplus
}
#endif /* defined __cplusplus */


#endif  /* !defined NOTIFY_H */
