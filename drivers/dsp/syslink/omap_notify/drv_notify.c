
/**============================================================================
*@file   drv_notify.c
*
*@desc   Implementation of linux module driver interface.
*
*============================================================================
*Copyright(c) Texas Instruments Incorporated 2002-2008
*
*Use of this software is controlled by the terms and conditions found in the
*license agreement under which this software has been supplied or provided.
*============================================================================
*/



/*----------------------------------- OS Specific Headers         */
#include <linux/autoconf.h>
#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/list.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <asm/pgtable.h>


/*----------------------------------- IPC headers                 */
#include <ipc.h>
#include <_ipc.h>
#include <ipcdefs.h>
#include <trc.h>

/*----------------------------------- Trace & Debug               */

/*----------------------------------- OSAL Headers                */
#include <_drvdefs.h>
#include <sync.h>

/*----------------------------------- gen headers                 */
#include "list.h"
#include <mem.h>


/*----------------------------------- Notify headers              */
#include <gpptypes.h>
#include <notifyerr.h>
#include <notify.h>
#include <notify_driver.h>

#include <_notifydefs.h>
#include <global_var.h>





/**============================================================================
*@macro  COMPONENT_ID
*
*@desc   Identifier for sub-component.
*============================================================================
*/
#define  COMPONENT_ID       ID_DRV_IPCNOTIFY

/**============================================================================
*@macro  SET_FAILURE_REASON
*
*@desc   Sets failure reason.
*============================================================================
*/
#if defined NOTIFY_DEBUG
#define SET_FAILURE_REASON(status) \
TRC_SetReason(status, FID_C_DRV_IPCNOTIFY, __LINE__)
#else
#define SET_FAILURE_REASON(status)
#endif


/**============================================================================
*@macro  MAX_PROCESSES
*
*@desc   Maximum number of user supported.
*============================================================================
*/
#define  MAX_PROCESSES 32u


/**============================================================================
*@name   Event_IsInitialized
*
*@desc   Flag for indicating initialization status.
*============================================================================
*/
static bool Event_IsInitialized = FALSE ;


/**============================================================================
*@name   struct Event_Packet_tag
*
*@desc   Structure of Event Packet read from notify kernel-side.
*
*@field  element
*List element header
*@field  eventNo
*Event Number.
*@field  data
*Data associated with event.
*@field  func
*User callback function.
*@field  param
*User callback argument.
*@field  isExit
*Tells whether to terminate notify thread.
*============================================================================
*/
struct Event_Packet_tag {
struct list_head  element;
u32                      procId       ;
u32       	eventNo      ;
u32       data         ;
FnNotifyCbck func         ;
void *param        ;
bool         isExit       ;
};


/**============================================================================
*@name   struct Event_Cbck_tag
*
*@desc   Structure of Event callback argument passed to register fucntion.
*
*@field  element
*List element header
*@field  func
*User callback function.
*@field  param
*User callback argument.
*============================================================================
*/
struct Event_Cbck_tag {
struct list_head  element      ;
u32 procId       ;
FnNotifyCbck func         ;
void *param        ;
u32       pid          ;
};


/**============================================================================
*@name   struct Event_State_tag
*
*@desc   Keeps the information related to Event.
*
*@field  bufList
*head of received event list.
*@field  pid
*process group ID.
*@field  refCount
*reference count, used when multiple notify_register is called
*from same process space(multi threads/processes).
*@field  semObj
*Semphore for waiting on event.
*============================================================================
*/
struct Event_State_tag {
struct lst_list *bufList  ;
u32          pid      ;
u32          refCount ;
struct semaphore sem;
};

static struct semaphore  drv_notify_sem;

/**----------------------------------------------------------------------------
*@name   major
*
*@desc   Major number of driver.
*----------------------------------------------------------------------------
*/
static signed long int major = 232 ;

/**----------------------------------------------------------------------------
*@name   Event_cbckList
*
*@desc   list containg callback arguments for all registered handlers from
*user mode.
*----------------------------------------------------------------------------
*/
static struct lst_list *Event_cbckList;

