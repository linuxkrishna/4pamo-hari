/* *============================================================================
*@file   notify.c
*
*@path   $(NOTIFY)/dsp/src/Notify/
*
*@desc   Implementation of Notify module implementation for the driver
*interface.
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
#define EXPORT_SYMTAB
#endif
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/uaccess.h>

#include <linux/io.h>
#include <asm/pgtable.h>
#include <gt.h>
MODULE_LICENSE("GPL");


/*----------------------------------- IPC Headers                 */
#include <gpptypes.h>
#include <dbc.h>

/*----------------------------------- Notify Headers              */
#include <notify.h>
#include <notify_driver.h>
#include <notifyerr.h>

/*----------------------------------- Generic Headers             */
#include <mem.h>
#include <global_var.h>

/* *============================================================================
*@macro  COMPONENT_ID
*
*@desc   Component and Subcomponent Identifier.
*============================================================================
*/
#define  COMPONENT_ID       ID_KNL_NOTIFY_DRIVER

/* *============================================================================
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

/* *============================================================================
*@const  SPINLOCK_START
*
*@desc   Start spinlock protection
*============================================================================
*/

/* *============================================================================
*@const  SPINLOCK_END
*
*@desc   End spinlock protection
*============================================================================
*/


/* *============================================================================
*@name   notify_is_init
*
*@desc   Extern declaration to the Notify module initialization flag.
*============================================================================
*/

/* *============================================================================
*@name  struct notify_state_obj
*
*@desc   Extern declaration to the Notify state object instance.
*============================================================================
*/


/* *============================================================================
*@func   notify_register_driver
*
*@desc   This function registers a Notify driver with the Notify module.
*
*@modif struct notify_state_obj.drivers
*============================================================================
*/

signed long int notify_register_driver(IN  char  *driver_name,
		IN  struct notify_interface  *fn_table,
		IN  struct notify_driver_attrs  *drv_attrs,
		OUT struct notify_driver_handle  **driver_handle)
{
	signed long int       status    = 0 ;
	struct notify_driver_handle  *drv_handle = NULL ;
	unsigned short int              i ;
	gt_4trace(notify_debugmask, GT_ENTER, "notify_register_driver",
			driver_name,
			fn_table,
			drv_attrs,
			driver_handle) ;

	BUG_ON(notify_is_init != TRUE) ;
	BUG_ON(driver_name == NULL) ;
	BUG_ON(fn_table == NULL) ;
	BUG_ON(drv_attrs == NULL) ;
	BUG_ON(driver_handle == NULL) ;

	if (notify_is_init == FALSE) {
		status = -EPERM ;
		goto error_exit;
	} else if ((driver_name   == NULL)
			|| (fn_table      == NULL)
			|| (drv_attrs     == NULL)
			|| (driver_handle == NULL)) {
		status = -EINVAL ;
		goto error_exit;
	} else {
		*driver_handle = NULL ;
		/*Initialize to status that indicates that an empty slot was not
		  *found for the driver.
		 */
		status = -ENOENT ;


		for (i = 0 ; i < notify_state_obj.maxDrivers ; i++) {

			drv_handle = &(notify_state_obj.drivers[i]) ;

			if ((drv_handle->fn_table != NULL) &&
					(drv_handle->fn_table !=
				(struct notify_interface *) TRUE)) {

				if (strncmp(driver_name,
					drv_handle->name,
					NOTIFY_MAX_NAMELEN)
							== 0) {

					status = -EEXIST ;
					break ;
				}
			}

			if (drv_handle->fn_table == NULL) {
				/*Found an empty slot, so block it. */
				drv_handle->fn_table =
					(struct notify_interface *) TRUE ;
				status = 0 ;
				break ;
			}
		}

		if (status != 0) {
			SET_FAILURE_REASON(status) ;
		} else {
			/*Complete registration of the driver. */
			strncpy(drv_handle->name,
					driver_name, NOTIFY_MAX_NAMELEN) ;
			memcpy(&(drv_handle->attrs),
					drv_attrs,
					sizeof(struct notify_driver_attrs)) ;
			drv_handle->fn_table = fn_table ;
			/*notifyId is set by default. */
			/*isInit is set when driverInit is called. */
			*driver_handle = drv_handle ;
		}
	}

	gt_1trace(notify_debugmask, GT_5CLASS,
		"notify_register_driver", status) ;
error_exit:
	return status ;
}
EXPORT_SYMBOL(notify_register_driver);

/* *============================================================================
*@func   notify_unregister_driver
*
*@desc   This function un-registers a Notify driver with the Notify module.
*
*@modif  None
*============================================================================
*/

signed long int
notify_unregister_driver(IN  struct notify_driver_handle  *drv_handle)
{
	signed long int status = 0;

	gt_1trace(notify_debugmask, GT_ENTER,
		"notify_unregister_driver", drv_handle) ;

	dbc_require(notify_is_init  == TRUE) ;
	dbc_require(drv_handle      != NULL) ;

	if (notify_is_init == FALSE) {
		status = -ENODEV ;
		SET_FAILURE_REASON(status) ;
	} else if (drv_handle == NULL) {
		status = -EFAULT;
		SET_FAILURE_REASON(status) ;
	} else {
		/*Unregister the driver. */
		drv_handle->fn_table = NULL ;
	}

	gt_1trace(notify_debugmask, GT_5CLASS,
		"notify_unregister_driver", status) ;

	return status ;
}
EXPORT_SYMBOL(notify_unregister_driver);

