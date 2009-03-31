/*===========================================================================
File    notify_mbxDriver.cpp

Path $ (PROJROOT)\driver\NotifyDrivers

Desc Implements the Notify Mailboxes Driver for both
Tesla and Ducati remote processors

Rev     0.1.0

This computer program is copyright to Texas Instruments Incorporated.
The program may not be used without the written permission of
Texas Instruments Incorporated or against the terms and conditions
stipulated in the agreement under which this program has been supplied.

(c) Texas Instruments Incorporated 2008

===========================================================================
*/

/*----------------------------------- OS Specific Headers         */
/*#include  < e32def.h>
#include  < e32err.h>
#include  < kernel.h>
#include  < platform.h>*/

/*Kernel Hdeaders*/
#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/io.h>



/*----------------------------------- IPC Headers                 */
#include  <gpptypes.h>
#include  <ipctypes.h>
#include  <notifyerr.h>
#include  <linux/module.h>

/*----------------------------------- OSAL Headers                */




/*----------------------------------- Generic Headers             */
#include  <_bitops.h>
#include  <dbc.h>
#include  <list.h>

/*----------------------------------- Notify Headers              */
#include <notifydefs.h>
#include <notify_driverdefs.h>

#include <notifydrv_install.h>
#include <notify_mbxDriver.h>
#include <_notify_mbxDriver.h>

#include <shmdefs.h>
#include <notify_driver.h>



#include  <mem.h>
#include <trc.h>
#include <global_var.h>

/*-------------------------------- Mailmox Manager Headers                 */
#include  <notify_dispatcher.h>
MODULE_LICENSE("GPL");

/**============================================================================
*@macro  COMPONENT_ID
*
*@desc   Component and Subcomponent Identifier.
*============================================================================
*/
#define  COMPONENT_ID       ID_KNL_NOTIFY_MBXDRIVER


/**============================================================================
*@macro  SET_FAILURE_REASON
*
*@desc   Sets failure reason.
*============================================================================
*/
#if defined NOTIFY_DEBUG
#define SET_FAILURE_REASON(status)  (TRC_SetReason \
(status, FID_C_KNL_NOTIFY_SHMDRIVER, __LINE__))
#else
#define SET_FAILURE_REASON(status)
#endif /*if defined (NOTIFY_DEBUG) */


/**============================================================================
*@const  NOTIFYSHMDRV_MEM_ALIGN
*
*@desc   Default alignment to be used for local memory allocations.
*============================================================================
*/
#define NOTIFYSHMDRV_MEM_ALIGN       0

/**============================================================================
*@const  NOTIFYSHMDRV_MAX_EVENTS
*
*@desc   Maximum number of events supported by the NotiyShmDrv driver.
*============================================================================
*/
#define NOTIFYSHMDRV_MAX_EVENTS      32

/**============================================================================
*@const  NOTIFYSHMDRV_RECV_INT_ID
*
*@desc   Interrupt ID of physical interrupt handled by the Notify driver to
*receive events.
*============================================================================
*/
#define NOTIFYSHMDRV_RECV_INT_ID     55

/**============================================================================
*@const  NOTIFYSHMDRV_SEND_INT_ID
*
*@desc   Interrupt ID of physical interrupt handled by the Notify driver to
*send events.
*============================================================================
*/
#define NOTIFYSHMDRV_SEND_INT_ID     26

/**============================================================================
*@const  NOTIFYSHMDRV_INIT_STAMP
*
*@desc   Stamp indicating that the Notify Shared Memory driver on the
*processor has been initialized.
*============================================================================
*/
#define NOTIFYSHMDRV_INIT_STAMP      0xA9C8B7D6

/**============================================================================
*@const  NOTIFYSHMDRV_MAX_EVENTS
*
*@desc   Maximum number of events supported by the NotiyShmDrv driver.
*============================================================================
*/
#define NOTIFYNONSHMDRV_MAX_EVENTS      1

/**============================================================================
*@const  NOTIFYSHMDRV_MAX_EVENTS
*
*@desc   Maximum number of events supported by the NotiyShmDrv driver.
*============================================================================
*/
#define NOTIFYNONSHMDRV_RESERVED_EVENTS      1


#define NOTIFYDRV_TESLA_RECV_MBX     1

#define NOTIFYDRV_TESLA_SEND_MBX     0

#define NOTIFYDRV_DUCATI_RECV_MBX     2

#define NOTIFYDRV_DUCATI_SEND_MBX     3



/**============================================================================
*@const  SELF_ID
*
*@desc   Identifier for this processor communicating with the other
*processor
*============================================================================
*/
#define SELF_ID    0

/**============================================================================
*@const  OTHER_ID
*
*@desc   Identifier for the other processor communicating with this
*processor
*============================================================================
*/
#define OTHER_ID   1

/**============================================================================
*@const  UP
*
*@desc   Flag indicating event is set.
*============================================================================
*/
#define UP    1

/**============================================================================
*@const  DOWN
*
*@desc   Flag indicating event is not set.
*============================================================================
*/
#define DOWN  0


#define PROC_TESLA  0
#define PROC_DUCATI 1
#define PROC_GPP    2

#define PAGE_ALIGN_BITS  12

#if defined(DEBUGLDD)
#define ON_DEBUGLDD(x) x
#else
#define ON_DEBUGLDD(x)
#endif

irqreturn_t(*irq_handler)(int, void *, struct pt_regs *);
EXPORT_SYMBOL(irq_handler);



/**============================================================================
*@const  SPINLOCK_START
*
*@desc   Start spinlock protection
*============================================================================
*/
/*#define SPINLOCK_START()           SYNC_SpinLockStart ()*/

/**============================================================================
*@const  SPINLOCK_END
*
*@desc   End spinlock protection
*============================================================================
*/
/*#define SPINLOCK_END(irqFlags)     SYNC_SpinLockEnd(irqFlags)*/


/**============================================================================
*@name   struct NotifyDrv_EventListener
*
*@desc   Defines the Event Listener object, which contains information for
*each registered listener for an event.
*
*@field  element
*Structure that allows it to be used by LIST.
*@field  fnNotifyCbck
*Callback function for the event.
*@field  cbckArg
*Parameters passed to the callback.
*============================================================================
*/
struct NotifyDrv_EventListener{
struct list_head    element;
FnNotifyCbck   fnNotifyCbck;
void *cbckArg;
};

