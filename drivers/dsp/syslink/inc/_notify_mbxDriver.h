
/** ============================================================================
*  @file   notify_mbxDriver.h
*
*  @path   $(NOTIFY)/dsp/src/Notify/drivers
*
*  @desc   Defines the interface for the Notify driver using Shared Memory and
*          interrupts to communicate with the remote processor.
*
*  @ver    1.00.00.01
*  ============================================================================
*  Copyright (c) Texas Instruments Incorporated 2002-2008
*
*  Use of this software is controlled by the terms and conditions found in the
*  license agreement under which this software has been supplied or provided.
*  ============================================================================
*/


#if !defined _NOTIFY_MBXDRIVER_H_
#define _NOTIFY_MBXDRIVER_H_


/*  ----------------------------------- IPC */
#include <gpptypes.h>
#include <ipctypes.h>

/*  ----------------------------------- Notify       */
#include <notifydefs.h>
#include <notifyerr.h>

/*  ----------------------------------- Notify driver */
#include <notify_driverdefs.h>
#include <global_var.h>


#if defined __cplusplus
extern "C" {
#endif /* defined __cplusplus */


/** ============================================================================
*  @name   struct NotifyShmDrv_EventEntry
*
*  @desc   Defines the structure of event entry within the event chart.
*          Each entry contains occured event-specific information.
*          This is shared between GPP and DSP.
*
*  @field  flag
*              Indicating event is present or not.
*  @field  payload
*              Variable containing data associated with each occured event.
*  @field  reserved
*              Reserved field to contain additional information about the
*              event entry.
*  @field  padding
*              Padding.
*  ============================================================================
*/
struct NotifyShmDrv_EventEntry {
REG unsigned long int  flag     ;
REG unsigned long int  payload  ;
REG unsigned long int  reserved ;
/*ADD_PADDING(padding, NOTIFYSHMDRV_EVENT_ENTRY_PADDING)*/
};

/** ============================================================================
*  @name   struct NotifyShmDrv_EventRegMask
*
*  @desc   Defines the mask indicating registered events on the processor.
*          This is shared between GPP and DSP.
*
*  @field  mask
*              Indicating event is registered.
*  @field  enableMask
*              Indicates event is enabled.
*  @field  padding
*              Padding.
*  ============================================================================
*/
struct NotifyShmDrv_EventRegMask {
REG unsigned long int mask ;
REG unsigned long int enableMask ;
/*ADD_PADDING (padding, IPC_64BIT_PADDING)*/
};

/** ============================================================================
*  @name   NotifyShmDrv_EventRegEntry
*
*  @desc   Defines the structure of event registration entry within the Event
*          Registration Chart.
*          Each entry contains registered event-specific information.
*
*  @field  regEventNo
*              Index into the event chart, indicating the registered event.
*  @field  reserved
*              Reserved field to contain additional information about the
*              registered event.
*  ============================================================================
*/
struct NotifyShmDrv_EventRegEntry {
unsigned long int     regEventNo ;
unsigned long int     reserved ;
};

/** ============================================================================
*  @name   struct NotifyShmDrv_ProcCtrl
*
*  @desc   Defines the NotifyShmDrv control structure, which contains all
*          information for one processor.
*          This structure is shared between the two processors.
*
*  @field  otherEventChart
*              Address of event chart for the other processor.
*  @field  selfEventChart
*              Address of event chart for this processor.
*  @field  recvInitStatus
*              Indicates whether the driver has been initialized, and is ready
*              to receive events on this processor. If the driver does not
*              support events from other processor to this processor, this flag
*              will always indicate not-initialized status.
*  @field  sendInitStatus
*              Indicates whether the driver has been initialized, and is ready
*              to send events on this processor. If the driver does not
*              support events from this processor to other processor, this flag
*              will always indicate not-initialized status.
*  @field  padding
*              Padding for alignment.
*  @field  selfRegMask
*              Registration mask.
*  ============================================================================
*/
struct NotifyShmDrv_ProcCtrl {
struct NotifyShmDrv_EventEntry *selfEventChart ;
struct NotifyShmDrv_EventEntry *otherEventChart ;
unsigned long int                        recvInitStatus ;
unsigned long int                        sendInitStatus ;
/*ADD_PADDING(padding, NOTIFYSHMDRV_CTRL_PADDING)*/

struct NotifyShmDrv_EventRegMask     regMask ;
};

/** ============================================================================
*  @name   NotifyShmDrv_Ctrl
*
*  @desc   Defines the NotifyShmDrv control structure, which contains all
*          information shared between the two connected processors
*          This structure is shared between the two processors.
*
*  @field  otherProcCtrl
*              Control structure for other processor
*  @field  selfProcCtrl
*              Control structure for self processor
*  ============================================================================
*/
struct NotifyShmDrv_Ctrl {
struct NotifyShmDrv_ProcCtrl otherProcCtrl ;
struct NotifyShmDrv_ProcCtrl selfProcCtrl ;
};


/** ============================================================================
*  @name   NotifyMbxDrv_driverInit
*
*  @desc   Initialization function for the Notify shared memory mailbox driver.
*
*  @arg    driverName
*              Name of the Notify driver to be initialized.
*  @arg    config
*              Configuration information for the Notify driver. This contains
*              generic information as well as information specific to the type
*              of Notify driver, as defined by the driver provider.
*  @arg    driverObj
*              Location to receive the pointer to the Notify-driver specific
*              object.
*
*  @ret    NOTIFY_SOK
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
*  @enter  Notify module must have been initialized before this call
*          driverName must be a valid pointer
*          config must be valid
*          driverObj must be a valid pointer.
*
*  @leave  On success, the driver must be initialized.
*
*  @see    Notify_Interface, Notify_driverInit ()
*  ============================================================================
*/

signed long int
NotifyMbxDrv_driverInit(IN  char *driverName,
IN  struct Notify_Config *config,
OUT void **driverObj) ;

/** ============================================================================
*  @name   NotifyMbxDrv_driverExit
*
*  @desc   Finalization function for the Notify driver.
*
*  @arg    handle
*              Handle to the Notify driver
*
*  @ret    NOTIFY_SOK
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
*  @enter  The Notify module and driver must be initialized before calling
*          this function.
*          handle must be a valid Notify driver handle
*
*  @leave  On success, the driver must be initialized.
*
*  @see    Notify_Interface, Notify_driverExit ()
*  ============================================================================
*/

signed long int
NotifyMbxDrv_driverExit(IN struct Notify_DriverHandle *handle) ;

/** ============================================================================
*  @func   NotifyMbxDrv_registerEvent
*
*  @desc   Register a callback for an event with the Notify driver.
*
*  @arg    handle
*              Handle to the Notify driver
*  @arg    procId
*              ID of the processor from which notifications can be received on
*              this event.
*  @arg    eventNo
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
*          NOTIFY_EINVALIDARG
*              Invalid argument
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
*          fnNotifyCbck must be a valid pointer.
*          The event must be supported by the Notify driver.
*
*  @leave  On success, the event must be registered with the Notify driver
*
*  @see    Notify_Interface, Notify_registerEvent ()
*  ============================================================================
*/

signed long int
NotifyMbxDrv_registerEvent(IN struct Notify_DriverHandle *handle,
IN unsigned long int        procId,
IN unsigned long int              eventNo,
IN FnNotifyCbck        fnNotifyCbck,
IN void *cbckArg) ;

/** ============================================================================
*  @func   NotifyMbxDrv_unregisterEvent
*
*  @desc   Unregister a callback for an event with the Notify driver.
*
*  @arg    handle
*              Handle to the Notify driver
*  @arg    procId
*              ID of the processor from which notifications can be received on
*              this event.
*  @arg    eventNo
*              Event number to be un-registered.
*  @arg    fnNotifyCbck
*              Callback function to be un-registered for the specified event.
*  @arg    cbckArg
*              Optional argument to the callback function to be un-registered
*              for the specified event.
*
*  @ret    NOTIFY_SOK
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
*  @enter  The Notify module and driver must be initialized before calling this
*          function.
*          handle must be a valid Notify driver handle
*          fnNotifyCbck must be a valid pointer.
*          The event must be supported by the Notify driver.
*          The event must have been registered with the Notify driver earlier.
*
*  @leave  On success, the event must be unregistered with the Notify driver.
*
*  @see    Notify_Interface, Notify_unregisterEvent ()
*  ============================================================================
*/

signed long int
NotifyMbxDrv_unregisterEvent(IN struct Notify_DriverHandle *handle,
IN unsigned long int        procId,
IN unsigned long int        eventNo,
IN FnNotifyCbck        fnNotifyCbck,
IN void *cbckArg) ;

/** ============================================================================
*  @func   NotifyMbxDrv_sendEvent
*
*  @desc   Send a notification event to the registered users for this
*          notification on the specified processor.
*
*  @arg    handle
*              Handle to the Notify driver
*  @arg    procId
*              ID of the processor to which the notification is to be sent.
*  @arg    eventNo
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
*  @enter  The Notify module and driver must be initialized before calling this
*          function.
*          handle must be a valid Notify driver handle
*          The event must be supported by the Notify driver.
*
*  @leave  On success, the event must be sent to the specified processor.
*
*  @see    Notify_Interface, Notify_sendEvent ()
*  ============================================================================
*/

signed long int
NotifyMbxDrv_sendEvent(IN struct Notify_DriverHandle *handle,
IN unsigned long int        procId,
IN unsigned long int              eventNo,
IN unsigned long int              payload,
IN short int                waitClear) ;

/** ============================================================================
*  @func   NotifyMbxDrv_disable
*
*  @desc   Disable all events for this Notify driver.
*
*  @arg    handle
*              Handle to the Notify driver
*
*  @ret    flags
*              Flags to be provided when NotifyMbxDrv_restore is called.
*
*  @enter  The Notify module and driver must be initialized before calling this
*          function.
*          handle must be a valid Notify driver handle
*
*  @leave  On success, all events for the Notify driver must be disabled
*
*  @see    Notify_Interfacej, Notify_disable ()
*  ============================================================================
*/

void *
NotifyMbxDrv_disable(IN struct Notify_DriverHandle *handle) ;

/** ============================================================================
*  @func   NotifyMbxDrv_restore
*
*  @desc   Restore the Notify driver to the state before the last disable was
*          called.
*
*  @arg    handle
*              Handle to the Notify driver
*  @arg    flags
*            Flags returned from the call to last NotifyMbxDrv_disable in order
*            to restore the Notify driver to the state before the last
*            disable call.
*
*  @ret    NOTIFY_SOK
*              Operation successfully completed
*          NOTIFY_EHANDLE
*              Invalid Notify handle specified
*          NOTIFY_EPOINTER
*              Invalid pointer passed
*          NOTIFY_EFAIL
*              General failure
*
*  @enter  The Notify module and driver must be initialized before calling this
*          function.
*          handle must be a valid Notify driver handle
*          flags must be the same as what was returned from the previous
*          NotifyMbxDrv_disable call.
*
*  @leave  On success, all events for the Notify driver must be restored to
*          the state as indicated by the provided flags.
*
*  @see    Notify_Interface, Notify_restore ()
*  ============================================================================
*/

signed long int
NotifyMbxDrv_restore(IN struct Notify_DriverHandle *handle,
IN void *flags) ;

/** ============================================================================
*  @func   NotifyMbxDrv_disableEvent
*
*  @desc   Disable a specific event for this Notify driver.
*
*  @arg    handle
*              Handle to the Notify driver
*  @arg    procId
*              ID of the processor.
*  @arg    eventNo
*              Event number to be disabled.
*
*  @ret    NOTIFY_SOK
*              Operation successfully completed
*          NOTIFY_EINVALIDARG
*              Invalid argument
*          NOTIFY_EHANDLE
*              Invalid Notify handle specified
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
*  @see    Notify_Interface, Notify_disableEvent ()
*  ============================================================================
*/

signed long int
NotifyMbxDrv_disableEvent(IN struct Notify_DriverHandle *handle,
IN unsigned long int        procId,
IN unsigned long int              eventNo) ;

/** ============================================================================
*  @func   NotifyMbxDrv_enableEvent
*
*  @desc   Enable a specific event for this Notify driver.
*
*  @arg    handle
*              Handle to the Notify driver
*  @arg    procId
*              ID of the processor.
*  @arg    eventNo
*              Event number to be enabled.
*
*  @ret    NOTIFY_SOK
*              Operation successfully completed
*          NOTIFY_EINVALIDARG
*              Invalid argument
*          NOTIFY_EHANDLE
*              Invalid Notify handle specified
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
*  @see    Notify_Interface, Notify_enableEvent ()
*  ============================================================================
*/

signed long int
NotifyMbxDrv_enableEvent(IN struct Notify_DriverHandle *handle,
IN unsigned long int        procId,
IN unsigned long int              eventNo) ;


/** ============================================================================
*  @func   NotifyMbxDrv_debug
*
*  @desc   Print debug information for the Notify driver.
*
*  @arg    handle
*              Handle to the Notify driver
*
*  @ret    NOTIFY_SOK
*              Operation successfully completed
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
*  @see    Notify_Interface, Notify_debug ()
*  ============================================================================
*/

signed long int
NotifyMbxDrv_debug(IN struct Notify_DriverHandle *handle) ;


#if defined __cplusplus
}
#endif /* defined __cplusplus */


#endif  /* !defined _NOTIFY_SHMDRIVER_H_ */

