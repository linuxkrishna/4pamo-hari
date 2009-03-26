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
MODULE_LICENSE("GPL");


/*----------------------------------- IPC Headers                 */
#include <gpptypes.h>
#include <ipctypes.h>
#include <dbc.h>

/*----------------------------------- OSAL Headers                */
#include <sync.h>

/*----------------------------------- Notify Headers              */
#include <notify.h>
#include <notify_driver.h>
#include <_notifydefs.h>
#include <notifyerr.h>

/*----------------------------------- Generic Headers             */
#include <gen_utils.h>
#include <mem.h>
#include <trc.h>
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
#define SET_FAILURE_REASON(status) (TRC_SetReason \
(status, FID_C_KNL_NOTIFY_DRIVER, __LINE__))
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
*@name   Notify_isInit
*
*@desc   Extern declaration to the Notify module initialization flag.
*============================================================================
*/

/* *============================================================================
*@name  struct Notify_StateObj
*
*@desc   Extern declaration to the Notify state object instance.
*============================================================================
*/


/* *============================================================================
*@func   Notify_registerDriver
*
*@desc   This function registers a Notify driver with the Notify module.
*
*@modif struct Notify_StateObj.drivers
*============================================================================
*/

signed long int
Notify_registerDriver(IN  char  *driverName,
		IN  struct Notify_Interface  *fnTable,
		IN  struct Notify_DriverAttrs  *drvAttrs,
		OUT struct Notify_DriverHandle  **driverHandle)
{
	signed long int       status    = NOTIFY_SOK ;
	struct Notify_DriverHandle  *drvHandle = NULL ;
	unsigned short int              i ;
	TRC_4ENTER("Notify_registerDriver",
			driverName,
			fnTable,
			drvAttrs,
			driverHandle) ;

	DBC_require(Notify_isInit == TRUE) ;
	DBC_require(driverName    != NULL) ;
	DBC_require(fnTable       != NULL) ;
	DBC_require(drvAttrs      != NULL) ;
	DBC_require(driverHandle  != NULL) ;

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;
	} else if ((driverName   == NULL)
			|| (fnTable      == NULL)
			|| (drvAttrs     == NULL)
			|| (driverHandle == NULL)) {
		status = NOTIFY_EPOINTER ;
		SET_FAILURE_REASON(status) ;
	} else {
		*driverHandle = NULL ;
		/*Initialize to status that indicates that an empty slot was not
		  *found for the driver.
		 */
		status = NOTIFY_ERESOURCE ;


		for (i = 0 ; i < Notify_StateObj.maxDrivers ; i++) {

			drvHandle = &(Notify_StateObj.drivers[i]) ;

			if ((drvHandle->fnTable != NULL) &&
					(drvHandle->fnTable !=
				(struct Notify_Interface *) TRUE)) {

				if (GEN_strncmp(driverName,
					drvHandle->name,
					NOTIFY_MAX_NAMELEN)
							== 0) {

					status = NOTIFY_EALREADYEXISTS ;
					break ;
				}
			}

			if (drvHandle->fnTable == NULL) {
				/*Found an empty slot, so block it. */
				drvHandle->fnTable =
					(struct Notify_Interface *) TRUE ;
				status = NOTIFY_SOK ;
				break ;
			}
		}

		if (NOTIFY_FAILED(status)) {
			SET_FAILURE_REASON(status) ;
		} else {
			/*Complete registration of the driver. */
			GEN_strncpy(drvHandle->name,
					driverName, NOTIFY_MAX_NAMELEN) ;
			memcpy(&(drvHandle->attrs),
					drvAttrs,
					sizeof(struct Notify_DriverAttrs)) ;
			drvHandle->fnTable = fnTable ;
			/*notifyId is set by default. */
			/*isInit is set when driverInit is called. */
			*driverHandle = drvHandle ;
		}
	}

	TRC_1LEAVE("Notify_registerDriver", status) ;

	return status ;
}
EXPORT_SYMBOL(Notify_registerDriver);

/* *============================================================================
*@func   Notify_unregisterDriver
*
*@desc   This function un-registers a Notify driver with the Notify module.
*
*@modif  None
*============================================================================
*/

signed long int
Notify_unregisterDriver(IN  struct Notify_DriverHandle  *drvHandle)
{
	signed long int status = NOTIFY_SOK ;

	TRC_1ENTER("Notify_unregisterDriver", drvHandle) ;

	DBC_require(Notify_isInit  == TRUE) ;
	DBC_require(drvHandle      != NULL) ;

	if (Notify_isInit == FALSE) {
		status = NOTIFY_EINIT ;
		SET_FAILURE_REASON(status) ;
	} else if (drvHandle == NULL) {
		status = NOTIFY_EHANDLE ;
		SET_FAILURE_REASON(status) ;
	} else {
		/*Unregister the driver. */
		drvHandle->fnTable = NULL ;
	}

	TRC_1LEAVE("Notify_unregisterDriver", status) ;

	return status ;
}
EXPORT_SYMBOL(Notify_unregisterDriver);

