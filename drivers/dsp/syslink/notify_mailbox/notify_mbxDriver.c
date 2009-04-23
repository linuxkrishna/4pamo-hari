/*
 * notify_mbxDriver.c
 *
 * DSP-BIOS Bridge driver support functions for TI OMAP processors.
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


#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/io.h>

#include <gpptypes.h>
#include <notifyerr.h>
#include <linux/module.h>

#include  <_bitops.h>
#include  <dbc.h>
#include <notify_driver.h>
#include <notifydefs.h>
#include <notify_driverdefs.h>
#include <notifydrv_install.h>
#include <notify_mbxDriver.h>


#include  <mem.h>
#include <global_var.h>
#include <gt.h>

#include  <notify_dispatcher.h>

MODULE_LICENSE("GPL");


#define NOTIFYSHMDRV_MEM_ALIGN       0

#define NOTIFYSHMDRV_MAX_EVENTS      32

#define NOTIFYSHMDRV_RECV_INT_ID     55

#define NOTIFYSHMDRV_SEND_INT_ID     26

#define NOTIFYSHMDRV_INIT_STAMP      0xA9C8B7D6

#define NOTIFYNONSHMDRV_MAX_EVENTS      1

#define NOTIFYNONSHMDRV_RESERVED_EVENTS      1

#define NOTIFYDRV_TESLA_RECV_MBX     1

#define NOTIFYDRV_TESLA_SEND_MBX     0

#define NOTIFYDRV_DUCATI_RECV_MBX     2

#define NOTIFYDRV_DUCATI_SEND_MBX     3

#define SELF_ID    0

#define OTHER_ID   1

#define UP    1

#define DOWN  0

#define PROC_TESLA  0
#define PROC_DUCATI 1
#define PROC_GPP    2

irqreturn_t(*irq_handler)(int, void *, struct pt_regs *);
EXPORT_SYMBOL(irq_handler);



/*
* Defines the Event Listener object, which contains
* information for *each registered listener for an event.
*/
struct notify_drv_eventlistner{
	struct list_head    element;
	fn_notify_cbck   fn_notify_cbck;
	void *cbck_arg;
};

/*
* Defines the Event object, which contains the event-specific
*information.
*/
struct notify_drv_eventlist {
	unsigned long int      event_handler_count;
	struct lst_list *listeners;
};

/*
* Defines the NotifyShmDrv state object..
*/
struct notifyshm_drv_state{
	unsigned long int                  proc_id;
	struct notify_drv_eventlist *event_list;
	struct notify_shmdrv_attrs            attrs;
	struct notify_shmdrv_ctrl *ctrlPtr;
	struct notify_shmdrv_eventreg *reg_chart;
	struct notify_driver_handle *drv_handle;
};

union notify_drv_procevents{
	struct {
		struct notify_shmdrv_attrs  attrs;
		struct notify_shmdrv_ctrl *ctrlPtr;
	} shm_events;

	struct {
	/*Attributes */
	unsigned long int    num_events ;
	unsigned long int    send_event_pollcount ;

	/*Control Paramters */
	unsigned long int send_init_status ;
	struct notify_shmdrv_eventreg_mask     reg_mask ;
	} non_shm_events;
};

struct notify_drv_proc_module {

	unsigned long int  proc_id;
	struct notify_drv_eventlist *event_list;
	struct notify_shmdrv_eventreg *reg_chart;
	union notify_drv_procevents  events_obj;
};

struct notify_mbxdrv_module {

	short int idriver_isinit;
	struct notify_driver_handle *drv_handle;
	struct notify_drv_proc_module  drv_proc_objects[2];
};

/*
* Notify Shared Memroy driver state object instance
*/
struct notify_mbxdrv_module notify_mbxdrv_state_obj;
EXPORT_SYMBOL(notify_mbxdrv_state_obj);

/*
* Funtion table interface for the Notify Shm Driver.
*/
struct notify_interface notify_mbxdrv_interface = {
	notify_mbxdrv_driver_init,
	notify_mbxdrv_driver_exit,
	notify_mbxdrv_register_event,
	notify_mbxdrv_unregevent,
	notify_mbxdrv_sendevent,
	notify_mbxdrv_disable,
	notify_mbxdrv_restore,
	notify_mbxdrv_disable_event,
	notify_mbxdrv_enable_event,
#if defined NOTIFY_DEBUG
	notify_mbxdrv_debug
#endif
};

/*
*This function searchs for a element the List.
*/
static void
notify_mbxdrv_qsearch_elem(IN  struct lst_list *list,
		struct notify_drv_eventlistner *checkObj,
		struct notify_drv_eventlistner **listener);

