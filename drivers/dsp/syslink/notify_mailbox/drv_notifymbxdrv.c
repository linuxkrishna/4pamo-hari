/*
 *
 *  @path   $(NOTIFY)/gpp/src/knl/drivers/drv/Linux/2.6.18/
 *
 *  @desc   Implementation of linux module driver interface for NotifyMbxDrv
 *
 *  @ver    1.00.00.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  Defined to include MACROS EXPORT_SYMBOL. This must be done before including
 *  module.h
 */
#if !defined EXPORT_SYMTAB
#define EXPORT_SYMTAB
#endif

/*  ----------------------------------- OS Specific Headers         */
#include <linux/autoconf.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include <asm/io.h>
#include <asm/pgtable.h>

/*  ----------------------------------- IPC headers                 */
#include <ipc.h>
#include <_ipc.h>
#include <ipcdefs.h>
#include <gpptypes.h>
#include <notifyerr.h>
#include <_notifydefs.h>
#include <GlobalTypes.h>


/*  ----------------------------------- Trace & Debug               */
#include <trc.h>

/*  ----------------------------------- Notify headers              */
#include <notify_mbxDriver.h>





/* Export kernel Notify functions */




#if defined CONFIG_PREEMPT_RT
#if CONFIG_PREEMPT_RT == 1
#error Realtime preemption is not supported with this version of Notify IPC
#endif
#endif /* defined (CONFIG_PREEMPT_RT) */


#if defined __cplusplus
extern "C" {
#endif /* defined (__cplusplus) */

extern void NotifyMbxDrv_NonShmISR(IN void *refData);
/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Identifier for sub-component.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_DRV_NOTIFYSHMDRV

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#if defined NOTIFY_DEBUG
#define SET_FAILURE_REASON(status) \
		TRC_SetReason(status, FID_C_DRV_NOTIFYSHMDRV, __LINE__)
#else
#define SET_FAILURE_REASON(status)
#endif /* if defined (NOTIFY_DEBUG) */


extern short int Notify_isInit;
extern struct Notify_State Notify_StateObj;
const unsigned long *LinearAddress;
EXPORT_SYMBOL(LinearAddress);
irqreturn_t Notify_Mailbox0User0_ISR(int temp, void *anArg, struct pt_regs *p);








/** ----------------------------------------------------------------------------
 *  @name   major
 *
 *  @desc   Major number of driver.
 *  ----------------------------------------------------------------------------
 */
static signed long int major = 233 ;

#define OMAP_MBOX_BASE 0x4A0F4000
#define OMAP_MBOX_SIZE 0x2000

/** ----------------------------------------------------------------------------
 *  @name   DRV_open
 *
 *  @desc   Linux driver function to open the driver object.
 *
 *  @arg    inode
 *              inode pointer.
 *          filp
 *              File structure pointer.
 *
 *  @ret    0
 *              Success.
 *          non-zero
 *              Failure.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.

 *  ----------------------------------------------------------------------------
 */
static
int
DRVMbx_open(struct inode *inode, struct file *filp) ;


/** ----------------------------------------------------------------------------
 *  @func   DRV_initializeModule
 *
 *  @desc   Module initialization function for Linux driver.
 *
 *  @arg    None.
 *
 *  @ret    0
 *              Successful execution.
 *          non-zero
 *              Failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static int __init DRVMbx_initializeModule(void) ;


/** ----------------------------------------------------------------------------
 *  @func   DRV_finalizeModule
 *
 *  @desc   Module finalization  function for Linux driver.
 *
 *  @arg    None.
 *
 *  @ret    None.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static void  __exit DRVMbx_finalizeModule(void) ;


/** ----------------------------------------------------------------------------
 *  @name   DriverOps
 *
 *  @desc   Function to invoke the APIs through ioctl.
 *  ----------------------------------------------------------------------------
 */
static struct file_operations driverOps = {
open:    DRVMbx_open,
} ;


/** ----------------------------------------------------------------------------
 *  @func   DRV_initializeModule
 *
 *  @desc   Module initialization  function for Linux driver.
 *  ----------------------------------------------------------------------------
 */
static
int __init DRVMbx_initializeModule(void)
{
	signed long int status = NOTIFY_SOK;
	int result    = 0;


/* Display the version info and created date/time */
 printk(KERN_ALERT "NotifyMbxDrv Module (%s) created on Date: %s Time: %s\n",
				NOTIFY_VERSION,
					__DATE__,
					__TIME__) ;

/*****************************MailBox Remap************************/

	LinearAddress = (const unsigned long *)ioremap(OMAP_MBOX_BASE,
					OMAP_MBOX_SIZE);

	/*  --------------------------------------------------------------------
	*  To enable trace for a component and/or subcomponent, uncomment the
	*  corresponding statements below. (This is not a comprehensive list
	*  of available trace masks. See file _signature.h)
	*  --------------------------------------------------------------------
	*/

	TRC_ENABLE(ID_NOTIFYSHMDRV_ALL)     ;
	TRC_ENABLE(ID_HAL_ALL)              ;

	/* TRC_ENABLE(ID_KNL_NOTIFY_SHMDRIVER) ; */
	/* TRC_ENABLE(ID_DRV_NOTIFYSHMDRV)     ; */

	/* TRC_ENABLE(ID_HAL_INTERRUPT)        ; */

	/*  --------------------------------------------------------------------
	*  To set desired severity level for trace, uncomment the statement
	*  below and change the argument to the function below.
	*  --------------------------------------------------------------------
	*/

	/* TRC_SET_SEVERITY (TRC_ENTER) ; */

TRC_0ENTER("DRV_initializeModule") ;

	result = register_chrdev(major, "notifyshmdrv", &driverOps) ;
	if (result < 0) {
		status = NOTIFY_EFAIL ;
		SET_FAILURE_REASON(status) ;
		TRC_1PRINT(TRC_LEVEL7,
			"Linux API register_chrdev returned error: %d\n",
								result);
	}

	if (NOTIFY_SUCCEEDED(status))
			NotifyMbxDrv_init() ;

    TRC_1LEAVE("DRV_initializeModule", result) ;

    return result ;
}


/** ----------------------------------------------------------------------------
 *  @name   DRV_finalizeModule
 *
 *  @desc   Linux driver function to finalize the driver module.
 *  ----------------------------------------------------------------------------
 */
static
void __exit DRVMbx_finalizeModule(void)
{
	TRC_0ENTER("DRV_finalizeModule") ;

	unregister_chrdev(major, "notifyshmdrv") ;

	NotifyMbxDrv_exit() ;

	TRC_0LEAVE("DRV_finalizeModule") ;
}


/** ----------------------------------------------------------------------------
 *  @name   DRV_open
 *
 *  @desc   Linux specific function to open the driver.
 *  ----------------------------------------------------------------------------
 */
int DRVMbx_open(struct inode *inode, struct file *filp)
{
	return 0 ;
}



/** ============================================================================
 *  @name   module_init/module_exit
 *
 *  @desc   Macro calls that indicate initialization and finalization functions
 *          to the kernel.
 *  ============================================================================
 */
MODULE_LICENSE("GPL");
module_init(DRVMbx_initializeModule) ;
module_exit(DRVMbx_finalizeModule) ;


#if defined __cplusplus
}
#endif /* defined (__cplusplus) */