/**----------------------------------------------------------------------------
*@name   Event_stateObj
*
*@desc   List for all user processes registered.
*----------------------------------------------------------------------------
*/
static struct Event_State_tag Event_stateObj[MAX_PROCESSES] ;

/*----------------------------------------------------------------------------
*@name   Event_lockHandle
*
*@desc   Handle to lock used by spinlock.
*----------------------------------------------------------------------------
*/
static struct SYNC_CSOBJECT *hEvent_lockHandle;
static spinlock_t drv_notify_lock;
/*
*	Notify_StateObject
*/

/**----------------------------------------------------------------------------
*@name   DRV_open
*
*@desc   Linux driver function to open the driver object.
*
*@arg    inode
*inode pointer.
*filp
*File structure pointer.
*
*@ret    0
*Success.
*non-zero
*Failure.
*
*@enter  None.
*
*@leave  None.
*
*@see    None.

*----------------------------------------------------------------------------
*/
static int DRV_open(struct inode *inode, struct file *filp) ;


/**----------------------------------------------------------------------------
*@name   DRV_close
*
*@desc   Linux driver function to close the driver object.
*
*@arg    inode
*inode pointer.
*filp
*File structure pointer.
*
*@ret    0
*Success.
*non-zero
*Failure.
*
*@enter  None.
*
*@leave  None.
*
*@see    None.

*----------------------------------------------------------------------------
*/
static int DRV_close(struct inode *inode, struct file *filp) ;


/**----------------------------------------------------------------------------
*@func   DRV_read
*
*@desc   read function for of Linux Notify driver.
*
*@arg    inode
*inode pointer.
*@arg    dst
*Buffer to be filled.
*@arg    size
*number of bytes to read.
*@arg    offset
*offset from where to read.
*
*@ret    0
*Operation successfully completed.
*non-zero
*failure occured.
*
*@enter  None
*
*@leave  None
*
*@see    None
*----------------------------------------------------------------------------
*/
static int DRV_read(struct file *filp, char *dst, size_t  size, loff_t *offset);


/**----------------------------------------------------------------------------
*@func   DRV_ioctl
*
*@desc   ioctl function for of Linux Notify driver.
*
*@arg    inode
*inode pointer.
*@arg    filp
*file pointer.
*@arg    cmd
*IOCTL command id.
*@arg    args
*arguments for the command.
*
*@ret    0
*Operation successfully completed.
*non-zero
*failure occured.
*
*@enter  None
*
*@leave  None
*
*@see    None
*----------------------------------------------------------------------------
*/
static int DRV_ioctl(struct inode *inode, struct file *filp,
unsigned int cmd, unsigned long  args) ;


/**----------------------------------------------------------------------------
*@func   DRV_initializeModule
*
*@desc   Module initialization function for Linux driver.
*
*@arg    None.
*
*@ret    0
*Successful execution.
*non-zero
*Failure.
*
*@enter  None
*
*@leave  None
*
*@see    None
*----------------------------------------------------------------------------
*/
static int __init DRV_initializeModule(void) ;


/**----------------------------------------------------------------------------
*@func   DRV_finalizeModule
*
*@desc   Module finalization  function for Linux driver.
*
*@arg    None.
*
*@ret    None.
*
*@enter  None
*
*@leave  None
*
*@see    None
*----------------------------------------------------------------------------
*/
static void  __exit DRV_finalizeModule(void) ;


/**----------------------------------------------------------------------------
*@name   DriverOps
*
*@desc   Function to invoke the APIs through ioctl.
*----------------------------------------------------------------------------
*/
static struct file_operations driverOps = {

.open = DRV_open,
.ioctl = DRV_ioctl,
.release = DRV_close,
.read = DRV_read,

} ;