#if GT_TRACE
static struct GT_Mask notifymbx_debugmask = { NULL, NULL };
#endif

/*
* Top-level initialization function for the Notify shared memory
* mailbox driver.
*
*/
void notify_mbxdrv_init()
{
	int status = 0;
	struct notify_driver_attrs drv_attrs;

	GT_create(&notifymbx_debugmask, "NM");
	gt_0trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_init");

	/*Initialize the global Notify Mbx Driver Object */
	notify_mbxdrv_state_obj.idriver_isinit = false;
	notify_mbxdrv_state_obj.drv_handle = NULL;

	/*Fill in information about driver attributes. */
	drv_attrs.numProc = 2;

	drv_attrs.proc_info[PROC_TESLA].max_events =
			NOTIFYNONSHMDRV_MAX_EVENTS;
	drv_attrs.proc_info[PROC_TESLA].reserved_events =
				NOTIFYNONSHMDRV_MAX_EVENTS;
	drv_attrs.proc_info[PROC_TESLA].event_priority  = false;
	drv_attrs.proc_info[PROC_TESLA].payload_size = 32;
	drv_attrs.proc_info[PROC_TESLA].proc_id = PROC_TESLA;

	drv_attrs.proc_info[PROC_DUCATI].max_events  =
				NOTIFYSHMDRV_MAX_EVENTS;
	drv_attrs.proc_info[PROC_DUCATI].reserved_events =
			NOTIFYSHMDRV_RESERVED_EVENTS;
	drv_attrs.proc_info[PROC_DUCATI].event_priority = TRUE;
	drv_attrs.proc_info[PROC_DUCATI].payload_size = 32;
	drv_attrs.proc_info[PROC_DUCATI].proc_id = PROC_DUCATI;

	mailbx_init();
	status = notify_register_driver((char *)
				(NOTIFYMBXDRV_DRIVERNAME),
				&(notify_mbxdrv_interface),
				&drv_attrs,
				&(notify_mbxdrv_state_obj.drv_handle));

	if (status == 0)
		notify_mbxdrv_state_obj.idriver_isinit = TRUE;
	 else
		pr_err("notify_mbxdrv_init: failed, status = %d\n", status);
	gt_1trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_init status"
							"= %d\n", status);
}
EXPORT_SYMBOL(notify_mbxdrv_init);

/*
*
*Top-level finalization function for the Notify shared memory mailbox driver.
*
*/
void notify_mbxdrv_exit()
{
	int status = 0;

	gt_0trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_exit");

	WARN_ON(notify_mbxdrv_state_obj.idriver_isinit != TRUE);
	ntfy_disp_deinit();

	status = notify_unregister_driver(notify_mbxdrv_state_obj.drv_handle);

	if (status == 0)
		notify_mbxdrv_state_obj.idriver_isinit = false;
	else
		pr_err("notify_mbxdrv_exit: failed, status = %d\n", status);

	gt_1trace(notifymbx_debugmask, GT_5CLASS, "notify_mbxdrv_exit,"
						"status = %d\n", status);
}
EXPORT_SYMBOL(notify_mbxdrv_exit);

/*
* Initialization function for the Notify shared memory mailbox driver.
*
*/
signed long int notify_mbxdrv_driver_init(char *driver_name,
			struct notify_config *config, void **driver_object)
{
	int status = 0;
	int i = 0;
	struct notify_shmdrv_attrs *attrs = NULL;
	struct notify_drv_eventlist *event_list;
	struct NotifyShmDrv_ProcCtrl *ctrlPtr = NULL;
	 int proc_id;
	struct mbox_config *mbox_hw_config;
	int mbox_module_no;
	int interrupt_no;
	 int mbx_ret_val;