/**============================================================================
*@name   struct NotifyDrv_EventList
*
*@desc   Defines the Event object, which contains the event-specific
*information.
*
*@field  eventHandlerCount
*Number of listener attached to the event.
*@field  listeners
*Queue of listeners.
*============================================================================
*/
struct NotifyDrv_EventList {
unsigned long int      eventHandlerCount;
struct lst_list *listeners;
};

/**============================================================================
*@name   struct NotifyShmDrv_State
*
*@desc   Defines the NotifyShmDrv state object.
*
*@field  procId
*ID of processor with which this Notify driver communicates.
*@field  eventList
*Pointer to array containing information about registered
*listeners and events.
*@field  attrs
*Contains user-provided information for the NotifyShmDrv driver.
*@field  ctrlPtr
*Pointer to the NotifyShmDrv control structure.
*@field  regChart
*Registration chart indicating event number of registered events.
*@field  drvHandle
*Handle to the Notify driver object.
*@field  isrObject
*NotifyShmDrv Isr object.
*============================================================================
*/
struct NotifyShmDrv_State{
unsigned long int                  procId;
struct NotifyDrv_EventList *eventList;
struct NotifyShmDrv_Attrs            attrs;
struct NotifyShmDrv_Ctrl *ctrlPtr;
struct NotifyShmDrv_EventRegEntry *regChart;
struct Notify_DriverHandle *drvHandle;
};

union NotifyDrv_ProcEvents{
struct {
struct NotifyShmDrv_Attrs            attrs;
struct NotifyShmDrv_Ctrl *ctrlPtr;
}  shmEvents;

struct {
/*Attributes */
unsigned long int    numEvents ;
unsigned long int    sendEventPollCount ;

/*Control Paramters */
unsigned long int sendInitStatus ;
struct NotifyShmDrv_EventRegMask     regMask ;
}  nonShmEvents;
};


struct NotifyDrvProcModule {

unsigned long int                  procId;
struct NotifyDrv_EventList *eventList;
struct NotifyShmDrv_EventRegEntry *regChart;
union NotifyDrv_ProcEvents     eventsObj;
};


struct NotifyMbxDrvModule {

short int       iDriver_IsInit;
struct Notify_DriverHandle *drvHandle;
struct NotifyDrvProcModule        drvProcObjects[2];
};
/**============================================================================
*@name   struct NotifyShmDrv_StateObj
*
*@desc   Notify Shared Memroy driver state object instance
*============================================================================
*/
struct NotifyMbxDrvModule NotifyMbxDrv_StateObj;
EXPORT_SYMBOL(NotifyMbxDrv_StateObj);



/*----------------------------------------------------------------------------
*@name   NotifyShmDrv_IsInit
*
*@desc   Indicates whether the NotifyShmDrv driver is initialized.
*----------------------------------------------------------------------------
*/

/**============================================================================
*@name   NotifyMbxDrv_Interface
*
*@desc   Funtion table interface for the Notify Shm Driver.
*============================================================================
*/
struct Notify_Interface NotifyMbxDrv_Interface = {
NotifyMbxDrv_driverInit,
NotifyMbxDrv_driverExit,
NotifyMbxDrv_registerEvent,
NotifyMbxDrv_unregisterEvent,
NotifyMbxDrv_sendEvent,
NotifyMbxDrv_disable,
NotifyMbxDrv_restore,
NotifyMbxDrv_disableEvent,
NotifyMbxDrv_enableEvent,
#if defined NOTIFY_DEBUG
NotifyMbxDrv_debug
#endif
} ;



/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_queSearchElement
*
*@desc   This function searchs for a element the List.
*
*@arg    list.
*Pointer to List.
*@arg    checkObj
*Listener object to be matched with queue element.
*@arg    listener
*Pointer to store found element.
*
*@ret    None.
*
*@enter  None.
*
*@leave  None.
*
*@see    None.
*----------------------------------------------------------------------------
*/
static void
NotifyMbxDrv_queSearchElement(IN  struct lst_list *list,
IN  struct NotifyDrv_EventListener *checkObj,
OUT struct NotifyDrv_EventListener **listener);


signed long int DECLARE_STANDARD_EXTENSION(void)
{
	signed long int status = KErrNone;


	TRC_ENABLE(ID_NOTIFYMBXDRV_ALL);
	TRC_SET_SEVERITY(TRC_ENTER);

	NotifyMbxDrv_init();

	return status;
}

/**============================================================================
@name   NotifyMbxDrv_init
*
*@desc   Top-level initialization function for the Notify shared memory
*mailbox driver.
*This can be plugged in as the user init function.
*
*@modif  NotifyMbxDrv_StateObj.iDriver_IsInit
*============================================================================
*/

