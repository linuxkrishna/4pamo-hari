/*
 * notify_mbxDriver.h
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

#ifndef NOTIFY_SHMDRIVER_H_
#define NOTIFY_SHMDRIVER_H_



/* Notify*/

#include <dspbridge/gpptypes.h>
#include <dspbridge/notifyerr.h>

/*
 *  const  NOTIFYSHMDRV_DRIVERNAME
 *
 *  desc   Name of the Notify Shared Memory Mailbox driver.
 *
 */
#define NOTIFYMBXDRV_DRIVERNAME   "NOTIFYMBXDRV"

/*
 *  const  NOTIFYSHMDRV_RESERVED_EVENTS
 *
 *  desc   Maximum number of events marked as reserved events by the
 *          notify_shmdrv driver.
 *          If required, this value can be changed by the system integrator.
 *
 */
#define NOTIFYSHMDRV_RESERVED_EVENTS  3


/*
 *  name   notify_shmdrv_attrs
 *
 *  desc   This structure defines the attributes for Notify Shared Memory
 *          Mailbox driver.
 *          These attributes are passed to the driver when notify_driver_init ()
 *          is called for this driver.
 *
 *  field  shmBaseAddr
 *              Shared memory address base for the NotifyShmDrv driver. This
 *              must be the start of shared memory as used by both connected
 *              processors, and the same must be specified on both sides when
 *              initializing the NotifyShmDrv driver.
 *  field  shmSize
 *              Size of shared memory provided to the NotifyShmDrv driver. This
 *              must be the start of shared memory as used by both connected
 *              processors, and the same must be specified on both sides when
 *              initializing the NotifyShmDrv driver.
 *  field  num_events
 *              Number of events required to be supported. Must be greater than
 *              or equal to reserved events supported by the driver.
 *  field  send_event_pollcount
 *              Poll count to be used when sending event. If the count is
 *              specified as -1, the wait will be infinite. NOTIFY_sendEvent
 *              will return with timeout error if the poll count expires before
 *              the other processor acknowledges the received event.
 *
 *  see    None.
 *
 */
struct notify_shmdrv_attrs {
	unsigned long int    shmBaseAddr ;
	unsigned long int    shmSize ;
	unsigned long int    num_events ;
	unsigned long int    send_event_pollcount ;
};


/*
 *  name   notify_mbxdrv_init
 *
 *  desc   Top-level initialization function for the Notify shared memory
 *          mailbox driver.
 *          This can be plugged in as the user init function.
 *
 *  arg    None.
 *
 *  ret    None.
 *
 *  enter  Notify module must have been initialized before this call
 *
 *  leave  On success, the driver is registered with the Notify module.
 *
 *  see    notify_mbxdrv_exit ()
 *
 */

void notify_mbxdrv_init(void) ;

/*
 *  name   notify_mbxdrv_exit
 *
 *  desc   Top-level finalization function for the Notify shared memory
 *          mailbox driver.
 *
 *  arg    None.
 *
 *  ret    None.
 *
 *  enter  Notify module must have been initialized before this call
 *
 *  leave  On success, the driver is unregistered with the Notify module.
 *
 *  see    notify_mbxdrv_init ()
 *
 */

void notify_mbxdrv_exit(void) ;



#endif  /* !defined  NOTIFY_SHMDRIVER_H_ */

