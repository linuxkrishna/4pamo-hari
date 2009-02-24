/** ============================================================================
 *  @file   notify_shmDriver.h
 *
 *  @path   $(NOTIFY)/dsp/inc
 *
 *  @desc   Defines the direct user interface for the Notify driver using
 *          Shared Memory and interrupts to communicate with the remote
 *          processor.
 *
 *  @ver    1.00.00.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (NOTIFY_SHMDRIVER_H_)
#define NOTIFY_SHMDRIVER_H_


/*  ----------------------------------- IPC */
#include <ipctypes.h>

/*  ----------------------------------- Notify       */
#include <notifyerr.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  NOTIFYSHMDRV_DRIVERNAME
 *
 *  @desc   Name of the Notify Shared Memory Mailbox driver.
 *  ============================================================================
 */
#define NOTIFYSHMDRV_DRIVERNAME   "NOTIFYSHMDRV"

/** ============================================================================
 *  @const  NOTIFYSHMDRV_RESERVED_EVENTS
 *
 *  @desc   Maximum number of events marked as reserved events by the
 *          NotiyShmDrv driver.
 *          If required, this value can be changed by the system integrator.
 *  ============================================================================
 */
#define NOTIFYSHMDRV_RESERVED_EVENTS  3


/** ============================================================================
 *  @name   NotifyShmDrv_Attrs
 *
 *  @desc   This structure defines the attributes for Notify Shared Memory
 *          Mailbox driver.
 *          These attributes are passed to the driver when Notify_driverInit ()
 *          is called for this driver.
 *
 *  @field  shmBaseAddr
 *              Shared memory address base for the NotifyShmDrv driver. This
 *              must be the start of shared memory as used by both connected
 *              processors, and the same must be specified on both sides when
 *              initializing the NotifyShmDrv driver.
 *  @field  shmSize
 *              Size of shared memory provided to the NotifyShmDrv driver. This
 *              must be the start of shared memory as used by both connected
 *              processors, and the same must be specified on both sides when
 *              initializing the NotifyShmDrv driver.
 *  @field  numEvents
 *              Number of events required to be supported. Must be greater than
 *              or equal to reserved events supported by the driver.
 *  @field  sendEventPollCount
 *              Poll count to be used when sending event. If the count is
 *              specified as -1, the wait will be infinite. NOTIFY_sendEvent
 *              will return with timeout error if the poll count expires before
 *              the other processor acknowledges the received event.
 *
 *  @see    None.
 *  ============================================================================
 */
struct NotifyShmDrv_Attrs {
    unsigned long int    shmBaseAddr ;
    unsigned long int    shmSize ;
    unsigned long int    numEvents ;
    unsigned long int    sendEventPollCount ;
};


/** ============================================================================
 *  @name   NotifyShmDrv_init
 *
 *  @desc   Top-level initialization function for the Notify shared memory
 *          mailbox driver.
 *          This can be plugged in as the user init function.
 *
 *  @arg    None.
 *
 *  @ret    None.
 *
 *  @enter  Notify module must have been initialized before this call
 *
 *  @leave  On success, the driver is registered with the Notify module.
 *
 *  @see    NotifyShmDrv_exit ()
 *  ============================================================================
 */
EXPORT_API
void
NotifyShmDrv_init (void) ;

/** ============================================================================
 *  @name   NotifyShmDrv_exit
 *
 *  @desc   Top-level finalization function for the Notify shared memory
 *          mailbox driver.
 *
 *  @arg    None.
 *
 *  @ret    None.
 *
 *  @enter  Notify module must have been initialized before this call
 *
 *  @leave  On success, the driver is unregistered with the Notify module.
 *
 *  @see    NotifyShmDrv_init ()
 *  ============================================================================
 */
EXPORT_API
void
NotifyShmDrv_exit (void) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (NOTIFY_SHMDRIVER_H_) */