	gt_3trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_driver_init",
				driver_name, config, driver_object);

	BUG_ON(notify_mbxdrv_state_obj.idriver_isinit == false);
	BUG_ON(driver_object ==  NULL);
	BUG_ON(config ==  NULL);

	(void) driver_name;

	*driver_object = NULL;
	attrs = (struct notify_shmdrv_attrs *)config->driverAttrs;

	if (attrs == NULL) {
		status = -EINVAL;
		WARN_ON(1);
		gt_0trace(notifymbx_debugmask, GT_5CLASS,
			"notify_mbxdrv_driver_init: config"
			"attributes == NULL\n");
		goto func_exit;
	}

	/*Validate the attributes for Tesla */
	proc_id = PROC_TESLA;
	if (proc_id == PROC_TESLA) {
		gt_0trace(notifymbx_debugmask, GT_1CLASS,
			" --Tesla currently not using driverAttrs--");
	}

	/*Initialize the data structures for both processors. */
	proc_id = PROC_TESLA;

	if (proc_id == PROC_TESLA) {
		unsigned long int num_events =
			NOTIFYNONSHMDRV_MAX_EVENTS;

		gt_0trace(notifymbx_debugmask, GT_ENTER,
			"Tesla Driver Initialization");

		notify_mbxdrv_state_obj.
		drv_proc_objects[PROC_TESLA].proc_id = PROC_TESLA;

		notify_mbxdrv_state_obj.
		drv_proc_objects[PROC_TESLA].events_obj.
		non_shm_events.num_events = num_events;

		/*Allocate the listener list for each event. */
		notify_mbxdrv_state_obj.
		drv_proc_objects[PROC_TESLA].event_list =
		mem_calloc((sizeof(struct notify_drv_eventlist)
		* num_events), MEM_NONPAGED);

		if (notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].
				event_list == NULL) {
			status = -ENOMEM;
			WARN_ON(1);
			gt_0trace(notifymbx_debugmask, GT_5CLASS,
			"notify_mbxdrv_driver_init: TESLA event_list == NULL");
			goto func_exit;
		}

		if (notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].event_list) {

			notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].reg_chart =
			mem_calloc((sizeof
			(struct notify_shmdrv_eventreg) *
			num_events), MEM_NONPAGED);

			if (notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].
					reg_chart == NULL) {
				status = -ENOMEM;
				gt_0trace(notifymbx_debugmask, GT_5CLASS,
				"notify_mbxdrv_driver_init:"
				"TESLA event_list == NULL");
				WARN_ON(1);
				goto func_exit;
			}
		}
		gt_3trace(notifymbx_debugmask, GT_2CLASS, "status[0x%x]"
			"event_listr[0x%x] reg_chart[0x%x]\n", tmp_status,
			notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].event_list,
			notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].reg_chart);

		if (notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].reg_chart) {

			event_list = notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].event_list;

			for (i = 0; (i < num_events) && (status >= 0); i++) {
				notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].
				reg_chart[i].reg_event_no =
				(unsigned long int) -1;

				event_list[i].event_handler_count = 0;

				event_list[i].listeners =
				mem_calloc(sizeof(struct lst_list),
							MEM_NONPAGED);

				INIT_LIST_HEAD(&event_list[i].
						listeners->head);
				if (event_list[i].
					listeners == NULL) {
					status = -ENOMEM;
					gt_0trace(notifymbx_debugmask,
					GT_5CLASS, "notify_mbxdrv_driver_init:"
					"event list failure\n");
				}
			}
		}
	}
	gt_1trace(notifymbx_debugmask, GT_5CLASS,
			"Tesla Driver Initialization, status = %d\n", status);

	notify_mbxdrv_state_obj.
	drv_proc_objects[PROC_TESLA].proc_id = PROC_TESLA;

	notify_mbxdrv_state_obj.drv_proc_objects[PROC_TESLA].
	events_obj.shm_events.ctrlPtr =
	(struct notify_shmdrv_ctrl *)ioremap_nocache
	((dma_addr_t)attrs->shmBaseAddr, attrs->shmSize);

	ctrlPtr = (struct NotifyShmDrv_ProcCtrl *)
	(notify_mbxdrv_state_obj.drv_proc_objects[PROC_TESLA].
	 events_obj.shm_events.ctrlPtr);

	ctrlPtr->selfEventChart =
	(struct notify_shmdrv_event_entry *)
	((unsigned long int *) (notify_mbxdrv_state_obj.
	drv_proc_objects[PROC_TESLA].
	events_obj.shm_events.ctrlPtr) +
	sizeof(struct notify_shmdrv_ctrl) +
	(sizeof(struct notify_shmdrv_event_entry) *
		attrs->num_events * SELF_ID));

	ctrlPtr->otherEventChart =
	(struct notify_shmdrv_event_entry *)
	((unsigned long int *) (notify_mbxdrv_state_obj.
	drv_proc_objects[PROC_TESLA].events_obj.
	shm_events.ctrlPtr) +
	sizeof(struct notify_shmdrv_ctrl) +
	(sizeof(struct notify_shmdrv_event_entry) *
	attrs->num_events
	* OTHER_ID));

	gt_3trace(notifymbx_debugmask, GT_2CLASS,
	" ctrlPtr[0x%x] selfEventChart[0x%x] otherEventChart[0x%x]\n",
	ctrlPtr, ctrlPtr->selfEventChart, ctrlPtr->otherEventChart);

	gt_0trace(notifymbx_debugmask, GT_ENTER, "Interrupt Configuration");

	mbox_hw_config = mailbx_get_config();
	mbox_module_no = mbox_hw_config->mbox_modules;
	interrupt_no = mbox_hw_config->interrupt_lines[mbox_module_no-1];

	mbx_ret_val = ntfy_disp_bind_interrupt(interrupt_no,
			(void *)notify_mailbx0_user0_isr, NULL);
	/*Set up the ISR on the Modena-Tesla FIFO */
	if (mbx_ret_val == 0) {
		gt_0trace(notifymbx_debugmask, GT_ENTER,
				"Tesla Interrupt Enable\n");
		proc_id = PROC_TESLA;
		mbx_ret_val = mailbx_register(mbox_module_no,
				(NOTIFYDRV_TESLA_RECV_MBX * 2),
				(void *)notify_mbxdrv_nonshm_isr,
				(void *)
				&(notify_mbxdrv_state_obj.
				drv_proc_objects[proc_id]));

		if (mbx_ret_val == 0) {
			mbx_ret_val = mailbx_interrupt_enable(
				mbox_module_no, (NOTIFYDRV_TESLA_RECV_MBX * 2));
		}
	}

	/*Set up the ISR on the Modena-Ducati FIFO */
	if (mbx_ret_val != 0) {
		status = -ENODEV;
		gt_0trace(notifymbx_debugmask, GT_5CLASS,
				"MAIL box setup failure\n");
		WARN_ON(1);
		goto func_exit;
	}

	gt_1trace(notifymbx_debugmask, GT_5CLASS,
		"Interrupt Configuration", mbx_ret_val);

	if (status == 0) {
		*driver_object = &(notify_mbxdrv_state_obj);
		ctrlPtr->reg_mask.mask = 0x0;
		ctrlPtr->reg_mask.enable_mask = 0xFFFFFFFF;
		ctrlPtr->recvInitStatus = NOTIFYSHMDRV_INIT_STAMP;
		ctrlPtr->send_init_status = NOTIFYSHMDRV_INIT_STAMP;
	}
	gt_1trace(notifymbx_debugmask, GT_5CLASS, "notify_mbxdrv_driver_init"
						" status = %d\n", status);