/**----------------------------------------------------------------------------
*@func   DRV_initializeModule
*
*@desc   Module initialization  function for Linux driver.
*----------------------------------------------------------------------------
*/
static int __init DRV_initializeModule(void)
{
	u32   status    = NOTIFY_SOK ;
	int             result    = 0 ;
	struct Notify_Attrs    attrs ;
	u32          i ;

/*Display the version info and created date/time */


/*--------------------------------------------------------------------
*To enable trace for a component and/or subcomponent, uncomment the
*corresponding statements below.(This is not a comprehensive list
*of available trace masks. See file _signature.h)
*--------------------------------------------------------------------
*/

	TRC_ENABLE(ID_KNL_NOTIFY_ALL);

/*TRC_ENABLE(ID_KNL_NOTIFY_DRIVER)  ; */
/*TRC_ENABLE(ID_DRV_IPCNOTIFY)      ; */

/*--------------------------------------------------------------------
*To set desired severity level for trace, uncomment the statement
*below and change the argument to the function below.
*--------------------------------------------------------------------
*/

/*TRC_SET_SEVERITY(TRC_ENTER) ; */


	result = register_chrdev(major, "ipcnotify", &driverOps);

	if (result < 0) {
		status = NOTIFY_EFAIL ;
		SET_FAILURE_REASON(status) ;

		TRC_1PRINT(TRC_LEVEL7,
		"Linux API register_chrdev returned error: %d\n",
		result) ;
	}

	if (NOTIFY_SUCCEEDED(status)) {
		/*TBD: what about future */
		attrs.maxDrivers = 1 ;
		status = Notify_init(&attrs) ;

	if (NOTIFY_FAILED(status))
		SET_FAILURE_REASON(status) ;

	}

	if (NOTIFY_SUCCEEDED(status)) {
		/*initializes the list */
		Event_cbckList =
		MEM_Calloc(sizeof(struct lst_list), MEM_NONPAGED);

		INIT_LIST_HEAD(&Event_cbckList->head) ;

		if (Event_cbckList == NULL) {
			status = NOTIFY_EFAIL ;
			SET_FAILURE_REASON(status);
		}
	}

	if (NOTIFY_SUCCEEDED(status))
		spin_lock_init(&drv_notify_lock);

	if (NOTIFY_SUCCEEDED(status)) {
		for (i = 0 ; i < MAX_PROCESSES ; i++) {

			Event_stateObj[i].bufList =
			MEM_Calloc(sizeof(struct lst_list),
			MEM_NONPAGED);

			INIT_LIST_HEAD(&Event_stateObj[i].
			bufList->head);

			Event_stateObj[i].pid     = -1   ;
			Event_stateObj[i].refCount = 0   ;
		}
		Event_IsInitialized = TRUE ;
	}
	TRC_1LEAVE("DRV_initializeModule", result) ;

	return result ;
}


/**--------------------------------------------------------------------------
*@name   DRV_finalizeModule
*
*@desc   Linux driver function to finalize the driver module.
*----------------------------------------------------------------------------
*/
static void __exit DRV_finalizeModule(void)
{
	u32  status = NOTIFY_SOK ;
	u32     tmpStatus = DSP_SOK ;
	struct Event_Packet_tag *packet              ;
	struct Event_Cbck_tag *cbck                ;
	u32         i ;
	TRC_0ENTER("DRV_finalizeModule") ;

	unregister_chrdev(major, "ipcnotify") ;

	status = Notify_exit() ;

	if (NOTIFY_FAILED(status))
		SET_FAILURE_REASON(status) ;


	for (i = 0 ; i < MAX_PROCESSES ; i++) {

		while (!list_empty((struct list_head *)
		Event_stateObj[i].bufList)) {

			packet =
			(struct Event_Packet_tag *)(Event_stateObj[i].bufList);

			if ((DSP_SUCCEEDED(tmpStatus)) && (packet != NULL))
				MEM_Free(packet) ;
		}

	if (Event_stateObj[i].bufList)
		list_del((struct list_head *)Event_stateObj[i].bufList);
	}


	while (!list_empty((struct list_head *)Event_cbckList)) {

		cbck = (struct Event_Cbck_tag *)(Event_cbckList) ;

		if ((DSP_SUCCEEDED(tmpStatus)) && (cbck != NULL))
			MEM_Free(cbck) ;
	}


	if (Event_cbckList)
		list_del((struct list_head *)Event_cbckList) ;

	Event_IsInitialized = FALSE ;

	SYNC_DeleteCS(hEvent_lockHandle) ;

	TRC_0LEAVE("DRV_finalizeModule") ;
}


