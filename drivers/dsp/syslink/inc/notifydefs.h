/*
 * notifydefs.h
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


#if !defined NOTIFYDEFS_H
#define NOTIFYDEFS_H

/*  ----------------------------------- Notify */
#include <notifyerr.h>
/*
 *  const  NOTIFY_MAX_DRIVERS
 *
 *  desc   Maximum number of Notify drivers supported.
 *          NOTE: This can be modified by the user if data section size needs to
 *                be reduced, or if there is a need for more than the defined
 *                max drivers.
 *
 */

#define NOTIFY_MAX_DRIVERS   16

/*
 *  const  NOTIFY_MAX_NAMELEN
 *
 *  desc   Maximum length of the name of Notify drivers, inclusive of NULL
 *          string terminator.
 *
 */
#define NOTIFY_MAX_NAMELEN   32


/*
 *  name   fn_notify_cbck
 *
 *  desc   Signature of the callback function to be registered with the NOTIFY
 *          component.
 *
 *  arg    proc_id
 *              Processor ID from which the event is being received
 *  arg    event_no
 *              Event number for which the callback is being received
 *  arg    arg
 *              Fixed argument registered with the IPS component along with
 *              the callback function.
 *  arg    payload
 *              Run-time information provided to the upper layer by the Notify
 *              component. Depending on the Notify driver implementation, this
 *              may or may not pass on a user-specific value to the registered
 *              application
 *
 *  ret    None.
 *
 *  enter  None.
 *
 *  leave  None.
 *
 *  see    None.
 *
 */

typedef void (*fn_notify_cbck) (unsigned long int proc_id,
						u32       event_no,
						void *arg,
						u32       payload) ;


/*
 *  name   notify_attrs
 *
 *  desc   This structure defines attributes for initialization of the Notify
 *          module.
 *
 *  field  maxDrivers
 *              Maximum number of drivers that can be registered with the Notify
 *              module.
 *
 *  see    notify_init ()
 *
 */
struct notify_attrs {
	u32 maxDrivers ;
};

/*
 *  name   notify_config
 *
 *  desc   This structure defines the configuration structure for
 *          initialization of the Notify driver.
 *
 *  field  driverAttrs
 *              Notify driver-specific attributes
 *
 *  see    notify_driver_init ()
 *
 */

struct notify_config {
	void *driverAttrs ;
};

#if 0
/*
 *  name   notify_state
 *
 *  desc   Defines the Notify state object, which contains all the module
 *          specific information.
 *
 *  field  maxDrivers
 *              Maximum number of Notify drivers configured
 *  field  drivers
 *              Array of Notify driver object.
 *  field  disable_flags
 *              Array of flags for Notify disable.
 *              TBD: Currently does not support nesting of disable/restore calls
 *
 */
struct notify_state {
	unsigned short int	maxDrivers ;
	struct notify_driver_handle  drivers[NOTIFY_MAX_DRIVERS] ;
	void *disable_flags[NOTIFY_MAX_DRIVERS] ;
};

#endif
#endif  /* !defined (NOTIFYDEFS_H) */