func_exit:
return status;
}

/*
* Finalization function for the Notify driver.
*/
signed long int notify_mbxdrv_driver_exit(struct notify_driver_handle *handle)
{
	int status = 0;
	struct notify_drv_eventlistner *listener  = NULL;
	struct notify_mbxdrv_module *driver_object;
	struct notify_drv_eventlist *event_list;
	int i = 0;
	int proc_id;
	struct mbox_config *mbox_hw_config;
	int mbox_module_no;
	int interrupt_no;
	int mbx_ret_val = 0;

	gt_1trace(notifymbx_debugmask, GT_ENTER,
			"notify_mbxdrv_driver_exit", handle);

	WARN_ON(handle == NULL);
	if (handle == NULL)
		return -1;
	driver_object = (struct notify_mbxdrv_module *)&handle->driver_object;
	WARN_ON(handle->driver_object == NULL);
		return -1;
	WARN_ON(notify_mbxdrv_state_obj.idriver_isinit == false);

	/*Uninstall the ISRs & Disable the Mailbox interrupt.*/
	mbox_hw_config = mailbx_get_config();
	mbox_module_no = mbox_hw_config->mbox_modules;
	interrupt_no = mbox_hw_config->interrupt_lines[mbox_module_no-1];

	/*Remove the ISR on the Modena-Tesla FIFO */
	gt_0trace(notifymbx_debugmask, GT_ENTER, "Tesla Interrupt Removal");
	proc_id = PROC_TESLA;
	mailbx_interrupt_disable(mbox_module_no,
				(NOTIFYDRV_TESLA_RECV_MBX * 2));
	mailbx_unregister(mbox_module_no,
				(NOTIFYDRV_TESLA_RECV_MBX * 2));

	/*Remove the generic ISR */
	mbx_ret_val = mailbx_unbind_interrupt(interrupt_no);

	if (mbx_ret_val != 0)
		gt_1trace(notifymbx_debugmask, GT_6CLASS, "UnBind"
		"Interrupt failed[0x%x]", mbx_ret_val);

	gt_0trace(notifymbx_debugmask, GT_ENTER,
			"Tesla Driver Finalization");

	if (notify_mbxdrv_state_obj.
		drv_proc_objects[PROC_TESLA].reg_chart !=  NULL) {
		mem_free(notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].reg_chart);
		notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].reg_chart = NULL;
	}
	event_list = notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].event_list;

	for (i = 0; i < notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].
			events_obj.non_shm_events.num_events; i++) {

		WARN_ON(event_list[i].event_handler_count != 0);
		event_list[i].event_handler_count = 0;

		if (event_list[i].listeners !=  NULL) {
			while ((list_empty((struct list_head *)
				event_list[i].listeners) != true)) {

				listener = (struct notify_drv_eventlistner *)
					(event_list[i].listeners);
				if (listener != NULL) {
					mem_free(listener);
				} else {
					status = 0;
					gt_0trace(notifymbx_debugmask,
					GT_5CLASS, "listener== NULL\n");
					WARN_ON(1);
				}
			}
			list_del((struct list_head *)event_list[i].listeners);
			event_list[i].listeners = NULL;
		}
	}
	/*Free allocated memory. */
	if (notify_mbxdrv_state_obj.
		drv_proc_objects[PROC_TESLA].event_list !=  NULL) {
		mem_free(notify_mbxdrv_state_obj.
		drv_proc_objects[PROC_TESLA].event_list);

		notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].event_list = NULL;
	}
	gt_1trace(notifymbx_debugmask, GT_5CLASS,
		"notify_mbxdrv_driver_exit", status);
	return status;
}


