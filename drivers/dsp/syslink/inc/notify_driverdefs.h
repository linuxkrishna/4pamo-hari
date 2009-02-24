/** ============================================================================
 *  @file   notify_driverdefs.h
 *
 *  @path   $(NOTIFY)/include
 *
 *  @desc   Defines data types and structures used by Notify driver writers.
 *
 *  @ver    1.00.00.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (NOTIFYDRIVERDEFS_H)
#define NOTIFYDRIVERDEFS_H


/*  ----------------------------------- IPC */
#include <ipctypes.h>

/*  ----------------------------------- Notify */
#include <notifyerr.h>
#include <notifytypes.h>
#include <notifydefs.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  NOTIFY_SYSTEM_KEY
 *
 *  @desc   This key must be provided as the upper 16 bits of the eventNo when
 *          registering for an event, if any reserved event numbers are to be
 *          used.
 *  ============================================================================
 */
#define NOTIFY_SYSTEM_KEY    (unsigned short int) 0xC1D2

/** ============================================================================
 *  @const  NOTIFY_SYSTEM_KEY_MASK
 *
 *  @desc   Mask to check for system key.
 *  ============================================================================
 */
#define NOTIFY_SYSTEM_KEY_MASK  (unsigned short int) 0xFFFF0000

/** ============================================================================
 *  @const  NOTIFY_EVENT_MASK
 *
 *  @desc   Mask to check for event ID.
 *  ============================================================================
 */
#define NOTIFY_EVENT_MASK      (unsigned short int) 0x0000FFFF


/** ============================================================================
 *  @name   unsigned long int
 *
 *  @desc   This type defines the Notify ID used to identify the position of the
 *          Notify driver within the Notify module table.
 *  ============================================================================
 */


/** ============================================================================
 *  @name   Notify_DriverProcInfo
 *
 *  @desc   This structure defines information for all processors supported by
 *          the Notify driver.
 *          An instance of this object is provided for all processors handled by
 *          the Notify driver, when registering itself with the Notify module.
 *
 *  @field  maxEvents
 *              Maximum number of events supported by the Notify driver for
 *              communication with this processor.
 *  @field  reservedEvents
 *              Maximum number of events reserved for usage only by the system
 *              (kernel-side APIs) for this processor. The events from 0 to
 *              (reservedEvents -1) are considered as reserved by the Notify
 *              module.
 *  @field  eventPriority
 *              Indicates whether event prioritization is supported for this
 *              processor.
 *  @field  payloadSize
 *              Size of payload supported for this processor. Specify 0 if
 *              payload is not supported at all.
 *  @field  procId
 *              ID of this processor
 *
 *  @see    struct Notify_DriverAttrs, Notify_registerDriver ()
 *  ============================================================================
 */
struct Notify_DriverProcInfo {
    unsigned long int       maxEvents ;
    unsigned long int       reservedEvents ;
    short int         eventPriority ;
    unsigned long int       payloadSize ;
    unsigned long int procId ;
};

/** ============================================================================
 *  @name   struct Notify_DriverAttrs
 *
 *  @desc   This structure defines the structure for specifying Notify driver
 *          attributes to the Notify module.
 *          This structure provides information about the Notify driver to the
 *          Notify module. The information is used by the Notify module mainly
 *          for parameter validation. It may also be used by the Notify module
 *          to take appropriate action if required based on the characteristics
 *          of the Notify driver.
 *
 *  @field  numProc
 *              Number of processors handled by this Notify driver
 *  @field  procInfo
 *              Array of processor information objects for processors supported
 *              by this Notify driver
 *
 *  @see    Notify_registerDriver ()
 *  ============================================================================
 */
 struct Notify_DriverAttrs {
    unsigned long int                numProc ;
    struct Notify_DriverProcInfo procInfo [MAX_PROCESSORS] ;
};

/** ============================================================================
 *  @name   Notify_DriverObject, struct Notify_DriverHandle *
 *
 *  @desc   This structure defines the Notify driver object and handle used
 *          internally to contain all information required for the Notify driver
 *          This object contains all information for the Notify module to be
 *          able to identify and interact with the Notify driver.
 *
 *  @field  isInit
 *              indicates whether the Notify driver has been initialized
 *  @field  name
 *              Name of the driver
 *  @field  notifyId
 *              ID of the Notify driver. This is the position in the internal
 *              table maintained by the Notify module, and is used for
 *              registering and un-registering the Notify driver.
 *  @field  fnTable
 *              Function table pointer for the Notify driver
 *  @field  attrs
 *              Information about the registered driver
 *  @field  driverObj
 *              This is a driver-specific object and is defined by the Notify
 *              driver implementer.
 *
 *  @see    Notify_registerDriver ()
 *  ============================================================================
 */
