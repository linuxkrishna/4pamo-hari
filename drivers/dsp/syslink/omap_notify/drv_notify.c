
/*
 * drv_notify.c
 *
 * Syslink support functions for TI OMAP processors.
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
#include <gt.h>

/*----------------------------------- OSAL Headers                */
#include <_drvdefs.h>

/*----------------------------------- gen headers                 */
#include <mem.h>


/*----------------------------------- Notify headers              */
#include <notify_driver.h>
#include <gpptypes.h>
#include <notifyerr.h>
#include <notify.h>


#include <global_var.h>


/*
* Maximum number of user supported.
*/
#define  MAX_PROCESSES 32u

/*Flag for indicating initialization status.*/
static bool event_is_initialized = false ;

/*Structure of Event Packet read from notify kernel-side..*/
struct event_packet_tag {
	struct list_head  element;
	u32     proc_id;
	u32     event_no;
	u32     data;
	fn_notify_cbck func;
	void *param;
	bool  is_exit;
};

/*Structure of Event callback argument passed to register fucntion*/
struct event_cbck_tag {
	struct list_head  element;
	u32 proc_id;
	fn_notify_cbck func;
	void *param;
	u32    pid;
};

/*Keeps the information related to Event.*/
struct event_state_tag {
	struct lst_list *buf_list;
	u32  pid;
	u32  ref_count;
	struct semaphore sem;
};

static struct semaphore  drv_notify_sem;

/*Major number of driver.*/
static signed long int major = 232 ;

/*list containg callback arguments for all registered handlers from
* user mode.*/
static struct lst_list *event_cbck_list;

/*List for all user processes registered.*/
static struct event_state_tag event_state_obj[MAX_PROCESSES] ;

/* Handle to lock used by spinlock.*/
static spinlock_t drv_notify_lock;

/* open the Notify driver object..*/
static int drv_open(struct inode *inode, struct file *filp) ;

/* close the Notify driver object..*/
static int drv_close(struct inode *inode, struct file *filp) ;

/* read function for of  Notify driver.*/
static int drv_read(struct file *filp, char *dst, size_t  size, loff_t *offset);

/* ioctl function for of Linux Notify driver.*/
static int drv_ioctl(struct inode *inode, struct file *filp, unsigned int cmd,
						unsigned long  args);

/* Module initialization function for Linux driver.*/
static int __init drv_init_module(void);

/* Module finalization  function for Linux driver.*/
static void  __exit drv_finalize_module(void) ;

/* Function to invoke the APIs through ioctl.*/
static struct file_operations driver_ops = {
	.open = drv_open,
	.ioctl = drv_ioctl,
	.release = drv_close,
	.read = drv_read,
};

#if defined(NOTIFY_DEBUG)
#define SET_FAILURE_REASON(status)
#else
#define SET_FAILURE_REASON(status)
#endif /*if defined(NOTIFY_DEBUG) */

/* Module initialization  function for Notify driver.*/
static int __init drv_init_module(void)
{
	int result = 0 ;
	struct notify_attrs attrs ;
	u32 i ;

	result = register_chrdev(major, "ipcnotify", &driver_ops);

	if (result < 0) {
		pr_err("Notify driver initialization failure - 1\n");
		goto func_end;
	}

	/*TBD: what about future */
	attrs.maxDrivers = 1 ;
	result  = notify_init(&attrs) ;
	if (result < 0) {
		pr_err("Notify driver initialization failure - 2\n");
		goto func_end;
	}

	/*initializes the list */
	event_cbck_list = mem_calloc(sizeof(struct lst_list), MEM_NONPAGED);

	INIT_LIST_HEAD(&event_cbck_list->head);
	BUG_ON(event_cbck_list == NULL);

	spin_lock_init(&drv_notify_lock);
	for (i = 0 ; i < MAX_PROCESSES ; i++) {

		event_state_obj[i].buf_list =
		mem_calloc(sizeof(struct lst_list),
		MEM_NONPAGED);

		INIT_LIST_HEAD(&event_state_obj[i].buf_list->head);
		event_state_obj[i].pid= -1;
		event_state_obj[i].ref_count = 0;
	}
	event_is_initialized = true;	
	gt_1trace(notify_debugmask, GT_ENTER, "drv_init_module", result) ;
func_end:
	return result ;
}