/*
* Register a callback for an event with the Notify driver.
*
*/
signed long int notify_mbxdrv_register_event(
				struct notify_driver_handle *handle,
				unsigned long int  proc_id,
				unsigned long int  event_no,
				fn_notify_cbck     fn_notify_cbck,
				void *cbck_arg)
{
	int status = 0;
	int firstReg = false;
	struct notify_drv_eventlistner *event_listener;
	struct notify_drv_eventlist *event_list;
	struct notify_mbxdrv_module *driver_object;
	struct notify_shmdrv_eventreg *reg_chart;
	int i;
	int j;

	gt_5trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_register_event",
			handle, proc_id, event_no, fn_notify_cbck, cbck_arg);
	BUG_ON(notify_mbxdrv_state_obj.idriver_isinit != TRUE);
	BUG_ON(fn_notify_cbck == NULL);
	BUG_ON(handle == NULL);
	BUG_ON(handle->driver_object == NULL);
	driver_object = (struct notify_mbxdrv_module *)
				&handle->driver_object;
	if (proc_id == PROC_TESLA) {
		gt_0trace(notifymbx_debugmask, GT_ENTER, "Tesla RegisterEvent");

		event_listener =
		mem_calloc(sizeof(struct notify_drv_eventlistner),
						MEM_NONPAGED);

		if (event_listener == NULL) {
			status = -ENOMEM;
			goto func_end;
		}
		event_list = notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].event_list;

		event_listener->fn_notify_cbck = fn_notify_cbck;
		event_listener->cbck_arg   = cbck_arg;
		BUG_ON(event_no != 0);
		if (list_empty((struct list_head *)
			event_list[event_no].listeners)) {
			firstReg = TRUE;
			list_add_tail((struct list_head *)
					&(event_listener->element),
					(struct list_head *)
					event_list[event_no].listeners);

			event_list[event_no].event_handler_count++;

		} else {
			status = -EFAULT;
			gt_0trace(notifymbx_debugmask, GT_7CLASS,
				"GT_ENTERlist is empty\n");
		}
		if (firstReg == true) {
			reg_chart = notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].reg_chart;

			for (i = 0; i < notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].
				events_obj.non_shm_events.
					num_events; i++) {
				if (reg_chart[i].reg_event_no == (int) -1) {
					for (j = (i - 1); j >= 0; j--) {
						if (event_no
						< reg_chart[j].reg_event_no) {
							reg_chart
							[j + 1].
							reg_event_no =
							reg_chart[j].
							reg_event_no;

							reg_chart
							[j + 1].
							reserved  =
							reg_chart[j].
							reserved;
							i = j;
						} else {
							j = -1;
						}
					}
					reg_chart[i].reg_event_no =
							event_no;
					break;
				}
			}

			/*Mark the event as registered */
			SET_BIT(notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].
				events_obj.non_shm_events.
				reg_mask.mask, event_no);

		}
		gt_0trace(notifymbx_debugmask, GT_5CLASS,
					"Tesla RegisterEvent\n");
	}
func_end:
	gt_1trace(notifymbx_debugmask, GT_5CLASS,
			"notify_mbxdrv_register_event", status);
	return status;
}


/*
*
* Unregister a callback for an event with the Notify driver.
*
*/

