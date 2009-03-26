#include<ipctypes.h>
#include<linux/interrupt.h>
/*defines*/
#define REG volatile

/*FILE: drivers/dsp/syslink/notify_mailbox/notify_dispatcher.c*/
void NotifyMbxDrv_NonShmISR(IN void *refData);


/*FILE: drivers/dsp/syslink/notify_mailbox/notify_mbxDriver.c*/
void NotifyMbxDrv_NonShmISR(IN void *refData);


/* FILE: drivers/dsp/syslink/omap_notify/notify.c*/

signed long int _Notify_findDriver(IN  char *driverName, OUT void  **handle);

/*File drv_notifymbxdrv.c*/
extern void NotifyMbxDrv_NonShmISR(IN void *refData);
extern short int Notify_isInit;
extern struct Notify_State Notify_StateObj;
irqreturn_t Notify_Mailbox0User0_ISR(int temp, void *anArg, struct pt_regs *p);