/**----------------------------------------------------------------------------
*@func   NOTIFY_KnlMatchEvent
*
*@desc   This function matches an event listener with a list element.
*
*@modif  None.
*----------------------------------------------------------------------------
*/
static bool Drv_KnlMatchEventCbck(struct list_head *elem, void *data)
{
	bool           retVal    = FALSE ;
	struct Event_Cbck_tag *regInfo   = NULL  ;
	struct Event_Cbck_tag *unRegInfo = NULL  ;
	TRC_2ENTER("Drv_KnlMatchEventCbck", elem, data) ;

	DBC_Require(elem != NULL) ;

	regInfo   = (struct Event_Cbck_tag *) elem ;
	unRegInfo = (struct Event_Cbck_tag *) data ;

	if ((regInfo->func    == unRegInfo->func)
	&& (regInfo->param   == unRegInfo->param)
	&& (regInfo->pid     == unRegInfo->pid)
	&& (regInfo->procId   == unRegInfo->procId))
		retVal = TRUE ;


	TRC_1ENTER("Drv_KnlMatchEventCbck", retVal) ;

	return retVal ;
}


/**----------------------------------------------------------------------------
*@func   NOTIFY_KnlCallback
*
*@desc   This function implements the callback registered with IPS. Here
*to pass event no. back to user function(so that it can do another
*level of demultiplexing of callbacks)
*
*This function is called in ISR context on Linux.
*
*@modif  None.
*----------------------------------------------------------------------------
*/
static void Notify_drvCallback(IN  unsigned long int procId, IN u32 eventNo,
IN OPT void *arg,  IN OPT u32 payload)
{
	u32     status = DSP_SOK ;
	bool           flag   = FALSE   ;
	struct Event_Packet_tag *uBuf = NULL;
	u32         i                ;
	struct Event_Cbck_tag *cbck ;
	unsigned long irqFlags;
	TRC_3ENTER("Notify_drvCallback", eventNo, arg, payload) ;

	if (Event_IsInitialized == TRUE) {

		cbck = (struct Event_Cbck_tag *) arg ;

		for (i = 0 ; (i < MAX_PROCESSES) && (flag != TRUE) ; i++) {
			if (Event_stateObj[i].pid == cbck->pid) {
				flag = TRUE ;
				break ;
			}
		}

	if (flag != TRUE) {
		status = DSP_EFAIL ;
		SET_FAILURE_REASON(status);

	} else {

	uBuf = (struct Event_Packet_tag *)
	MEM_Alloc(sizeof(struct Event_Packet_tag),
	MEM_NONPAGED);

		if (uBuf != NULL) {

			INIT_LIST_HEAD((struct list_head *)
			&uBuf->element);

			if (DSP_FAILED(status)) {
				MEM_Free(uBuf) ;
				SET_FAILURE_REASON(status) ;
			} else {

				uBuf->procId  = cbck->procId ;
				uBuf->data    = payload ;
				uBuf->eventNo = eventNo ;
				uBuf->func    = cbck->func ;
				uBuf->param   = cbck->param ;
				spin_lock_irqsave(&drv_notify_lock,
				irqFlags);

				list_add_tail
				((struct list_head *)&(uBuf->element),
				(struct list_head *)
				Event_stateObj[i].bufList);

				spin_unlock(&drv_notify_lock);

					if (DSP_FAILED(status)) {
						MEM_Free(uBuf) ;
						SET_FAILURE_REASON
						(status);


					} else {

						up(&(Event_stateObj
						[i].sem));
					}
				}
			}
		}
	} else {
		status = DSP_EFAIL ;
		SET_FAILURE_REASON(status) ;
	}

	TRC_1LEAVE("Notify_drvCallback", status) ;
}