void
NotifyMbxDrv_init()
{
	signed long int status = NOTIFY_SOK;
	struct Notify_DriverAttrs drvAttrs;
	signed long int mbxRetVal = KErrNone;

	TRC_0ENTER("NotifyMbxDrv_init");

	/*Initialize the global Notify Mbx Driver Object */
	NotifyMbxDrv_StateObj.iDriver_IsInit = FALSE;
	NotifyMbxDrv_StateObj.drvHandle = NULL;

	/*Fill in information about driver attributes. */
	drvAttrs.numProc = 2;

	drvAttrs.procInfo[PROC_TESLA].maxEvents =
			NOTIFYNONSHMDRV_MAX_EVENTS;

	drvAttrs.procInfo[PROC_TESLA].reservedEvents =
				NOTIFYNONSHMDRV_MAX_EVENTS;

	drvAttrs.procInfo[PROC_TESLA].eventPriority  =
			FALSE; /*Events are prioritized. */

	drvAttrs.procInfo[PROC_TESLA].payloadSize = 32;
			/*32-bit payload supported. */

	drvAttrs.procInfo[PROC_TESLA].procId = PROC_TESLA;

	drvAttrs.procInfo[PROC_DUCATI].maxEvents  =
				NOTIFYSHMDRV_MAX_EVENTS;

	drvAttrs.procInfo[PROC_DUCATI].reservedEvents =
			NOTIFYSHMDRV_RESERVED_EVENTS;

	drvAttrs.procInfo[PROC_DUCATI].eventPriority = TRUE;
				/*Events are prioritized. */

	drvAttrs.procInfo[PROC_DUCATI].payloadSize = 32;
			/*32-bit payload supported. */

	drvAttrs.procInfo[PROC_DUCATI].procId = PROC_DUCATI;


	mbxRetVal = Mailbx_Init();

	if (mbxRetVal !=  KErrNone)
		status = NOTIFY_EFAIL;


	if (NOTIFY_SUCCEEDED(status)) {
		status =
			Notify_registerDriver((char *)
				(NOTIFYMBXDRV_DRIVERNAME),
				&(NotifyMbxDrv_Interface),
				&drvAttrs,
				&(NotifyMbxDrv_StateObj.drvHandle));


	}

	if (NOTIFY_SUCCEEDED(status))
		NotifyMbxDrv_StateObj.iDriver_IsInit = TRUE;
	 else
		SET_FAILURE_REASON(status);


	TRC_1LEAVE("NotifyMbxDrv_init", status);
}
EXPORT_SYMBOL(NotifyMbxDrv_init);




/**============================================================================
*@name   NotifyMbxDrv_exit
*
*@desc   Top-level finalization function for the Notify shared memory
*mailbox driver.
*
*@modif  NotifyMbxDrv_IsInit
*============================================================================
*/

void
NotifyMbxDrv_exit()
{
	signed long int status = NOTIFY_SOK;
	signed long int mbxRetVal = KErrNone;

	TRC_0ENTER("NotifyMbxDrv_exit");

	DBC_require(NotifyMbxDrv_StateObj.iDriver_IsInit == TRUE);


	mbxRetVal = Mailbx_DeInit();

	if (mbxRetVal !=  KErrNone)
		status = NOTIFY_EFAIL;


	if (NOTIFY_SUCCEEDED(status))
		status = Notify_unregisterDriver(NotifyMbxDrv_StateObj.
							drvHandle);

	if (NOTIFY_SUCCEEDED(status))
		NotifyMbxDrv_StateObj.iDriver_IsInit = FALSE;
	else
		SET_FAILURE_REASON(status);


	TRC_1LEAVE("NotifyMbxDrv_exit", status);
}
EXPORT_SYMBOL(NotifyMbxDrv_exit);

/**----------------------------------------------------------------------------
*@name   NotifyMbxDrv_driverInit
*
*@desc   Initialization function for the Notify shared memory mailbox driver.
*
*@modif  NotifyMbxDrv_StateObj
*----------------------------------------------------------------------------
*/

signed long int
NotifyMbxDrv_driverInit(IN  char *driverName,
IN  struct Notify_Config *config,
OUT void **driverObj)
{
	signed long int              status    = NOTIFY_SOK;
	unsigned long int                     i         = 0;
	struct NotifyShmDrv_Attrs *attrs     = NULL;
	struct NotifyDrv_EventList *eventList;
	struct NotifyShmDrv_ProcCtrl *ctrlPtr = NULL;
	unsigned long int               procId;
	struct MboxConfig *mBoxHwConfig;
	unsigned long int mBoxModuleNo;
	signed long int interruptNo;
	signed long int mbxRetVal;






	TRC_3ENTER("NotifyMbxDrv_driverInit",
				driverName, config, driverObj);

	DBC_require(NotifyMbxDrv_StateObj.iDriver_IsInit == TRUE);
	DBC_require(driverObj !=  NULL);
	DBC_require(config !=  NULL);

	(void) driverName;

	if (NotifyMbxDrv_StateObj.iDriver_IsInit == FALSE) {
		status = NOTIFY_EDRIVERINIT;
		SET_FAILURE_REASON(status);

	} else if ((driverObj == NULL) || (config == NULL)) {
		status = NOTIFY_EPOINTER;
		SET_FAILURE_REASON(status);

	} else {
		*driverObj = NULL;
		attrs = (struct NotifyShmDrv_Attrs *)
						config->driverAttrs;

		if (attrs == NULL) {
			status = NOTIFY_ECONFIG;
			SET_FAILURE_REASON(status);
		}


		if (NOTIFY_SUCCEEDED(status)) {

			/*Validate the attributes for Tesla */
			procId = PROC_TESLA;

			if (procId == PROC_TESLA) {
				TRC_0PRINT(TRC_LEVEL1,
				" --Tesla currently not using driverAttrs--");
			}

		}
	}


	/*Initialize the data structures for both processors. */
	if (NOTIFY_SUCCEEDED(status)) {

		procId = PROC_TESLA;

		if (procId == PROC_TESLA) {
			unsigned long int numEvents =
				NOTIFYNONSHMDRV_MAX_EVENTS;

			TRC_0ENTER("Tesla Driver Initialization");

			NotifyMbxDrv_StateObj.
			drvProcObjects[PROC_TESLA].procId = PROC_TESLA;

			NotifyMbxDrv_StateObj.
			drvProcObjects[PROC_TESLA].eventsObj.
			nonShmEvents.numEvents = numEvents;

			/*Allocate the listener list for each event. */


			if (NOTIFY_SUCCEEDED(status)) {

				NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].eventList =
				MEM_Calloc((sizeof(struct NotifyDrv_EventList)
				* numEvents), MEM_NONPAGED);


				if (NotifyMbxDrv_StateObj.
					drvProcObjects[PROC_TESLA].
						eventList == NULL) {

					NotifyMbxDrv_StateObj.
						drvProcObjects[PROC_TESLA].
							eventList = NULL;

					status = NOTIFY_EMEMORY;
					SET_FAILURE_REASON(status);
				}
			}


			if (NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].eventList) {

				NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].regChart =
				MEM_Calloc((sizeof
				(struct NotifyShmDrv_EventRegEntry) *
				numEvents), MEM_NONPAGED);


				if (NotifyMbxDrv_StateObj.
					drvProcObjects[PROC_TESLA].
						regChart == NULL) {

					NotifyMbxDrv_StateObj.
					drvProcObjects[PROC_TESLA].
					regChart = NULL;

					status = NOTIFY_EMEMORY;
					SET_FAILURE_REASON(status);
				}
			}

			TRC_3PRINT(TRC_LEVEL2, "status[0x%x]  \
				eventListr[0x%x] regChart[0x%x]",
							tmpStatus,
			NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].eventList,
			NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].regChart);



			if (NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].regChart) {

				eventList = NotifyMbxDrv_StateObj.
					drvProcObjects[PROC_TESLA].eventList;

				for (i = 0; (i < numEvents) &&
					NOTIFY_SUCCEEDED(status); i++) {

					NotifyMbxDrv_StateObj.
					drvProcObjects[PROC_TESLA].
					regChart[i].regEventNo =
					(unsigned long int) -1;

					eventList[i].eventHandlerCount = 0;

					eventList[i].listeners =
					MEM_Calloc(sizeof(struct lst_list),
								MEM_NONPAGED);

					INIT_LIST_HEAD(&eventList[i].
							listeners->head);


						if (eventList[i].
							listeners == NULL) {

							status = NOTIFY_EFAIL;
							SET_FAILURE_REASON(
								status);
						}
				}
			}

			TRC_1LEAVE("Tesla Driver Initialization", status);
		}