signed long int notify_mbxdrv_unregevent(struct notify_driver_handle *handle,
					unsigned long int  proc_id,
					unsigned long int  event_no,
					fn_notify_cbck     fn_notify_cbck,
					void *cbck_arg)
{
	int status = 0;
	struct notify_drv_eventlistner *listener  = NULL;
	int num_events;
	struct notify_mbxdrv_module *driver_object;
	struct notify_drv_eventlist *event_list;
	struct notify_shmdrv_eventreg *reg_chart;
	struct notify_shmdrv_ctrl *ctrlPtr = NULL;
	struct notify_drv_eventlistner   unreg_info;
	int i;
	int j;

	gt_5trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_unregevent",
		handle, proc_id, event_no, fn_notify_cbck, cbck_arg);

	WARN_ON(notify_mbxdrv_state_obj.idriver_isinit == false);
	BUG_ON(fn_notify_cbck ==  NULL);
	BUG_ON(handle == NULL);
	BUG_ON(handle->driver_object == NULL);
	driver_object = (struct notify_mbxdrv_module *)&handle->driver_object;

	if (proc_id == PROC_TESLA) {
		gt_0trace(notifymbx_debugmask, GT_ENTER,
			"Tesla UnregisterEvent\n");
		num_events = notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].events_obj.
			non_shm_events.num_events;
		event_list = notify_mbxdrv_state_obj.
			drv_proc_objects[PROC_TESLA].event_list;
		unreg_info.fn_notify_cbck = fn_notify_cbck;
		unreg_info.cbck_arg = cbck_arg;
		notify_mbxdrv_qsearch_elem(event_list[event_no].listeners,
								&unreg_info,
								&listener);
		if (listener == NULL) {
			status = -EFAULT;
			goto func_end;
		}
		list_del((struct list_head *)&(listener->element));
		event_list[event_no].event_handler_count--;
		mem_free(listener);
		if (list_empty((struct list_head *)
			event_list[event_no].listeners) == TRUE) {
			CLEAR_BIT(ctrlPtr->selfProcCtrl.
					reg_mask.mask, event_no);
			reg_chart = notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].reg_chart;

			for (i = 0; i < num_events; i++) {
				if (event_no == reg_chart[i].reg_event_no) {
					reg_chart[i].reg_event_no =
						(unsigned long int) -1;
					for (j = (i + 1); (reg_chart[j].
						reg_event_no != (int) -1)
						&& (j != num_events); j++) {
						reg_chart
						[j - 1].reg_event_no
						= reg_chart[j].reg_event_no;
						reg_chart
						[j - 1].reserved =
							reg_chart[j].reserved;
					}
					if (j == num_events) {
						reg_chart
						[j - 1].reg_event_no = (int) -1;
					}
					break;
				}
			}
		}
		gt_0trace(notifymbx_debugmask, GT_5CLASS,
					"Tesla UnregisterEvent\n");
	}
func_end:
	gt_1trace(notifymbx_debugmask, GT_5CLASS,
		"notify_mbxdrv_unregevent", status);
	return status;
}

/*
*Send a notification event to the registered users for this
*notification on the specified processor.
*
*/
signed long int notify_mbxdrv_sendevent(struct notify_driver_handle *handle,
		 unsigned long int proc_id, unsigned long int event_no,
		 unsigned long int payload, short int wait_clear)
{
	int status = 0;
	struct notify_mbxdrv_module *driver_object;
	int maxPollCount;
	struct mbox_config *mbox_hw_config = mailbx_get_config();
	int mbox_module_no = mbox_hw_config->mbox_modules;
	int mbx_ret_val = 0;


	gt_5trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_sendevent",
					handle,
					proc_id,
					event_no,
					payload,
					wait_clear);
	BUG_ON(notify_mbxdrv_state_obj.idriver_isinit == false);
	BUG_ON(handle ==  NULL);
	BUG_ON(handle->driver_object == NULL);
	if (proc_id == PROC_TESLA) {
		gt_0trace(notifymbx_debugmask, GT_ENTER, "Tesla Send");
		driver_object =
		(struct notify_mbxdrv_module *)&handle->driver_object;
		BUG_ON(event_no > notify_mbxdrv_state_obj.
					drv_proc_objects[PROC_TESLA].events_obj.
					non_shm_events.num_events);
		maxPollCount = notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].
				events_obj.non_shm_events.send_event_pollcount;
		mbx_ret_val = mailbx_send(mbox_module_no,
				NOTIFYDRV_TESLA_SEND_MBX, payload);
		gt_1trace(notifymbx_debugmask, GT_5CLASS,
				"Tesla Send", mbx_ret_val);
	}
	gt_1trace(notifymbx_debugmask, GT_5CLASS,
			"notify_mbxdrv_sendevent", status);
	return status;
}

