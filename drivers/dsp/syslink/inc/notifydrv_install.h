/*
 * notify_install.h
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

#if !defined NOTIFYDRIVER_H
#define NOTIFYDRIVER_H


/*  ----------------------------------- Notify */
#include <gpptypes.h>
#include <notifyerr.h>

/*  ----------------------------------- Notify driver */
#include <notify_driverdefs.h>


/*
 *  func   notify_register_driver
 *
 *  desc
 * This function registers a Notify driver with the Notify module.
 * each Notify driver must register itself with the Notify module. Once
 * the registration is done, the user can start using the Notify APIs
 * to interact with the Notify driver for sending and receiving
 * notifications.
 *
 *  arg    driver_name
 *              Name of the Notify driver
 *  arg    fn_table
 *              Pointer to the function table for this Notify driver
 *  arg    drv_attrs
 *              Attributes of the Notify driver relevant to the generic Notify
 *              module
 *  arg    driver_handle
 *              Location to receive the pointer to the Notify driver handle
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINIT
 *              The Notify module or driver was not initialized
 *          NOTIFY_ERESOURCE
 *              The maximum number of drivers have already been registered with
 *              the Notify module
 *          NOTIFY_EALREADYEXISTS
 *              The specified Notify driver is already registered.
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EPOINTER
 *              An invalid pointer was specified
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module must be initialized before calling this function.
 *          driver_name must be a valid string.
 *          fn_table must be valid.
 *          driverAttrs must be valid.
 *          driver_handle must be a valid pointer.
 *
 *  leave  On success, the Notify driver must be registered with the Notify
 *          module
 *
 *  see    struct notify_interface, struct notify_driver_attrs,
 * notify_unregister_driver ()
 *
 */

signed long int notify_register_driver(IN  char *driver_name,
		IN  struct notify_interface *fn_table,
		IN  struct notify_driver_attrs *drv_attrs,
		OUT struct notify_driver_handle **driver_handle) ;


/*
 *  func   notify_unregister_driver
 *
 *  desc   This function un-registers a Notify driver with the Notify module.
 *          When the Notify driver is no longer required, it can un-register
 *          itself with the Notify module. This facility is also useful if a
 *          different type of Notify driver needs to be plugged in for the same
 *          physical interrupt to meet different needs.
 *
 *  arg    drv_handle
 *              Handle to the Notify driver object
 *
 *  ret    NOTIFY_SOK
 *              Operation successfully completed
 *          NOTIFY_EINIT
 *              The Notify module or driver was not initialized
 *          NOTIFY_ENOTFOUND
 *              The specified driver is not registered with the Notify module
 *          NOTIFY_EHANDLE
 *              Invalid Notify handle specified
 *          NOTIFY_EMEMORY
 *              Operation failed due to a memory error
 *          NOTIFY_EINVALIDARG
 *              Invalid argument
 *          NOTIFY_EFAIL
 *              General failure
 *
 *  enter  The Notify module must be initialized before calling this function.
 *          handle must be a valid Notify driver handle.
 *
 *  leave  On success, the Notify driver must be unregistered with the Notify
 *          module
 *
 *  see    struct notify_driver_handle *, notify_register_driver ()
 *
 */

signed long int
notify_unregister_driver(IN  struct notify_driver_handle *drv_handle) ;


#endif  /* !defined (NOTIFYDRIVER_H) */