struct Notify_DriverHandle {
    short int                 isInit ;
    char                name [NOTIFY_MAX_NAMELEN] ;
    unsigned long int            notifyId ;
    struct Notify_Interface *   fnTable ;
    struct Notify_DriverAttrs   attrs ;
    void *               driverObj ;
};


/** ============================================================================
 *  @name   Notify_DriverObject, struct Notify_DriverHandle *
 *
 *  @desc   Forward declaration of Notify_DriverObject and definition of handle
 *          to Notify driver.
 *  ============================================================================
 */


/** ============================================================================
 *  @name   FnNotifyDriverInit
 *
 *  @desc   This type defines the initialization function for the Notify driver.
 *          This function gets called internally from the Notify_driverInit ()
 *          API. The Notify_driverInit () function checks for a registered
 *          Notify driver of the specified name and passes on the initialization
 *          request into the Notify driver, if found.
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
 *  @see    struct Notify_Interface, Notify_driverInit ()
 *  ============================================================================
 */
typedef
signed long int
(*FnNotifyDriverInit) (char *          driverName,
                       struct Notify_Config *  config,
                       void **          driverObj) ;

/** ============================================================================
 *  @name   FnNotifyDriverExit
 *
 *  @desc   This type defines the finalization function for the Notify driver.
 *          This function gets called internally from the Notify_driverExit ()
 *          API. The Notify_driverExit () function passes on the initialization
 *          request into the Notify driver identified by the Notify Handle
 *          passed.
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
 *  @see    struct Notify_Interface, Notify_driverExit ()
 *  ============================================================================
 */
typedef
signed long int
(*FnNotifyDriverExit) (struct Notify_DriverHandle  *handle) ;

/** ============================================================================
 *  @func   FnNotifyRegisterEvent
 *
 *  @desc   This type defines the function to register a callback for an event
 *          with the Notify driver.
 *          This function gets called internally from the Notify_registerEvent
 *          API. The Notify_registerEvent () function passes on the
 *          initialization request into the Notify driver identified by the
 *          Notify Handle passed.
 *
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
 *  @see    struct Notify_Interface, Notify_registerEvent ()
 *  ============================================================================
 */
typedef
signed long int
(*FnNotifyRegisterEvent) (struct Notify_DriverHandle * handle,
                          unsigned long int        procId,
                          unsigned long int              eventNo,
                          FnNotifyCbck        fnNotifyCbck,
                          void *              cbckArg) ;

/** ============================================================================
 *  @func   FnNotifyUnregisterEvent
 *
 *  @desc   This type defines the function to unregister a callback for an event
 *          with the Notify driver.
 *          This function gets called internally from the Notify_unregisterEvent
 *          API. The Notify_unregisterEvent () function passes on the
 *          initialization request into the Notify driver identified by the
 *          Notify Handle passed.
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
 *  @see    struct Notify_Interface, Notify_unregisterEvent ()
 *  ============================================================================
 */
typedef
signed long int
(*FnNotifyUnregisterEvent) (struct Notify_DriverHandle * handle,
                            unsigned long int        procId,
                            unsigned long int              eventNo,
                            FnNotifyCbck        fnNotifyCbck,
                            void *              cbckArg) ;

/** ============================================================================
 *  @func   FnNotifySendEvent
 *
 *  @desc   This type defines the function to send a notification event to the
 *          registered users for this notification on the specified processor.
 *          This function gets called internally from the Notify_sendEvent ()
 *          API. The Notify_sendEvent () function passes on the initialization
 *          request into the Notify driver identified by the Notify Handle
 *          passed.
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
 *  @see    struct Notify_Interface, Notify_sendEvent ()
 *  ============================================================================
 */
typedef
signed long int
(*FnNotifySendEvent) (struct Notify_DriverHandle * handle,
                      unsigned long int        procId,
                      unsigned long int              eventNo,
                      unsigned long int              payload,
                      short int                waitClear) ;

/** ============================================================================
 *  @func   FnNotifyDisable
 *
 *  @desc   This type defines the function to disable all events for this
 *          Notify driver.
 *          This function gets called internally from the Notify_disable () API.
 *          The Notify_disable () function passes on the disable call into all
 *          Notify drivers registered with it.
 *
 *  @arg    handle
 *              Handle to the Notify driver
 *
 *  @ret    flags
 *              Flags to be provided when FnNotifyRestore is called.
 *
 *  @enter  The Notify module and driver must be initialized before calling this
 *          function.
 *          handle must be a valid Notify driver handle
 *
 *  @leave  On success, all events for the Notify driver must be disabled
 *
 *  @see    struct Notify_Interfacej, Notify_disable ()
 *  ============================================================================
 */
typedef
void *
(*FnNotifyDisable) (struct Notify_DriverHandle * handle) ;