/**************************************************************/


		if (NOTIFY_SUCCEEDED(status)) {
			NotifyMbxDrv_StateObj.
			drvProcObjects[PROC_TESLA].procId = PROC_TESLA;


			NotifyMbxDrv_StateObj.drvProcObjects[PROC_TESLA].
			eventsObj.shmEvents.ctrlPtr =
			(struct NotifyShmDrv_Ctrl *)ioremap_nocache
			((dma_addr_t)attrs->shmBaseAddr, attrs->shmSize) ;



			ctrlPtr  =
			(struct NotifyShmDrv_ProcCtrl *)
			(NotifyMbxDrv_StateObj.drvProcObjects[PROC_TESLA].
			 eventsObj.shmEvents.ctrlPtr);



			ctrlPtr->selfEventChart =
			(struct NotifyShmDrv_EventEntry *)
			((unsigned long int *) (NotifyMbxDrv_StateObj.
			drvProcObjects[PROC_TESLA].
			eventsObj.shmEvents.ctrlPtr) +
			sizeof(struct NotifyShmDrv_Ctrl) +
			(sizeof(struct NotifyShmDrv_EventEntry) *
				attrs->numEvents * SELF_ID));


			ctrlPtr->otherEventChart =
			(struct NotifyShmDrv_EventEntry *)
			((unsigned long int *) (NotifyMbxDrv_StateObj.
			drvProcObjects[PROC_TESLA].eventsObj.
			shmEvents.ctrlPtr) +
			sizeof(struct NotifyShmDrv_Ctrl) +
			(sizeof(struct NotifyShmDrv_EventEntry) *
			attrs->numEvents
			* OTHER_ID));


				TRC_3PRINT(TRC_LEVEL2,
				" ctrlPtr[0x%x] selfEventChart[0x%x] \
				otherEventChart[0x%x]", ctrlPtr,
				ctrlPtr->selfEventChart,
				ctrlPtr->otherEventChart);

		}



/*****************************************************************/



	}



	if (NOTIFY_SUCCEEDED(status)) {
		TRC_0ENTER("Interrupt Configuration");

	mBoxHwConfig = Mailbx_GetMBoxConfig();
	mBoxModuleNo = mBoxHwConfig->mBoxModules;
	interruptNo = mBoxHwConfig->interruptLines[mBoxModuleNo-1];
	mbxRetVal = KErrNone;

	mbxRetVal = Mailbx_BindInterrupt(interruptNo,
			(void *)Notify_Mailbox0User0_ISR, NULL);
	/*Set up the ISR on the Modena-Tesla FIFO */

		if (mbxRetVal == KErrNone) {

			TRC_0ENTER("Tesla Interrupt Enable");

			procId = PROC_TESLA;

			mbxRetVal = Mailbx_Register(mBoxModuleNo,
					(NOTIFYDRV_TESLA_RECV_MBX * 2),
					(void *)NotifyMbxDrv_NonShmISR,
					(void *)
					&(NotifyMbxDrv_StateObj.
					drvProcObjects[procId]));

			if (mbxRetVal == KErrNone) {

					mbxRetVal = Mailbx_InterruptEnable(
								mBoxModuleNo,
						(NOTIFYDRV_TESLA_RECV_MBX * 2));
			}

			TRC_1LEAVE("Tesla Interrupt Enable", mbxRetVal);
		}


/*Set up the ISR on the Modena-Ducati FIFO */


		if (mbxRetVal !=  KErrNone) {
			status = NOTIFY_EFAIL;
			SET_FAILURE_REASON(status);
		}

		TRC_1LEAVE("Interrupt Configuration", mbxRetVal);
	}


	if (NOTIFY_SUCCEEDED(status)) {

		*driverObj = &(NotifyMbxDrv_StateObj);

		ctrlPtr->regMask.mask = 0x0;
		ctrlPtr->regMask.enableMask = 0xFFFFFFFF;


		ctrlPtr->recvInitStatus = NOTIFYSHMDRV_INIT_STAMP;
		ctrlPtr->sendInitStatus = NOTIFYSHMDRV_INIT_STAMP;


	}

TRC_1LEAVE("NotifyMbxDrv_driverInit", status);

return status;
}

/**----------------------------------------------------------------------------
*@name   NotifyMbxDrv_driverExit
*
*@desc   Finalization function for the Notify driver.
*
*@modif  NotifyMbxDrv_StateObj
*----------------------------------------------------------------------------
*/