/**----------------------------------------------------------------------------
*@name   DRV_open
*
*@desc   Linux specific function to open the driver.
*----------------------------------------------------------------------------
*/
static int DRV_open(struct inode *inode, struct file *filp)
{
	u32      status  = DSP_SOK ;
	short int            flag    = FALSE ;
	u32          pid     = (u32) current->mm ;
	u32          doInit  = 0 ;
	struct lst_list *bufList = NULL ;
	s32           osStatus = 0 ;
	u32          i ;
	unsigned long irqFlags;


	TRC_2ENTER("DRV_open", inode, filp) ;

	if (Event_IsInitialized == TRUE) {

		spin_lock_irqsave(&drv_notify_lock,
		irqFlags) ;

		for (i = 0 ;
		(i < MAX_PROCESSES) ; i++) {

			if (Event_stateObj[i].pid == pid) {
				Event_stateObj[i].refCount++ ;
				doInit = 1 ;
				break ;
			}
		}

		spin_unlock_irqrestore(&drv_notify_lock,
		irqFlags) ;

		if (doInit == 0) {
			INIT_LIST_HEAD((struct list_head *)bufList);

			if (bufList) {
				/*Create the semaphore */
				sema_init(&drv_notify_sem, 0);

				if (DSP_SUCCEEDED(status)) {
					spin_lock_irqsave(&drv_notify_lock,
					irqFlags);

					for (i = 0 ; (i < MAX_PROCESSES)
					&& (flag != TRUE) ; i++) {

						if (Event_stateObj[i].
								pid == -1) {
							Event_stateObj[i].
							sem = drv_notify_sem;

							Event_stateObj[i].pid
									= pid;

							Event_stateObj[i].
								refCount++;

							Event_stateObj[i].
							bufList = bufList;

							flag = TRUE;
							break ;
						}
					}

					spin_unlock_irqrestore
						(&drv_notify_lock, irqFlags);

				} else
					SET_FAILURE_REASON(status) ;

				if (flag != TRUE) {
					list_del((struct list_head *)bufList);
					status = DSP_EFAIL ;
					SET_FAILURE_REASON(status) ;
				}


			} else {
				status = DSP_EFAIL
				SET_FAILURE_REASON(status) ;
			}
		}

	} else {
		status = DSP_EFAIL ;
		SET_FAILURE_REASON(status) ;
	}

	if (DSP_FAILED(status))
		osStatus = -1 ;

	TRC_1LEAVE("DRV_open", osStatus) ;
	return osStatus ;
}


/**----------------------------------------------------------------------------
*@name   DRV_close
*
*@desc   Linux specific function to close the driver.
*----------------------------------------------------------------------------
*/
static int DRV_close(struct inode *inode, struct file *filp)
{

	u32      status  = DSP_SOK ;
	bool            flag    = FALSE ;
	u32          pid     = (u32) current->mm ;
	struct lst_list *bufList = NULL ;
	u32          i ;
	unsigned long irqsave;
	TRC_2ENTER("DRV_close", inode, filp) ;

	if (Event_IsInitialized == TRUE) {

		spin_lock_irqsave(&drv_notify_lock, irqsave);


		for (i = 0 ; (i < MAX_PROCESSES) && (flag != TRUE) ; i++) {

			if (Event_stateObj[i].pid == pid) {

				if (Event_stateObj[i].refCount == 1) {

					Event_stateObj[i].pid = -1 ;
					Event_stateObj[i].refCount-- ;




					bufList = Event_stateObj[i].bufList ;

					Event_stateObj[i].bufList = NULL ;
					flag = TRUE ;
					break ;
				} else {
					Event_stateObj[i].refCount-- ;
				}
			}
		}


		spin_unlock(&drv_notify_lock);

		if (flag != TRUE) {
			if (i == MAX_PROCESSES) {
				status = DSP_EFAIL ;
				SET_FAILURE_REASON(status) ;
			}
		} else
			list_del((struct list_head *)bufList) ;


	} else {
		status = DSP_EFAIL ;
		SET_FAILURE_REASON(status) ;
	}


	TRC_1LEAVE("DRV_close", status) ;
	return 0 ;
}