/** ============================================================================
 *  @func   FnNotifyRestore
 *
 *  @desc   This type defines the function to restore the Notify driver to the
 *          state before the last disable was called.
 *          This function gets called internally from the Notify_restore () API.
 *          The Notify_restore () function passes on the restore call into all
 *          Notify drivers registered with it.
 *
 *  @arg    handle
 *              Handle to the Notify driver
 *  @arg    flags
 *              Flags returned from the call to last FnNotifyDisable in order
 *              to restore the Notify driver to the state before the last
 *              disable call.
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
 *          FnNotifyDisable call.
 *
 *  @leave  On success, all events for the Notify driver must be restored to
 *          the state as indicated by the provided flags.
 *
 *  @see    struct Notify_Interface, Notify_restore ()
 *  ============================================================================
 */
typedef
signed long int
(*FnNotifyRestore) (struct Notify_DriverHandle * handle,
                    void *              flags) ;

/** ============================================================================
 *  @func   FnNotifyDisableEvent
 *
 *  @desc   This type defines the function to disable a specific event for this
 *          Notify driver.
 *          This function gets called internally from the Notify_disableEvent ()
 *          API. The Notify_disableEvent () function passes on the call into
 *          Notify driver identified by the Notify handle passed.
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
 *  @see    struct Notify_Interface, Notify_disableEvent ()
 *  ============================================================================
 */
typedef
signed long int
(*FnNotifyDisableEvent) (struct Notify_DriverHandle * handle,
                         unsigned long int        procId,
                         unsigned long int              eventNo) ;

/** ============================================================================
 *  @func   FnNotifyEnableEvent
 *
 *  @desc   This type defines the function to enable a specific event for this
 *          Notify driver.
 *          This function gets called internally from the Notify_enableEvent ()
 *          API. The Notify_enableEvent () function passes on the call into the
 *          Notify driver identified by the handle passed.
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
 *  @see    struct Notify_Interface, Notify_enableEvent ()
 *  ============================================================================
 */
typedef
signed long int
(*FnNotifyEnableEvent) (struct Notify_DriverHandle * handle,
                        unsigned long int        procId,
                        unsigned long int              eventNo) ;

#if defined (NOTIFY_DEBUG)
/** ============================================================================
 *  @func   FnNotifyDebug
 *
 *  @desc   This type defines the function to print debug information for the
 *          Notify module and driver.
 *          This function gets called internally from the Notify_debug () API.
 *          The Notify_debug () function first prints Notify module specific
 *          information and then passes on the initialization request into the
 *          Notify driver identified by the Notify Handle passed.
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
 *  @see    struct Notify_Interface, Notify_debug ()
 *  ============================================================================
 */
typedef
signed long int
(*FnNotifyDebug) (struct Notify_DriverHandle * handle) ;
#endif /* defined (NOTIFY_DEBUG) */


/** ============================================================================
 *  @name   struct Notify_Interface
 *
 *  @desc   This structure defines the function table interface for the Notify
 *          driver.
 *          This function table interface must be implemented by each Notify
 *          driver and registered with the Notify module.
 *
 *  @field  notifyDriverInit
 *              Function to initialize the Notify driver
 *  @field  notifyDriverExit
 *              Function to finalize the Notify driver
 *  @field  notifyRegisterEvent
 *              Function to register a callback for an event with the Notify
 *              driver
 *  @field  notifyUnregisterEvent
 *              Function to un-register a callback for an event with the Notify
 *              driver
 *  @field  notifySendEvent
 *              Function to send a notification event to the specified processor
 *              using the Notify driver
 *  @field  notifyDisable
 *              Function to disable all events for this Notify driver
 *  @field  notifyRestore
 *              Function to restore Notify driver to the state before the last
 *              disable call
 *  @field  notifyDisableEvent
 *              Function to disable a specific event for this Notify driver
 *  @field  notifyEnableEvent
 *              Function to enable a specific event for this Notify driver
 *  @field  notifyDebug
 *              Function to print debug information for the Notify driver.
 *              Only defined for debug build
 *
 *  @see    Notify_registerDriver ()
 *  ============================================================================
 */
struct Notify_Interface {
    FnNotifyDriverInit      notifyDriverInit ;
    FnNotifyDriverExit      notifyDriverExit ;
    FnNotifyRegisterEvent   notifyRegisterEvent ;
    FnNotifyUnregisterEvent notifyUnregisterEvent ;
    FnNotifySendEvent       notifySendEvent ;
    FnNotifyDisable         notifyDisable ;
    FnNotifyRestore         notifyRestore ;
    FnNotifyDisableEvent    notifyDisableEvent ;
    FnNotifyEnableEvent     notifyEnableEvent ;
#if defined (NOTIFY_DEBUG)
    FnNotifyDebug           notifyDebug ;
#endif /* defined (NOTIFY_DEBUG) */
};


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (NOTIFYDRIVERDEFS_H) */