signed long int
NotifyMbxDrv_driverExit (IN struct Notify_DriverHandle *handle)
{
	signed long int                status    = NOTIFY_SOK;
	signed long int                   tmpStatus = NOTIFY_SOK;
	struct NotifyDrv_EventListener *listener  = NULL;
	struct NotifyMbxDrvModule *driverObj;
	struct NotifyDrv_EventList *eventList;
	unsigned short int                       i;
	unsigned long int procId;


	driverObj = (struct NotifyMbxDrvModule *)&handle->driverObj;


	TRC_1ENTER("NotifyMbxDrv_driverExit", handle);

	DBC_require(NotifyMbxDrv_StateObj.iDriver_IsInit == TRUE);
	DBC_require(handle !=  NULL);
	DBC_require((handle !=  NULL) && (handle->driverObj != NULL));


	/*Uninstall the ISRs & Disable the Mailbox interrupt.*/
	if (NOTIFY_SUCCEEDED(status)) {

		struct MboxConfig *mBoxHwConfig = Mailbx_GetMBoxConfig();

		unsigned long int mBoxModuleNo = mBoxHwConfig->mBoxModules;

		signed long int interruptNo =
				mBoxHwConfig->interruptLines[mBoxModuleNo-1];

		signed long int mbxRetVal = KErrNone;

		/*Remove the ISR on the Modena-Tesla FIFO */

		TRC_0ENTER("Tesla Interrupt Removal");

		procId = PROC_TESLA;

		mbxRetVal = Mailbx_InterruptDisable(mBoxModuleNo,
					(NOTIFYDRV_TESLA_RECV_MBX * 2));



		mbxRetVal = Mailbx_Unregister(mBoxModuleNo,
					(NOTIFYDRV_TESLA_RECV_MBX * 2));



		TRC_1LEAVE("Tesla Interrupt Removal", mbxRetVal);



		/*Remove the generic ISR */
		mbxRetVal =
			Mailbx_UnBindInterrupt(interruptNo);

		if (mbxRetVal !=  KErrNone)
			TRC_1PRINT(TRC_LEVEL6, "UnBind \
			Interrupt failed[0x%x]", mbxRetVal);

	}

	procId = PROC_TESLA;

	if (procId == PROC_TESLA) {
		TRC_0ENTER("Tesla Driver Finalization");

		if (NotifyMbxDrv_StateObj.
			drvProcObjects[PROC_TESLA].regChart !=  NULL) {

			MEM_Free(NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].regChart);

			if (NOTIFY_SUCCEEDED(status)
				&& DSP_FAILED(tmpStatus)) {

				status = NOTIFY_EMEMORY;
				SET_FAILURE_REASON(status);
			}
			NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].regChart = NULL;
		}



		eventList = NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].eventList;



		for (i = 0; i < NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].
				eventsObj.nonShmEvents.numEvents; i++) {

			DBC_assert(eventList[i].eventHandlerCount == 0);
			eventList[i].eventHandlerCount = 0;

				if (eventList[i].listeners !=  NULL) {

					while ((list_empty(
						(struct list_head *)
						eventList[i].listeners)
						!=  TRUE)
						&& (NOTIFY_SUCCEEDED
						(status))) {

							listener =
							(struct
							NotifyDrv_EventListener
							*)
							(eventList[i].
							listeners);

						if (listener !=  NULL) {
							MEM_Free
							(listener);

							if ((NOTIFY_SUCCEEDED
								(status))) {

								status =
								NOTIFY_EMEMORY;


							}

						} else {

							status =
								NOTIFY_EFAIL;
							SET_FAILURE_REASON
								(status);
						}
					}

					list_del(
					(struct list_head *)
					eventList[i].listeners);

					eventList[i].listeners = NULL;
				}
		}

		/*Free allocated memory. */

		if (NotifyMbxDrv_StateObj.
			drvProcObjects[PROC_TESLA].eventList !=  NULL) {

			MEM_Free(NotifyMbxDrv_StateObj.
			drvProcObjects[PROC_TESLA].eventList);

			if (NOTIFY_SUCCEEDED(status)) {
				status = NOTIFY_EMEMORY;
				SET_FAILURE_REASON(status);
			}

			NotifyMbxDrv_StateObj.
					drvProcObjects[PROC_TESLA].
						eventList = NULL;
			}


		TRC_1LEAVE("Tesla Driver Finalization", status);
	}


	TRC_1LEAVE("NotifyMbxDrv_driverExit", status);

	return status;
}


/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_registerEvent
*
*@desc   Register a callback for an event with the Notify driver.
*
*@modif  eventList
*----------------------------------------------------------------------------
*/

signed long int
NotifyMbxDrv_registerEvent(IN struct Notify_DriverHandle *handle,
IN unsigned long int        procId,
IN unsigned long int              eventNo,
IN FnNotifyCbck        fnNotifyCbck,
IN void *cbckArg)
{
	signed long int                status    = NOTIFY_SOK;
	short int                         done      = FALSE;
	short int                         firstReg  = FALSE;
	struct NotifyDrv_EventListener *eventListener;
	struct NotifyDrv_EventList *eventList;
	struct NotifyMbxDrvModule *driverObj;
	struct NotifyShmDrv_EventRegEntry *regChart;
	signed long int                        i;
	signed long int                        j;


	TRC_5ENTER("NotifyMbxDrv_registerEvent",
					handle,
					procId,
					eventNo,
					fnNotifyCbck,
					cbckArg);

	driverObj = (struct NotifyMbxDrvModule *)
				&handle->driverObj;

	DBC_require(NotifyMbxDrv_StateObj.
					iDriver_IsInit == TRUE);

	DBC_require(fnNotifyCbck !=  NULL);

	DBC_require(handle !=  NULL);
	DBC_require((handle !=  NULL) && (handle->driverObj != NULL));


	if (procId == PROC_TESLA) {
		TRC_0ENTER("Tesla RegisterEvent");

		eventListener =
		MEM_Calloc(sizeof(struct NotifyDrv_EventListener),
						MEM_NONPAGED);

		if (eventListener == NULL) {
			status = NOTIFY_EMEMORY;
			SET_FAILURE_REASON(status);

		} else {

			eventList = NotifyMbxDrv_StateObj.
					drvProcObjects[PROC_TESLA].eventList;

			eventListener->fnNotifyCbck = fnNotifyCbck;
			eventListener->cbckArg   = cbckArg;

			DBC_assert(eventNo == 0);



			if (list_empty((struct list_head *)
				eventList[eventNo].listeners)) {

				firstReg = TRUE;
				list_add_tail((struct list_head *)
						&(eventListener->element),
						(struct list_head *)
						eventList[eventNo].listeners);

				eventList[eventNo].eventHandlerCount++;

			} else {
				status = NOTIFY_EFAIL;
				SET_FAILURE_REASON(status);
			}




			if (firstReg == TRUE) {
				regChart = NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].regChart;

				for (i = 0; i < NotifyMbxDrv_StateObj.
					drvProcObjects[PROC_TESLA].
					eventsObj.nonShmEvents.
						numEvents; i++) {


					if (regChart[i].regEventNo
						== (unsigned long int) -1) {

						for (j = (i - 1);
							j >= 0; j--) {

							if (eventNo
							< regChart[j].
							regEventNo) {

								regChart
								[j + 1].
								regEventNo  =
								regChart[j].
								regEventNo;

								regChart
								[j + 1].
								reserved  =
								regChart[j].
								reserved;
								i = j;
							} else {

								j = -1;
							}
						}

						regChart[i].regEventNo =
								eventNo;
						done = TRUE;
						break;
					}
				}


				if (1/*done*/) {
					/*Mark the event as registered */
					SET_BIT(NotifyMbxDrv_StateObj.
						drvProcObjects[PROC_TESLA].
						eventsObj.nonShmEvents.
						regMask.mask, eventNo);

				} else {
					status = NOTIFY_ERESOURCE;
					SET_FAILURE_REASON(status);
					list_del((struct list_head *)
						&(eventListener->element));
				}
			}

		}

		TRC_0LEAVE("Tesla RegisterEvent");
	}

	TRC_1LEAVE("NotifyMbxDrv_registerEvent", status);

	return status;
}