/* Module finalization  function for Notify driver.*/
static void __exit drv_finalize_module(void)
{
	int status = 0 ;
	u32     tmp_status = 0 ;
	struct event_packet_tag *packet;
	struct event_cbck_tag *cbck;
	u32 i ;
	gt_0trace(notify_debugmask, GT_ENTER, "drv_finalize_module") ;
	unregister_chrdev(major, "ipcnotify") ;

	status = notify_exit() ;

	if (!status)
		goto func_end;

	for (i = 0 ; i < MAX_PROCESSES ; i++) {
		while (!list_empty((struct list_head *)
		event_state_obj[i].buf_list)) {
			packet = 	(struct event_packet_tag *)
				(event_state_obj[i].buf_list);
			if (packet != NULL)
				mem_free(packet) ;
		}
		if (event_state_obj[i].buf_list)
			list_del((struct list_head *)event_state_obj[i].buf_list);
	}

	while (!list_empty((struct list_head *)event_cbck_list)) {
		cbck = (struct event_cbck_tag *)(event_cbck_list) ;
		if (cbck != NULL)
			mem_free(cbck) ;
	}
	if (event_cbck_list)
		list_del((struct list_head *)event_cbck_list) ;
	event_is_initialized = false;
func_end:
	gt_0trace(notify_debugmask, GT_ENTER, "drv_finalize_module") ;
	return;
}