/*
* Disable all events for this Notify driver.
*
*/
void *notify_mbxdrv_disable(struct notify_driver_handle *handle)
{
	int mbx_ret_val = KErrNone;
	struct mbox_config *mbox_hw_config = mailbx_get_config();
	int mbox_module_no = mbox_hw_config->mbox_modules;

	BUG_ON(notify_mbxdrv_state_obj.idriver_isinit == false);
	gt_1trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_disable\n",
								handle);
	mbx_ret_val = mailbx_interrupt_disable(mbox_module_no,
				(NOTIFYDRV_TESLA_RECV_MBX * 2));
	gt_1trace(notifymbx_debugmask, GT_ENTER,
			"Tesla Disable[%d]", mbx_ret_val);
	return NULL; /*No flags to be returned. */
}

/*
* Restore the Notify driver to the state before the last disable was
*called.
*
*/
signed long int notify_mbxdrv_restore(struct notify_driver_handle *handle,
							void *flags)
{
	int mbx_ret_val = 0;
	struct mbox_config *mbox_hw_config = mailbx_get_config();
	int mbox_module_no = mbox_hw_config->mbox_modules;

	BUG_ON(notify_mbxdrv_state_obj.idriver_isinit == false);
	gt_2trace(notifymbx_debugmask, GT_ENTER,
		"notify_mbxdrv_restore", handle, flags);
	(void) handle;
	(void) flags;
	/*Enable the receive interrupt for Tesla */
	mbx_ret_val = mailbx_interrupt_enable(mbox_module_no,
			(NOTIFYDRV_TESLA_RECV_MBX * 2));
	gt_1trace(notifymbx_debugmask, GT_1CLASS,
		"Tesla Enable[%d]", mbx_ret_val);
	return mbx_ret_val;
}

/*
* Disable a specific event for this Notify driver.
*
*/
signed long int notify_mbxdrv_disable_event(struct notify_driver_handle *handle,
			unsigned long int  proc_id, unsigned long int  event_no)
{
	static int access_count ;
	signed long int status = 0;
	struct notify_mbxdrv_module *driver_object;

	access_count++;
	gt_3trace(notifymbx_debugmask, GT_ENTER,
	"notify_mbxdrv_disable_event", handle, proc_id, event_no);

	BUG_ON(notify_mbxdrv_state_obj.idriver_isinit == false);
	BUG_ON(handle == NULL);
	BUG_ON(handle->driver_object == NULL);
	BUG_ON((proc_id != PROC_TESLA) && (proc_id != PROC_DUCATI));

	if (proc_id == PROC_TESLA) {
		gt_0trace(notifymbx_debugmask, GT_ENTER, "Tesla DisableEvent");

		driver_object = (struct notify_mbxdrv_module *)
					handle->driver_object;
		BUG_ON(event_no > notify_mbxdrv_state_obj.
					drv_proc_objects[PROC_TESLA].
					events_obj.non_shm_events.num_events);
		if (access_count == 1) {
			CLEAR_BIT(notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].
				events_obj.non_shm_events.
				reg_mask.enable_mask, event_no);
		}
		gt_1trace(notifymbx_debugmask, GT_5CLASS,
				"Tesla DisableEvent", notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].events_obj.
				non_shm_events.reg_mask.enable_mask);
	}
	gt_1trace(notifymbx_debugmask, GT_5CLASS,
			"notify_mbxdrv_disable_event", status);
	return status;
}

/*
* Enable a specific event for this Notify driver.
*
*/
signed long int notify_mbxdrv_enable_event(struct notify_driver_handle *handle,
		unsigned long int     proc_id, unsigned long int     event_no)
{
	int status = 0;
	struct notify_mbxdrv_module *driver_object;

	gt_3trace(notifymbx_debugmask, GT_ENTER,
			"notify_mbxdrv_enable_event", handle,
					proc_id, event_no);

	BUG_ON(notify_mbxdrv_state_obj.idriver_isinit == false);
	BUG_ON(handle == NULL);
	BUG_ON(handle->driver_object == NULL);
	BUG_ON((proc_id != PROC_TESLA) && (proc_id != PROC_DUCATI));

	driver_object = (struct notify_mbxdrv_module *)&handle->driver_object;
	if (proc_id == PROC_TESLA) {
		gt_0trace(notifymbx_debugmask, GT_ENTER, "Tesla EnableEvent");

		BUG_ON(event_no > notify_mbxdrv_state_obj.
				drv_proc_objects[PROC_TESLA].events_obj.
				non_shm_events.num_events);
		SET_BIT(notify_mbxdrv_state_obj.drv_proc_objects[PROC_TESLA].
		events_obj.non_shm_events.reg_mask.enable_mask, event_no);
		gt_1trace(notifymbx_debugmask, GT_5CLASS,
			"Tesla EnableEvent", notify_mbxdrv_state_obj.
		drv_proc_objects[PROC_TESLA].events_obj.non_shm_events.
						reg_mask.enable_mask);
	}
	gt_1trace(notifymbx_debugmask, GT_5CLASS,
			"notify_mbxdrv_enable_event", status);
	return status;
}