/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_unregisterEvent
*
*@desc   Unregister a callback for an event with the Notify driver.
*
*@modif  eventList
*----------------------------------------------------------------------------
*/

signed long int
NotifyMbxDrv_unregisterEvent(IN struct Notify_DriverHandle *handle,
IN unsigned long int        procId,
IN unsigned long int              eventNo,
IN FnNotifyCbck        fnNotifyCbck,
IN void *cbckArg)
{
	signed long int                status    = NOTIFY_SOK;
	struct NotifyDrv_EventListener *listener  = NULL;
	unsigned long int                       numEvents;
	struct NotifyMbxDrvModule *driverObj;
	struct NotifyDrv_EventList *eventList;
	struct NotifyShmDrv_EventRegEntry *regChart;
	struct NotifyShmDrv_Ctrl *ctrlPtr = NULL;
	struct NotifyDrv_EventListener   unregInfo;
	signed long int                        i;
	signed long int                        j;



	driverObj = (struct NotifyMbxDrvModule *)&handle->driverObj;


	TRC_5ENTER("NotifyMbxDrv_unregisterEvent",
					handle,
					procId,
					eventNo,
					fnNotifyCbck,
					cbckArg);

	DBC_require(NotifyMbxDrv_StateObj.iDriver_IsInit == TRUE);
	DBC_require(fnNotifyCbck !=  NULL);
	DBC_require(handle !=  NULL);
	DBC_require((handle !=  NULL) && (handle->driverObj != NULL));

	if (procId == PROC_TESLA) {
		TRC_0ENTER("Tesla UnregisterEvent");

		numEvents = NotifyMbxDrv_StateObj.
			drvProcObjects[PROC_TESLA].eventsObj.
			nonShmEvents.numEvents;


		eventList = NotifyMbxDrv_StateObj.
			drvProcObjects[PROC_TESLA].eventList;

		unregInfo.fnNotifyCbck = fnNotifyCbck;
		unregInfo.cbckArg      = cbckArg;

		NotifyMbxDrv_queSearchElement(eventList[eventNo].listeners,
								&unregInfo,
								&listener);

		if (listener !=  NULL) {
			list_del((struct list_head *)&(listener->element));
			eventList[eventNo].eventHandlerCount--;
			MEM_Free(listener);

			if (list_empty((struct list_head *)
				eventList[eventNo].listeners) == TRUE) {

				CLEAR_BIT(ctrlPtr->selfProcCtrl.
						regMask.mask, eventNo);

				regChart = NotifyMbxDrv_StateObj.
					drvProcObjects[PROC_TESLA].regChart;

				for (i = 0; i < numEvents; i++) {

					if (eventNo == regChart[i].regEventNo) {
						regChart[i].regEventNo =
							(unsigned long int) -1;

							for (j = (i + 1);

								(regChart[j].
								regEventNo !=
								(unsigned
								long int) -1)
								&&  (j !=
								numEvents);
								j++) {

								regChart
								[j - 1].
								regEventNo  =
								regChart[j].
								regEventNo;

								regChart[j - 1].
								reserved =
								regChart[j].
								reserved;
							}

							if (j == numEvents) {
								regChart
								[j - 1].
								regEventNo =
								(unsigned
								long int) -1;
							}

							break;
					}
				}
			}

		} else {
			status = NOTIFY_ENOTFOUND;
			SET_FAILURE_REASON(status);
		}

		TRC_0LEAVE("Tesla UnregisterEvent");
	}


	TRC_1LEAVE("NotifyMbxDrv_unregisterEvent", status);

	return status;
}


/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_sendEvent
*
*@desc   Send a notification event to the registered users for this
*notification on the specified processor.
*
*@modif  otherEventChart
*----------------------------------------------------------------------------
*/