/* This function matches an event listener with a list element..*/
static bool drv_knlmatch_eventcbck(struct list_head *elem, void *data)
{
	bool  ret_val = false ;
	struct event_cbck_tag *reg_info = NULL;
	struct event_cbck_tag *unreg_info = NULL;
	gt_2trace(notify_debugmask, GT_ENTER,
		"drv_knlmatch_eventcbck", elem, data) ;

	BUG_ON(elem == NULL);

	reg_info   = (struct event_cbck_tag *) elem ;
	unreg_info = (struct event_cbck_tag *) data ;

	if ((reg_info->func    == unreg_info->func)
	&& (reg_info->param   == unreg_info->param)
	&& (reg_info->pid     == unreg_info->pid)
	&& (reg_info->proc_id   == unreg_info->proc_id))
		ret_val = true ;
	gt_1trace(notify_debugmask, GT_ENTER,
		"drv_knlmatch_eventcbck", ret_val) ;

	return ret_val ;
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
static void notify_drv_cbck(IN  unsigned long int proc_id, IN u32 event_no,
				IN OPT void *arg,  IN OPT u32 payload)
{
	u32     status = NOTIFY_SOK ;
	bool           flag   = FALSE   ;
	struct event_packet_tag *u_buf = NULL;
	u32         i                ;
	struct event_cbck_tag *cbck ;
	unsigned long irqFlags;
	gt_3trace(notify_debugmask, GT_ENTER,
		"notify_drv_cbck", event_no, arg, payload) ;

	if (event_is_initialized == TRUE) {

		cbck = (struct event_cbck_tag *) arg ;

		for (i = 0 ; (i < MAX_PROCESSES) && (flag != TRUE) ; i++) {
			if (event_state_obj[i].pid == cbck->pid) {
				flag = TRUE ;
				break ;
			}
		}

	if (flag != TRUE) {
		status = NOTIFY_EFAIL ;
		SET_FAILURE_REASON(status);

	} else {

	u_buf = (struct event_packet_tag *)
	mem_alloc(sizeof(struct event_packet_tag),
	MEM_NONPAGED);

		if (u_buf != NULL) {

			INIT_LIST_HEAD((struct list_head *)
			&u_buf->element);

			if (NOTIFY_FAILED(status)) {
				mem_free(u_buf) ;
				SET_FAILURE_REASON(status) ;
			} else {

				u_buf->proc_id  = cbck->proc_id ;
				u_buf->data    = payload ;
				u_buf->event_no = event_no ;
				u_buf->func    = cbck->func ;
				u_buf->param   = cbck->param ;
				spin_lock_irqsave(&drv_notify_lock,
				irqFlags);

				list_add_tail
				((struct list_head *)&(u_buf->element),
				(struct list_head *)
				event_state_obj[i].buf_list);

				spin_unlock(&drv_notify_lock);

					if (NOTIFY_FAILED(status)) {
						mem_free(u_buf) ;
						SET_FAILURE_REASON
						(status);


					} else {

						up(&(event_state_obj
						[i].sem));
					}
				}
			}
		}
	} else {
		status = NOTIFY_EFAIL ;
		SET_FAILURE_REASON(status) ;
	}

	gt_1trace(notify_debugmask, GT_5CLASS, "notify_drv_cbck", status) ;
}


/**----------------------------------------------------------------------------
*@name   drv_open
*
*@desc   Linux specific function to open the driver.
*----------------------------------------------------------------------------
*/
static int drv_open(struct inode *inode, struct file *filp)
{
	u32      status  = NOTIFY_SOK ;
	short int            flag    = FALSE ;
	u32          pid     = (u32) current->mm ;
	u32          doInit  = 0 ;
	struct lst_list *buf_list = NULL ;
	s32           os_status = 0 ;
	u32          i ;
	unsigned long irqFlags;


	gt_2trace(notify_debugmask, GT_ENTER, "drv_open", inode, filp) ;

	if (event_is_initialized == TRUE) {

		spin_lock_irqsave(&drv_notify_lock,
		irqFlags) ;

		for (i = 0 ;
		(i < MAX_PROCESSES) ; i++) {

			if (event_state_obj[i].pid == pid) {
				event_state_obj[i].ref_count++ ;
				doInit = 1 ;
				break ;
			}
		}

		spin_unlock_irqrestore(&drv_notify_lock,
		irqFlags) ;

		if (doInit == 0) {
			INIT_LIST_HEAD((struct list_head *)buf_list);

			if (buf_list) {
				/*Create the semaphore */
				sema_init(&drv_notify_sem, 0);

				if (NOTIFY_SUCCEEDED(status)) {
					spin_lock_irqsave(&drv_notify_lock,
					irqFlags);

					for (i = 0 ; (i < MAX_PROCESSES)
					&& (flag != TRUE) ; i++) {

						if (event_state_obj[i].
								pid == -1) {
							event_state_obj[i].
							sem = drv_notify_sem;

							event_state_obj[i].pid
									= pid;

							event_state_obj[i].
								ref_count++;

							event_state_obj[i].
							buf_list = buf_list;

							flag = TRUE;
							break ;
						}
					}

					spin_unlock_irqrestore
						(&drv_notify_lock, irqFlags);

				} else
					SET_FAILURE_REASON(status) ;

				if (flag != TRUE) {
					list_del((struct list_head *)buf_list);
					status = NOTIFY_EFAIL ;
					SET_FAILURE_REASON(status) ;
				}


			} else {
				status = NOTIFY_EFAIL
				SET_FAILURE_REASON(status) ;
			}
		}

	} else {
		status = NOTIFY_EFAIL ;
		SET_FAILURE_REASON(status) ;
	}

	if (NOTIFY_FAILED(status))
		os_status = -1 ;

	gt_1trace(notify_debugmask, GT_5CLASS, "drv_open", os_status) ;
	return os_status ;
}


/**----------------------------------------------------------------------------
*@name   drv_close
*
*@desc   Linux specific function to close the driver.
*----------------------------------------------------------------------------
*/
static int drv_close(struct inode *inode, struct file *filp)
{

	u32      status  = NOTIFY_SOK ;
	bool            flag    = FALSE ;
	u32          pid     = (u32) current->mm ;
	struct lst_list *buf_list = NULL ;
	u32          i ;
	unsigned long irqsave;
	gt_2trace(notify_debugmask, GT_ENTER, "drv_close", inode, filp) ;

	if (event_is_initialized == TRUE) {

		spin_lock_irqsave(&drv_notify_lock, irqsave);


		for (i = 0 ; (i < MAX_PROCESSES) && (flag != TRUE) ; i++) {

			if (event_state_obj[i].pid == pid) {

				if (event_state_obj[i].ref_count == 1) {

					event_state_obj[i].pid = -1 ;
					event_state_obj[i].ref_count-- ;




					buf_list = event_state_obj[i].buf_list ;

					event_state_obj[i].buf_list = NULL ;
					flag = TRUE ;
					break ;
				} else {
					event_state_obj[i].ref_count-- ;
				}
			}
		}


		spin_unlock(&drv_notify_lock);

		if (flag != TRUE) {
			if (i == MAX_PROCESSES) {
				status = NOTIFY_EFAIL ;
				SET_FAILURE_REASON(status) ;
			}
		} else
			list_del((struct list_head *)buf_list) ;


	} else {
		status = NOTIFY_EFAIL ;
		SET_FAILURE_REASON(status) ;
	}


	gt_1trace(notify_debugmask, GT_5CLASS, "drv_close", status) ;
	return 0 ;
}


/**============================================================================
*@name   DRV_Read
*
*@desc   Linux specific function to read data from the driver.
*============================================================================
*/
static int drv_read(struct file *filp, char *dst, size_t size, loff_t *offset)
{
	short int           flag   = FALSE   ;
	struct event_packet_tag *u_buf   = NULL    ;
	u32         pid    = (u32) current->mm ;
	u32         ret_val = 0       ;
	u32         i                ;
	unsigned long irqsave;
	gt_4trace(notify_debugmask, GT_ENTER,
		"drv_read", filp, dst, size, offset) ;

	dbc_require(dst != NULL) ;

	if (event_is_initialized == TRUE) {
		for (i = 0 ; (i < MAX_PROCESSES) && (flag != TRUE) ; i++) {
			if (event_state_obj[i].pid == pid) {
				flag = TRUE ;
				break ;
			}
		}

		if (flag == TRUE) {
			if (event_state_obj[i].buf_list != NULL) {
				/*Wait for the event */
				if (down_interruptible(&event_state_obj[i]
								.sem)) {
					ret_val = NOTIFY_EFAIL;
					goto func_end;
				}

				spin_lock_irqsave(&drv_notify_lock, irqsave);

				u_buf =
				(struct event_packet_tag *)
				(event_state_obj[i].buf_list);

				spin_unlock_irqrestore(&drv_notify_lock,
								irqsave);

					if (u_buf != NULL) {
						ret_val =
						copy_to_user((void *) dst,
						u_buf,
						sizeof(struct
						event_packet_tag));

						if (ret_val != 0) {
#if 0
TODO: Change this to gt trace
							trc_1PRINT(trc_LEVEL4,
							"copy_to_user call \
							failed %d\n",
							ret_val);
#endif

						} else {
							ret_val =
							sizeof(struct
							event_packet_tag);
						}

						kfree(u_buf) ;
					}
			}
		}
	}

	gt_1trace(notify_debugmask, GT_5CLASS, "drv_read", ret_val) ;
func_end:
	return ret_val ;
}


/**----------------------------------------------------------------------------
*@name   drv_ioctl
*
*@desc   ioctl function for of Linux Notify driver.
*----------------------------------------------------------------------------
*/
static int drv_ioctl(struct inode *inode,
		struct file *filp,
		unsigned int   cmd,
		unsigned long  args)

{
	u32        status   = NOTIFY_SOK ;
	u32           tmp_status = NOTIFY_SOK ;
	int                  os_status = 0       ;
	int                  ret_val   = 0       ;
	struct notify_cmd_args *srcArgs  =
					(struct notify_cmd_args *) args;

	char                driver_name[NOTIFY_MAX_NAMELEN]  ;
	struct notify_config        config             ;
	struct notify_cmd_args       uargs              ;
	void *handle             ;
	struct event_cbck_tag *cbck               ;
	struct event_cbck_tag           tCbck              ;
	void *tmpUsrPtr          ;
	unsigned long irqsave;

	gt_4trace(notify_debugmask, GT_ENTER,
		"drv_ioctl", inode, filp, cmd, args) ;

	ret_val = copy_from_user((void *) &uargs,
			(const void *) srcArgs,
			sizeof(struct notify_cmd_args)) ;

	if (ret_val != 0) {
		os_status = -EFAULT ;
		SET_FAILURE_REASON(NOTIFY_EFAIL) ;
	}

	if (os_status == 0) {

		switch (cmd) {

		case NOTIFY_DRV_CMD_DRIVERINIT:
		{
			/*Get the user passed pointers */
			ret_val = copy_from_user(
			(void *) &config,
			(const void *) uargs.apiargs.driverInitArgs.config,
			sizeof(struct notify_config)) ;

				if (ret_val != 0) {
					os_status = -EFAULT ;
					SET_FAILURE_REASON(NOTIFY_EFAIL) ;
				}

				if (ret_val == 0) {

					tmpUsrPtr = config.driverAttrs ;

					ret_val = copy_from_user
					((void *) config.driverAttrs,
					(const void *) tmpUsrPtr,
					NOTIFY_MAX_NAMELEN);


					if (ret_val != 0) {
						os_status = -EFAULT ;
						SET_FAILURE_REASON(
							NOTIFY_EFAIL);
					}
				}


			if (ret_val == 0) {

				ret_val = copy_from_user(
				(void *) driver_name,
				(const void *)
				uargs.apiargs.driverInitArgs.
				driver_name,
				NOTIFY_MAX_NAMELEN);

				if (ret_val != 0) {
					os_status = -EFAULT ;
					SET_FAILURE_REASON(NOTIFY_EFAIL);
				}
			}

			if (ret_val == 0) {
				status = notify_driver_init
				(driver_name, &config, &handle);

				if (NOTIFY_FAILED(status)) {
					SET_FAILURE_REASON(status);
				} else {
					uargs.apiargs.driverInitArgs.handle
					= handle ;
				}
			}
		}
		break ;



		case NOTIFY_DRV_CMD_DRIVEREXIT:
		{
			handle = uargs.apiargs.driverExitArgs.handle;
			status = notify_driver_exit(handle);
				if (NOTIFY_FAILED(status))
					SET_FAILURE_REASON(status);

		}
		break ;


		case NOTIFY_DRV_CMD_REGISTEREVENT:
		{
			handle = uargs.apiargs.register_event_args.handle,

			cbck =
			mem_alloc(sizeof(struct event_cbck_tag),
			MEM_NONPAGED) ;

			if (NOTIFY_FAILED(tmp_status)) {
				status = NOTIFY_EMEMORY;
				SET_FAILURE_REASON(status);

			} else {

				cbck->proc_id =
				uargs.apiargs.register_event_args.proc_id;

				cbck->func  =
				uargs.apiargs.register_event_args.
				fn_notify_cbck;

				cbck->param =
				uargs.apiargs.register_event_args.cbck_arg;

				cbck->pid   = (u32) current->mm;

				status = notify_register_event(
				handle,
				uargs.apiargs.register_event_args.proc_id,
				uargs.apiargs.register_event_args.event_no,
				(void *)notify_drv_cbck,
				(void *) cbck) ;


				if (NOTIFY_FAILED(status)) {
						mem_free(cbck) ;
						SET_FAILURE_REASON(status);
				} else {
						spin_lock_irqsave
						(&drv_notify_lock,
							irqsave);

						INIT_LIST_HEAD(
							&(cbck->element));

						dbc_assert(
						NOTIFY_SUCCEEDED(tmp_status));


						list_add_tail
						((struct list_head *)
						&(cbck->element),
						(struct list_head *)
						event_cbck_list);

						spin_unlock_irqrestore
						(&drv_notify_lock, irqsave);
				}
			}
		}
		break ;

		case NOTIFY_DRV_CMD_UNREGISTEREVENT:
		{
			handle =
			uargs.apiargs.unregister_event_args.handle;

			tCbck.func  =
			uargs.apiargs.unregister_event_args.fn_notify_cbck;

			tCbck.param =
			uargs.apiargs.unregister_event_args.cbck_arg;

			spin_lock_irqsave(&drv_notify_lock, irqsave);

			tmp_status = ntfy_evt_cb_search(
			event_cbck_list,
			(void *) &tCbck,
			(struct list_head **) &cbck,
			(ListMatchFunc)
			(&drv_knlmatch_eventcbck));

			spin_unlock_irqrestore(&drv_notify_lock, irqsave);

			if (NOTIFY_FAILED(tmp_status)) {
				status = NOTIFY_EFAIL ;
				SET_FAILURE_REASON(status) ;
			} else {
				status = notify_unregister_event(
				handle,
				uargs.apiargs.unregister_event_args.
				proc_id,
				uargs.apiargs.unregister_event_args.
				event_no,
				notify_drv_cbck,
				(void *) cbck) ;

				if (NOTIFY_FAILED(status))
					SET_FAILURE_REASON(status) ;
				else {
					spin_lock_irqsave
					(&drv_notify_lock, irqsave);

					list_del((struct list_head *)&cbck);

					mem_free(cbck) ;

					spin_unlock_irqrestore
					(&drv_notify_lock, irqsave);
				}
			}
		}
		break ;


		case NOTIFY_DRV_CMD_SENDEVENT:
		{
			handle = uargs.apiargs.send_event_args.handle ;
			status = notify_sendevent(
			handle,
			uargs.apiargs.send_event_args.proc_id,
			uargs.apiargs.send_event_args.event_no,
			uargs.apiargs.send_event_args.payload,
			uargs.apiargs.send_event_args.wait_clear);

			if (NOTIFY_FAILED(status))
				SET_FAILURE_REASON(status) ;
		}
		break ;


		case NOTIFY_DRV_CMD_DISABLE:
		{
			uargs.apiargs.disableArgs.disable_flags =
			notify_disable() ;
		}
		break ;


		case NOTIFY_DRV_CMD_RESTORE:
		{
			status = notify_restore
			(uargs.apiargs.restoreArgs.restoreFlags);

			if (NOTIFY_FAILED(status))
				SET_FAILURE_REASON(status) ;
		}
		break ;


		case NOTIFY_DRV_CMD_DISABLEEVENT:
		{
			handle = uargs.apiargs.disable_event_args.handle ;

			status = notify_disable_event(
			handle,
			uargs.apiargs.disable_event_args.proc_id,
			uargs.apiargs.disable_event_args.event_no);

			if (NOTIFY_FAILED(status))
				SET_FAILURE_REASON(status) ;

		}
		break ;


		case NOTIFY_DRV_CMD_ENABLEEVENT:
		{
			handle = uargs.apiargs.enable_event_args.handle ;

			status = notify_enable_event(
			handle,
			uargs.apiargs.enable_event_args.proc_id,
			uargs.apiargs.enable_event_args.event_no);

			if (NOTIFY_FAILED(status))
				SET_FAILURE_REASON(status) ;

		}
		break ;

	}

	if (NOTIFY_SUCCEEDED(status))
		uargs.apiStatus = status ;

	}

	if (os_status == 0) {
		ret_val = copy_to_user((void *) srcArgs,
		(const void *) &uargs,
		sizeof(struct notify_cmd_args)) ;

		if (ret_val != 0) {
			os_status = -EFAULT ;
			SET_FAILURE_REASON(NOTIFY_EFAIL) ;
		}
	}

	gt_1trace(notify_debugmask, GT_5CLASS, "drv_ioctl", status) ;

	return os_status ;
}


signed long int ntfy_evt_cb_search(struct lst_list *list, void  *data,
			struct list_head **elem, ListMatchFunc  matchFunc)

{
	u32          status = NOTIFY_SOK;
	struct list_head *temp   = NULL    ;
	struct list_head *temp1  = NULL    ;
	bool                found  = FALSE   ;

	dbc_require(list != NULL) ;
	dbc_require(elem != NULL) ;
	dbc_require(matchFunc != NULL) ;

	if ((list == NULL) || (elem == NULL)) {
		status = NOTIFY_EINVALIDARG ;
	} else {

		if (list_empty((struct list_head *)list))
			status = NOTIFY_EINVALIDARG ;

		if (NOTIFY_SUCCEEDED(status)) {
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
				status = NOTIFY_ENOTFOUND ;
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

module_init(drv_init_module) ;
module_exit(drv_finalize_module) ;
MODULE_LICENSE("GPL");