/**============================================================================
*@name   DRV_Read
*
*@desc   Linux specific function to read data from the driver.
*============================================================================
*/
static int DRV_read(struct file *filp, char *dst, size_t size, loff_t *offset)
{
	short int           flag   = FALSE   ;
	struct Event_Packet_tag *uBuf   = NULL    ;
	u32         pid    = (u32) current->mm ;
	u32         retVal = 0       ;
	u32         i                ;
	unsigned long irqsave;
	TRC_4ENTER("DRV_read", filp, dst, size, offset) ;

	DBC_Require(dst != NULL) ;

	if (Event_IsInitialized == TRUE) {
		for (i = 0 ; (i < MAX_PROCESSES) && (flag != TRUE) ; i++) {
			if (Event_stateObj[i].pid == pid) {
				flag = TRUE ;
				break ;
			}
		}

		if (flag == TRUE) {
			if (Event_stateObj[i].bufList != NULL) {
				/*Wait for the event */
				if (down_interruptible(&Event_stateObj[i]
								.sem)) {
					retVal = DSP_EFAIL;
					goto func_end;
				}

				spin_lock_irqsave(&drv_notify_lock, irqsave);

				uBuf =
				(struct Event_Packet_tag *)
				(Event_stateObj[i].bufList);

				spin_unlock_irqrestore(&drv_notify_lock,
								irqsave);

					if (uBuf != NULL) {
						retVal =
						copy_to_user((void *) dst,
						uBuf,
						sizeof(struct
						Event_Packet_tag));

						if (retVal != 0) {
							TRC_1PRINT(TRC_LEVEL4,
							"copy_to_user call \
							failed %d\n",
							retVal);

						} else {
							retVal =
							sizeof(struct
							Event_Packet_tag);
						}

						kfree(uBuf) ;
					}
			}
		}
	}

	TRC_1LEAVE("DRV_read", retVal) ;
func_end:
	return retVal ;
}