signed long int
NotifyMbxDrv_sendEvent(IN struct Notify_DriverHandle *handle,
IN unsigned long int        procId,
IN unsigned long int              eventNo,
IN unsigned long int              payload,
IN short int                waitClear)
{
	signed long int status = NOTIFY_SOK;
	struct NotifyMbxDrvModule *driverObj;
	unsigned long int maxPollCount;
	struct MboxConfig *mBoxHwConfig = Mailbx_GetMBoxConfig();
	unsigned long int mBoxModuleNo = mBoxHwConfig->mBoxModules;
	signed long int mbxRetVal = KErrNone;


	TRC_5ENTER("NotifyMbxDrv_sendEvent",
					handle,
					procId,
					eventNo,
					payload,
					waitClear);

	DBC_require(NotifyMbxDrv_StateObj.iDriver_IsInit == TRUE);
	DBC_require(handle !=  NULL);
	DBC_require((handle !=  NULL) && (handle->driverObj != NULL));

	if (procId == PROC_TESLA) {

		TRC_0ENTER("Tesla Send");
		driverObj = (struct NotifyMbxDrvModule *)&handle->driverObj;


		DBC_assert(eventNo < NotifyMbxDrv_StateObj.
					drvProcObjects[PROC_TESLA].eventsObj.
					nonShmEvents.numEvents);

		maxPollCount = NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].
				eventsObj.nonShmEvents.sendEventPollCount;

		mbxRetVal =
			Mailbx_Send(mBoxModuleNo,
				NOTIFYDRV_TESLA_SEND_MBX, payload);


		if (mbxRetVal == KErrNone) {
			status = NOTIFY_SOK;
		} else {

			status = NOTIFY_EFAIL;
			SET_FAILURE_REASON(status);
		}

		TRC_1LEAVE("Tesla Send", mbxRetVal);

	}


	TRC_1LEAVE("NotifyMbxDrv_sendEvent", status);

	return status;
}


/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_disable
*
*@desc   Disable all events for this Notify driver.
*
*@modif  None
*----------------------------------------------------------------------------
*/
/*TODO:Need to introduce the procId not to affect the parallel IPC stacks*/

void *
NotifyMbxDrv_disable(IN struct Notify_DriverHandle *handle)
{
	signed long int mbxRetVal = KErrNone;
	struct MboxConfig *mBoxHwConfig = Mailbx_GetMBoxConfig();
	unsigned long int mBoxModuleNo = mBoxHwConfig->mBoxModules;
	signed long int procId;

	DBC_require(NotifyMbxDrv_StateObj.iDriver_IsInit == TRUE);
	TRC_1ENTER("NotifyMbxDrv_disable", handle);

	/*Disable the receive interrupt for Tesla */
	procId = PROC_TESLA;

	if (procId == PROC_TESLA) {
		mbxRetVal = Mailbx_InterruptDisable(mBoxModuleNo,
					(NOTIFYDRV_TESLA_RECV_MBX * 2));

		TRC_1PRINT(TRC_LEVEL1, "Tesla Disable[%d]", mbxRetVal);
	}


	TRC_1PRINT(TRC_LEVEL1, "  flags[%d]", NULL);
	TRC_0LEAVE("NotifyMbxDrv_disable");

	return NULL; /*No flags to be returned. */
}


/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_restore
*
*@desc   Restore the Notify driver to the state before the last disable was
*called.
*
*@modif  TBD
*----------------------------------------------------------------------------
*/
/*TODO: Need to introduce the procId not to affect the parallel IPC stacks*/

signed long int
NotifyMbxDrv_restore(IN struct Notify_DriverHandle *handle,
IN void *flags)
{
	signed long int mbxRetVal = KErrNone;
	struct MboxConfig *mBoxHwConfig = Mailbx_GetMBoxConfig();
	unsigned long int mBoxModuleNo = mBoxHwConfig->mBoxModules;
	signed long int procId;

	DBC_require(NotifyMbxDrv_StateObj.iDriver_IsInit == TRUE);
	TRC_2ENTER("NotifyMbxDrv_restore", handle, flags);

	(void) handle;
	(void) flags;

	/*Enable the receive interrupt for Tesla */
	procId = PROC_TESLA;

	if (procId == PROC_TESLA) {

		mbxRetVal = Mailbx_InterruptEnable(mBoxModuleNo,
				(NOTIFYDRV_TESLA_RECV_MBX * 2));

		TRC_1PRINT(TRC_LEVEL1, "Tesla Enable[%d]", mbxRetVal);
	}


	TRC_1LEAVE("NotifyMbxDrv_restore", NOTIFY_SOK);

	return NOTIFY_SOK;
}


/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_disableEvent
*
*@desc   Disable a specific event for this Notify driver.
*
*@modif  TBD
*----------------------------------------------------------------------------
*/

signed long int
NotifyMbxDrv_disableEvent(IN struct Notify_DriverHandle *handle,
IN unsigned long int        procId,
IN unsigned long int              eventNo)
{
	static int access_count ;
	signed long int        status = NOTIFY_SOK;
	struct NotifyMbxDrvModule *driverObj;

	access_count++;

	TRC_3ENTER("NotifyMbxDrv_disableEvent", handle, procId, eventNo);

	DBC_require(NotifyMbxDrv_StateObj.iDriver_IsInit == TRUE);
	DBC_require(handle !=  NULL);
	DBC_require((handle !=  NULL) && (handle->driverObj != NULL));
	DBC_require((procId == PROC_TESLA) || (procId == PROC_DUCATI));

	if (procId == PROC_TESLA) {
		TRC_0ENTER("Tesla DisableEvent");


	driverObj = (struct NotifyMbxDrvModule *)handle->driverObj;

	DBC_assert(eventNo < NotifyMbxDrv_StateObj.drvProcObjects[PROC_TESLA].
					eventsObj.nonShmEvents.numEvents);

		if (access_count == 1) {

			CLEAR_BIT(NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].
				eventsObj.nonShmEvents.
				regMask.enableMask, eventNo);
		}

		TRC_1LEAVE("Tesla DisableEvent", NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].eventsObj.
				nonShmEvents.regMask.enableMask);
	}



	TRC_1LEAVE("NotifyMbxDrv_disableEvent", status);

	return status;

}


/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_enableEvent
*
*@desc   Enable a specific event for this Notify driver.
*
*@modif  TBD
*----------------------------------------------------------------------------
*/

