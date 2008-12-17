/*
 * twl6030-irq.c - TWL6030 irq support
 *
 * Copyright (C) 2005-2008 Texas Instruments, Inc.
 *
 * Modifications to defer interrupt handling to a kernel thread:
 * Copyright (C) 2006 MontaVista Software, Inc.
 *
 * Based on tlv320aic23.c:
 * Copyright (c) by Kai Svahn <kai.svahn@nokia.com>
 *
 * Code cleanup and modifications to IRQ handler.
 * by syed khasim <x0khasim@ti.com>
 *
 * TWL6030 specific code and IRQ handling changes by
 * Jagadeesh Bhaskar Pakaravoor <j-pakaravoor@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kthread.h>

#include <linux/i2c/twl4030.h>


/*
 * TWL4030 IRQ handling has two stages in hardware, and thus in software.
 * The Primary Interrupt Handler (PIH) stage exposes status bits saying
 * which Secondary Interrupt Handler (SIH) stage is raising an interrupt.
 * SIH modules are more traditional IRQ components, which support per-IRQ
 * enable/disable and trigger controls; they do most of the work.
 *
 * These chips are designed to support IRQ handling from two different
 * I2C masters.  Each has a dedicated IRQ line, and dedicated IRQ status
 * and mask registers in the PIH and SIH modules.
 *
 * We set up IRQs starting at a platform-specified base, always starting
 * with PIH and the SIH for PWR_INT and then usually adding GPIO:
 *	base + 0  .. base + 7	PIH
 *	base + 8  .. base + 15	SIH for PWR_INT
 *	base + 16 .. base + 33	SIH for GPIO
 */

/* PIH register offsets */
#define REG_INT_STS_A			0x00
#define REG_INT_STS_B			0x01
#define REG_INT_STS_C			0x02

#define TWL4030_RTC_INT		370

static int twl6030_interrupt_mapping[24] = {
	-1,  /* Bit 0	PWRON		PWR_INT		*/
	-1,  /* Bit 1	RPWRON		PWR_INT		*/
	-1,  /* Bit 2	BAT_VLOW	PWR_INT		*/
	-1,  /* Bit 3	VBAT		PWR_INT		*/
	TWL4030_RTC_INT, /* Bit 4	RTC_ALARM	RTC_INT		*/
	TWL4030_RTC_INT, /* Bit 5	RTC_PERIOD	RTC_INT		*/
	-1,  /* Bit 6	HOT_DIE				*/
	-1,  /* Bit 7	Reserved			*/
	-1,  /*	Bit 8	VXXX_SHORT			*/
	-1,  /* Bit 9	VMMC_SHORT			*/
	-1,  /* Bit 10	VUSIM_SHORT			*/
	-1,  /* Bit 11	SIM				*/
	-1,  /* Bit 12	MMC				*/
	-1,  /* Bit 13	GPADC_RT_EOC			*/
	-1,  /* Bit 14	GPADC_SW_EOC			*/
	-1,  /* Bit 15	CC_AUTOCAL			*/
	-1,  /* Bit 16	ID_WKUP		USB_INT		*/
	-1,  /* Bit 17	VBUS_WKUP	USB_INT		*/
	-1,  /* Bit 18	ID		USB_INT		*/
	-1,  /* Bit 19	VBUS		USB_INT		*/
	-1,  /* Bit 20	CHRG_CTRL			*/
	-1,  /* Bit 21	EXT_CHRG			*/
	-1,  /* Bit 22	INT_CHRG			*/
	-1,  /* Bit 23	BAT_OVT				*/
};

/*----------------------------------------------------------------------*/

static unsigned twl4030_irq_base;

static struct completion irq_event;

/*
 * This thread processes interrupts reported by the Primary Interrupt Handler.
 */