/*
* Print debug information for the Notify driver.
*
*/
signed long int notify_mbxdrv_debug(IN struct notify_driver_handle *handle)
{
	int status = 0;
	gt_1trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_debug", handle);
	gt_0trace(notifymbx_debugmask, GT_1CLASS,
	"Tesla Debug: Nothing being printed currently.");
	gt_1trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_debug", status);
	return status;
}

/*
* This function implements the interrupt service routine for the
* interrupt received from the DSP.
*
*/
void notify_mbxdrv_nonshm_isr(void *ref_data)
{
	unsigned long int payload = 0;
	unsigned long int i = 0;
	struct list_head *temp;
	unsigned long int j;
	struct notify_shmdrv_eventreg *reg_chart;
	unsigned long int event_no;
	struct notify_drv_proc_module *drv_proc_object;
	struct mbox_config *mbox_hw_config = mailbx_get_config();
	unsigned long int mbox_module_no = mbox_hw_config->mbox_modules;
	signed long int mbx_ret_val = 0;
	unsigned long int num_messages = 0;
	unsigned long int num_events = 0;

	/* Commented out, since this function will be called in an ISR */
	gt_1trace(notifymbx_debugmask, GT_ENTER,
			"notify_mbxdrv_nonshm_isr", ref_data);

	drv_proc_object = (struct notify_drv_proc_module *) ref_data;
	BUG_ON(drv_proc_object == NULL);
	reg_chart = drv_proc_object->reg_chart;
	num_events = drv_proc_object->events_obj.non_shm_events.num_events;

	do {
		event_no = reg_chart[i].reg_event_no;
		if (event_no != (unsigned long int) -1) {
			if ((TEST_BIT(drv_proc_object->events_obj.
			non_shm_events.reg_mask.
				enable_mask, event_no)) == true) {
				mbx_ret_val = mailbx_read(mbox_module_no,
					NOTIFYDRV_TESLA_RECV_MBX, &payload,
					&num_messages, false);
				BUG_ON(mbx_ret_val != 0);
				temp  = drv_proc_object->event_list
					[event_no].listeners->head.next;
				for (j = 0; j <
				drv_proc_object->event_list[event_no].
					event_handler_count; j++) {
					if (temp > (struct list_head *)0) {
						((struct
						notify_drv_eventlistner
						 *)
						 temp)->fn_notify_cbck(
						drv_proc_object->
						proc_id,
						 event_no,
						((struct
						notify_drv_eventlistner
						 *)
						 temp)->cbck_arg,
						payload);
						temp = temp->next;
					}
				}
			}
			i++;
		}
	} while ((event_no != (unsigned long int) -1) && (i < num_events));
}
EXPORT_SYMBOL(notify_mbxdrv_nonshm_isr);

/*
* This function searchs for a element the List.
*
*/
static void notify_mbxdrv_qsearch_elem(IN  struct lst_list *list,
			IN  struct notify_drv_eventlistner *checkObj,
			OUT struct notify_drv_eventlistner * *listener)
{
	struct list_head    *temp      = NULL ;
	struct notify_drv_eventlistner *lTemp     = NULL ;
	short int found     = false;

	gt_3trace(notifymbx_debugmask, GT_ENTER, "notify_mbxdrv_qsearch_elem",
				list, checkObj, listener);

	BUG_ON(list ==  NULL);
	BUG_ON(checkObj == NULL);
	WARN_ON(listener == NULL);

	if (listener != NULL)
		return;
	*listener = NULL;
	if ((list !=  NULL) && (checkObj != NULL)) {
		if (list_empty((struct list_head *)list) == FALSE) {
			temp = list->head.next;
			while ((found == FALSE) && (temp != NULL)) {
				lTemp =
				(struct notify_drv_eventlistner *)(temp);
				if ((lTemp->fn_notify_cbck ==
				checkObj->fn_notify_cbck) &&
				(lTemp->cbck_arg == checkObj->cbck_arg)) {
					found = TRUE;
				} else
					temp = temp->next;
			}
			if (found == TRUE)
				*listener = lTemp;
		}
	}
	gt_1trace(notifymbx_debugmask, GT_1CLASS, "  listener[0x%x]\n",
							*listener);
	gt_0trace(notifymbx_debugmask, GT_ENTER,
					"notify_mbxdrv_qsearch_elem\n");
	return;
}