/**----------------------------------------------------------------------------
*@name   DRV_ioctl
*
*@desc   ioctl function for of Linux Notify driver.
*----------------------------------------------------------------------------
*/
static int DRV_ioctl(struct inode *inode,
struct file *filp,
unsigned int   cmd,
unsigned long  args)
{
	u32        status   = NOTIFY_SOK ;
	u32           tmpStatus = DSP_SOK ;
	int                  osStatus = 0       ;
	int                  retVal   = 0       ;
	struct Notify_CmdArgs *srcArgs  =
					(struct Notify_CmdArgs *) args;

	char                driverName[NOTIFY_MAX_NAMELEN]  ;
	struct Notify_Config        config             ;
	struct Notify_CmdArgs       uArgs              ;
	void *handle             ;
	struct Event_Cbck_tag *cbck               ;
	struct Event_Cbck_tag           tCbck              ;
	void *tmpUsrPtr          ;
	unsigned long irqsave;

	TRC_4ENTER("DRV_ioctl", inode, filp, cmd, args) ;

	retVal = copy_from_user((void *) &uArgs,
			(const void *) srcArgs,
			sizeof(struct Notify_CmdArgs)) ;

	if (retVal != 0) {
		osStatus = -EFAULT ;
		SET_FAILURE_REASON(NOTIFY_EFAIL) ;
	}

	if (osStatus == 0) {

		switch (cmd) {

		case NOTIFY_DRV_CMD_DRIVERINIT:
		{
			/*Get the user passed pointers */
			retVal = copy_from_user(
			(void *) &config,
			(const void *) uArgs.apiArgs.driverInitArgs.config,
			sizeof(struct Notify_Config)) ;

				if (retVal != 0) {
					osStatus = -EFAULT ;
					SET_FAILURE_REASON(NOTIFY_EFAIL) ;
				}

				if (retVal == 0) {

					tmpUsrPtr = config.driverAttrs ;

					retVal = copy_from_user
					((void *) config.driverAttrs,
					(const void *) tmpUsrPtr,
					NOTIFY_MAX_NAMELEN);


					if (retVal != 0) {
						osStatus = -EFAULT ;
						SET_FAILURE_REASON(
							NOTIFY_EFAIL);
					}
				}


			if (retVal == 0) {

				retVal = copy_from_user(
				(void *) driverName,
				(const void *)
				uArgs.apiArgs.driverInitArgs.
				driverName,
				NOTIFY_MAX_NAMELEN);

				if (retVal != 0) {
					osStatus = -EFAULT ;
					SET_FAILURE_REASON(NOTIFY_EFAIL);
				}
			}

			if (retVal == 0) {
				status = Notify_driverInit
				(driverName, &config, &handle);

				if (NOTIFY_FAILED(status)) {
					SET_FAILURE_REASON(status);
				} else {
					uArgs.apiArgs.driverInitArgs.handle
					= handle ;
				}
			}
		}
		break ;



		case NOTIFY_DRV_CMD_DRIVEREXIT:
		{
			handle = uArgs.apiArgs.driverExitArgs.handle;
			status = Notify_driverExit(handle);
				if (NOTIFY_FAILED(status))
					SET_FAILURE_REASON(status);

		}
		break ;


		case NOTIFY_DRV_CMD_REGISTEREVENT:
		{
			handle = uArgs.apiArgs.registerEventArgs.handle,

			cbck =
			MEM_Alloc(sizeof(struct Event_Cbck_tag),
			MEM_NONPAGED) ;

			if (DSP_FAILED(tmpStatus)) {
				status = NOTIFY_EMEMORY;
				SET_FAILURE_REASON(status);

			} else {

				cbck->procId =
				uArgs.apiArgs.registerEventArgs.procId;

				cbck->func  =
				uArgs.apiArgs.registerEventArgs.fnNotifyCbck;

				cbck->param =
				uArgs.apiArgs.registerEventArgs.cbckArg;

				cbck->pid   = (u32) current->mm;

				status = Notify_registerEvent(
				handle,
				uArgs.apiArgs.registerEventArgs.procId,
				uArgs.apiArgs.registerEventArgs.eventNo,
				(void *)Notify_drvCallback,
				(void *) cbck) ;


				if (NOTIFY_FAILED(status)) {
						MEM_Free(cbck) ;
						SET_FAILURE_REASON(status);
				} else {
						spin_lock_irqsave
						(&drv_notify_lock,
							irqsave);

						INIT_LIST_HEAD(
							&(cbck->element));

						DBC_Assert(
						DSP_SUCCEEDED(tmpStatus));


						list_add_tail
						((struct list_head *)
						&(cbck->element),
						(struct list_head *)
						Event_cbckList);

						spin_unlock_irqrestore
						(&drv_notify_lock, irqsave);
				}
			}
		}
		break ;

		case NOTIFY_DRV_CMD_UNREGISTEREVENT:
		{
			handle =
			uArgs.apiArgs.unregisterEventArgs.handle;

			tCbck.func  =
			uArgs.apiArgs.unregisterEventArgs.fnNotifyCbck;

			tCbck.param =
			uArgs.apiArgs.unregisterEventArgs.cbckArg;

			spin_lock_irqsave(&drv_notify_lock, irqsave);

			tmpStatus = omap_list_search(
			Event_cbckList,
			(void *) &tCbck,
			(struct list_head **) &cbck,
			(ListMatchFunc)
			(&Drv_KnlMatchEventCbck));

			spin_unlock_irqrestore(&drv_notify_lock, irqsave);

			if (DSP_FAILED(tmpStatus)) {
				status = NOTIFY_EFAIL ;
				SET_FAILURE_REASON(status) ;
			} else {
				status = Notify_unregisterEvent(
				handle,
				uArgs.apiArgs.unregisterEventArgs.
				procId,
				uArgs.apiArgs.unregisterEventArgs.
				eventNo,
				Notify_drvCallback,
				(void *) cbck) ;

				if (NOTIFY_FAILED(status))
					SET_FAILURE_REASON(status) ;
				else {
					spin_lock_irqsave
					(&drv_notify_lock, irqsave);

					list_del((struct list_head *)&cbck);

					MEM_Free(cbck) ;

					spin_unlock_irqrestore
					(&drv_notify_lock, irqsave);
				}
			}
		}
		break ;


		case NOTIFY_DRV_CMD_SENDEVENT:
		{
			handle = uArgs.apiArgs.sendEventArgs.handle ;
			status = Notify_sendEvent(
			handle,
			uArgs.apiArgs.sendEventArgs.procId,
			uArgs.apiArgs.sendEventArgs.eventNo,
			uArgs.apiArgs.sendEventArgs.payload,
			uArgs.apiArgs.sendEventArgs.waitClear);

			if (NOTIFY_FAILED(status))
				SET_FAILURE_REASON(status) ;
		}
		break ;


		case NOTIFY_DRV_CMD_DISABLE:
		{
			uArgs.apiArgs.disableArgs.disableFlags =
			Notify_disable() ;
		}
		break ;


		case NOTIFY_DRV_CMD_RESTORE:
		{
			status = Notify_restore
			(uArgs.apiArgs.restoreArgs.restoreFlags);

			if (NOTIFY_FAILED(status))
				SET_FAILURE_REASON(status) ;
		}
		break ;


		case NOTIFY_DRV_CMD_DISABLEEVENT:
		{
			handle = uArgs.apiArgs.disableEventArgs.handle ;

			status = Notify_disableEvent(
			handle,
			uArgs.apiArgs.disableEventArgs.procId,
			uArgs.apiArgs.disableEventArgs.eventNo);

			if (NOTIFY_FAILED(status))
				SET_FAILURE_REASON(status) ;

		}
		break ;


		case NOTIFY_DRV_CMD_ENABLEEVENT:
		{
			handle = uArgs.apiArgs.enableEventArgs.handle ;

			status = Notify_enableEvent(
			handle,
			uArgs.apiArgs.enableEventArgs.procId,
			uArgs.apiArgs.enableEventArgs.eventNo);

			if (NOTIFY_FAILED(status))
				SET_FAILURE_REASON(status) ;

		}
		break ;

	}

	if (NOTIFY_SUCCEEDED(status))
		uArgs.apiStatus = status ;

	}

	if (osStatus == 0) {
		retVal = copy_to_user((void *) srcArgs,
		(const void *) &uArgs,
		sizeof(struct Notify_CmdArgs)) ;

		if (retVal != 0) {
			osStatus = -EFAULT ;
			SET_FAILURE_REASON(NOTIFY_EFAIL) ;
		}
	}

	TRC_1LEAVE("DRV_ioctl", status) ;

	return osStatus ;
}