static int twl4030_irq_thread(void *data)
{
	long irq = (long)data;
	irq_desc_t *desc = irq_desc + irq;
	static unsigned i2c_errors;
	const static unsigned max_i2c_errors = 100;

	current->flags |= PF_NOFREEZE;

	while (!kthread_should_stop()) {
		int ret;
		int i;
		u32 int_sts;

		/* Wait for IRQ, then read PIH irq status (also blocking) */
		wait_for_completion_interruptible(&irq_event);

		ret = twl4030_i2c_read(TWL4030_MODULE_PIH, &int_sts,
				REG_INT_STS_A, 3); /* read INT_STS_ * */
		if (ret) {
			pr_warning("twl4030: I2C error %d reading PIH ISR\n",
					ret);
			if (++i2c_errors >= max_i2c_errors) {
				printk(KERN_ERR "Maximum I2C error count"
						" exceeded.  Terminating %s.\n",
						__func__);
				break;
			}
			complete(&irq_event);
			continue;
		}


		ret = twl4030_i2c_write_u8(TWL4030_MODULE_PIH, 0,
				REG_INT_STS_A); /* clear INT_STS_A */

		/* these handlers deal with the relevant SIH irq status */
		local_irq_disable();

		int_sts = ((int_sts << 8) >> 8);

		for (i = 0; int_sts; int_sts >>= 1, i++) {
			if (int_sts & 0x1) {
				int module_irq = twl6030_interrupt_mapping[i];
				irq_desc_t *d = irq_desc + module_irq;

				/* These can't be masked ... always warn
				 * if we get any surprises.
				 */
				if (d->status & IRQ_DISABLED)
					note_interrupt(module_irq, d,
							IRQ_NONE);
				else
					d->handle_irq(module_irq, d);
			}
		}

#if 0
		for (module_irq = twl4030_irq_base;
				pih_isr;
				pih_isr >>= 1, module_irq++) {
			if (pih_isr & 0x1) {
				irq_desc_t *d = irq_desc + module_irq;

				/* These can't be masked ... always warn
				 * if we get any surprises.
				 */
				if (d->status & IRQ_DISABLED)
					note_interrupt(module_irq, d,
							IRQ_NONE);
				else
					d->handle_irq(module_irq, d);
			}
		}
#endif
		local_irq_enable();

		desc->chip->unmask(irq);
	}

	return 0;
}

/*
 * handle_twl4030_pih() is the desc->handle method for the twl4030 interrupt.
 * This is a chained interrupt, so there is no desc->action method for it.
 * Now we need to query the interrupt controller in the twl4030 to determine
 * which module is generating the interrupt request.  However, we can't do i2c
 * transactions in interrupt context, so we must defer that work to a kernel
 * thread.  All we do here is acknowledge and mask the interrupt and wakeup
 * the kernel thread.
 */
static void handle_twl4030_pih(unsigned int irq, irq_desc_t *desc)
{
	/* Acknowledge, clear *AND* mask the interrupt... */
	desc->chip->ack(irq);
	complete(&irq_event);
}

static struct task_struct *start_twl4030_irq_thread(long irq)
{
	struct task_struct *thread;

	init_completion(&irq_event);
	thread = kthread_run(twl4030_irq_thread, (void *)irq, "twl4030-irq");
	if (!thread)
		pr_err("twl4030: could not create irq %ld thread!\n", irq);

	return thread;
}

/*----------------------------------------------------------------------*/


static inline void activate_irq(int irq)
{
#ifdef CONFIG_ARM
	/* ARM requires an extra step to clear IRQ_NOREQUEST, which it
	 * sets on behalf of every irq_chip.  Also sets IRQ_NOPROBE.
	 */
	set_irq_flags(irq, IRQF_VALID);
#else
	/* same effect on other architectures */
	set_irq_noprobe(irq);
#endif
}

/*----------------------------------------------------------------------*/


static unsigned twl4030_irq_next;



/*----------------------------------------------------------------------*/

/* FIXME pass in which interrupt line we'll use ... */
#define twl_irq_line	0

int twl_init_irq(int irq_num, unsigned irq_base, unsigned irq_end)
{

	int			status = 0;
	int			i;
	struct task_struct	*task;

	static struct irq_chip	twl4030_irq_chip;

	twl4030_irq_base = irq_base;

	/* install an irq handler for each of the SIH modules;
	 * clone dummy irq_chip since PIH can't *do* anything
	 */
	twl4030_irq_chip = dummy_irq_chip;
	twl4030_irq_chip.name = "twl4030";
	twl4030_irq_chip.set_type = NULL;


	for (i = irq_base; i < irq_end; i++) {
		set_irq_chip_and_handler(i, &twl4030_irq_chip,
				handle_simple_irq);
		activate_irq(i);
	}
	twl4030_irq_next = i;
	pr_info("twl4030: %s (irq %d) chaining IRQs %d..%d\n", "PIH",
			irq_num, irq_base, twl4030_irq_next - 1);

	/* install an irq handler to demultiplex the TWL4030 interrupt */
	task = start_twl4030_irq_thread(irq_num);
	if (!task) {
		pr_err("twl4030: irq thread FAIL\n");
		status = -ESRCH;
		goto fail;
	}

	set_irq_data(irq_num, task);
	set_irq_chained_handler(irq_num, handle_twl4030_pih);
	return status;

fail:
	for (i = irq_base; i < irq_end; i++)
		set_irq_chip_and_handler(i, NULL, NULL);
	return status;
}

int twl_exit_irq(void)
{
	/* FIXME undo twl_init_irq() */
	if (twl4030_irq_base) {
		pr_err("twl4030: can't yet clean up IRQs?\n");
		return -ENOSYS;
	}
	return 0;
}
