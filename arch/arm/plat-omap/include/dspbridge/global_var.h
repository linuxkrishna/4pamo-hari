#include<linux/interrupt.h>
/*defines*/
#define REG volatile

/*FILE: drivers/dsp/syslink/notify_mailbox/notify_dispatcher.c*/
void notify_mbxdrv_nonshm_isr(void *ref_data);


/*FILE: drivers/dsp/syslink/notify_mailbox/notify_mbxDriver.c*/
void notify_mbxdrv_nonshm_isr(void *ref_data);


/* FILE: drivers/dsp/syslink/omap_notify/notify.c*/

signed long int _notify_find_driver(char *driver_name, void  **handle);

/*File drv_notifymbxdrv.c*/
extern void notify_mbxdrv_nonshm_isr(void *ref_data);
extern short int notify_is_init;
extern struct notify_state notify_state_obj;
irqreturn_t notify_mailbx0_user0_isr(int temp, void *anArg, struct pt_regs *p);