signed long int omap_list_search(IN  struct lst_list *list,
IN  void  *data,
OUT struct list_head **elem,
IN  ListMatchFunc  matchFunc)

{
	u32          status = DSP_SOK ;
	struct list_head *temp   = NULL    ;
	struct list_head *temp1  = NULL    ;
	bool                found  = FALSE   ;

	DBC_Require(list != NULL) ;
	DBC_Require(elem != NULL) ;
	DBC_Require(matchFunc != NULL) ;

	if ((list == NULL) || (elem == NULL)) {
		status = DSP_EINVALIDARG ;
	} else {

		if (list_empty((struct list_head *)list))
			status = DSP_EINVALIDARG ;

		if (DSP_SUCCEEDED(status)) {
			temp = list->head.next;

			while ((found == FALSE) && (temp != NULL)) {
				if ((*matchFunc) (temp, data) == TRUE)
					found = TRUE ;
				else {
					temp1 = temp ;

					if (temp1->next !=
						(struct list_head *)list)
						temp = temp1->next;
				}
			}

			if (found == TRUE)
				*elem = temp ;
			else {
				*elem = NULL ;
				status = DSP_ENOTFOUND ;
			}
		}
	}
	return status;

}






/**============================================================================
*@name   module_init/module_exit
*
*@desc   Macro calls that indicate initialization and finalization functions
*to the kernel.

*============================================================================
*/

module_init(DRV_initializeModule) ;
module_exit(DRV_finalizeModule) ;
MODULE_LICENSE("Dual BSD/GPL");