signed long int
NotifyMbxDrv_enableEvent(IN struct Notify_DriverHandle *handle,
IN unsigned long int        procId,
IN unsigned long int              eventNo)
{
	signed long int        status = NOTIFY_SOK;
	struct NotifyMbxDrvModule *driverObj;


	TRC_3ENTER("NotifyMbxDrv_enableEvent", handle,
					procId, eventNo);


	DBC_require(NotifyMbxDrv_StateObj.iDriver_IsInit == TRUE);
	DBC_require(handle !=  NULL);
	DBC_require((handle !=  NULL) && (handle->driverObj != NULL));
	DBC_require((procId == PROC_TESLA) || (procId == PROC_DUCATI));

	driverObj = (struct NotifyMbxDrvModule *)&handle->driverObj;


	if (procId == PROC_TESLA) {
		TRC_0ENTER("Tesla EnableEvent");

		DBC_assert(eventNo < NotifyMbxDrv_StateObj.
				drvProcObjects[PROC_TESLA].eventsObj.
				nonShmEvents.numEvents);

		SET_BIT(NotifyMbxDrv_StateObj.drvProcObjects[PROC_TESLA].
		eventsObj.nonShmEvents.regMask.enableMask, eventNo);


		TRC_1LEAVE("Tesla EnableEvent", NotifyMbxDrv_StateObj.
		drvProcObjects[PROC_TESLA].eventsObj.nonShmEvents.
						regMask.enableMask);
	}


	TRC_1LEAVE("NotifyMbxDrv_enableEvent", status);

	return status;
}


/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_debug
*
*@desc   Print debug information for the Notify driver.
*
*@modif  TBD
*----------------------------------------------------------------------------
*/

signed long int
NotifyMbxDrv_debug(IN struct Notify_DriverHandle *handle)
{
	signed long int status = NOTIFY_SOK;
	unsigned long int    procId;

	TRC_1ENTER("NotifyMbxDrv_debug", handle);

	procId = PROC_TESLA;

	if (procId == PROC_TESLA) {
		TRC_0PRINT(TRC_LEVEL1,
		"Tesla Debug: Nothing being printed currently.");
	}


	TRC_1LEAVE("NotifyMbxDrv_debug", status);

	return status;
}


/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_ShmISR
*
*@desc   This function implements the interrupt service routine for the
*interrupt received from the DSP.
*
*@modif  None.
*----------------------------------------------------------------------------
*/

void
NotifyMbxDrv_NonShmISR(IN void *refData)
{
	unsigned long int                       payload   = 0;
	unsigned long int                       i         = 0;
	struct list_head  *temp;
	unsigned long int                       j;
	struct NotifyShmDrv_EventRegEntry *regChart;
	unsigned long int                       eventNo;
	struct NotifyDrvProcModule *drvProcObject;
	struct MboxConfig *mBoxHwConfig = Mailbx_GetMBoxConfig();
	unsigned long int mBoxModuleNo = mBoxHwConfig->mBoxModules;
	signed long int mbxRetVal = KErrNone;
	unsigned long int numMessages = 0;
	unsigned long int numEvents = 0;

	/* Commented out, since this function will be called in an ISR */
	TRC_1ENTER("NotifyMbxDrv_NonShmISR", refData);

	drvProcObject = (struct NotifyDrvProcModule *) refData;

	DBC_require(drvProcObject != NULL);

	regChart = drvProcObject->regChart;
	numEvents = drvProcObject->eventsObj.nonShmEvents.numEvents;




	do {

		eventNo = regChart[i].regEventNo;
		if (eventNo != (unsigned long int) -1) {


			if ((TEST_BIT(drvProcObject->eventsObj.
			nonShmEvents.regMask.enableMask, eventNo)) == TRUE) {


				mbxRetVal = Mailbx_Read(mBoxModuleNo,
					NOTIFYDRV_TESLA_RECV_MBX, &payload,
					&numMessages, FALSE);


				if (mbxRetVal == KErrNone) {

					temp  = drvProcObject->eventList
						[eventNo].
						listeners->head.next;

					for (j = 0; j <
					drvProcObject->eventList[eventNo].
						eventHandlerCount; j++) {


						if (temp >
							(struct list_head *)0) {

							((struct
							NotifyDrv_EventListener
							 *)
							 temp)->fnNotifyCbck(
							drvProcObject->procId,
							 eventNo,
							((struct
							NotifyDrv_EventListener
							 *)
							 temp)->cbckArg,
							payload);

							temp = temp->next;
						}

					}
				} else {
					SET_FAILURE_REASON(NOTIFY_EFAIL);
				}
			}



			i++;
		}
	} while ((eventNo != (unsigned long int) -1) &&
					(i < numEvents));


}
EXPORT_SYMBOL(NotifyMbxDrv_NonShmISR);


/**----------------------------------------------------------------------------
*@func   NotifyMbxDrv_queSearchElement
*
*@desc   This function searchs for a element the List.
*
*@modif  None.
*----------------------------------------------------------------------------
*/
static
void
NotifyMbxDrv_queSearchElement(IN  struct lst_list *list,
IN  struct NotifyDrv_EventListener *checkObj,
OUT struct NotifyDrv_EventListener * *listener)
{
	struct list_head    *temp      = NULL ;
	struct NotifyDrv_EventListener *lTemp     = NULL ;
	short int                         found     = FALSE;

	TRC_3ENTER("NotifyMbxDrv_queSearchElement",
				list, checkObj, listener);

	DBC_require(list     !=  NULL);
	DBC_require(checkObj !=  NULL);
	DBC_require(listener !=  NULL);

	if (listener !=  NULL) {
		*listener = NULL;

		if ((list !=  NULL) && (checkObj != NULL)) {
			if (list_empty((struct list_head *)list) == FALSE) {
				temp = list->head.next;

				while ((found == FALSE) && (temp != NULL)) {

					lTemp =
					(struct NotifyDrv_EventListener *)
					(temp);

					if ((lTemp->fnNotifyCbck ==
					checkObj->fnNotifyCbck) &&
					(lTemp->cbckArg   ==
					checkObj->cbckArg)) {
							found = TRUE;
					} else {
							temp = temp->next;
					}

				}

				if (found == TRUE)
					*listener = lTemp;


			}
		}
	}

	TRC_1PRINT(TRC_LEVEL1, "  listener[0x%x]", *listener);
	TRC_0LEAVE("NotifyMbxDrv_queSearchElement");
}
