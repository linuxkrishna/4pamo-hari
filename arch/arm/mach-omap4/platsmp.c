/* linux/arch/arm/mach-omap4/platsmp.c
 *
 * Copyright (C) 2009 Texas Instruments, Inc.
 * Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * This file is based on arm realview platform file.
 *
 * Copyright (C) 2002 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/smp.h>
#include <linux/io.h>

#include <asm/cacheflush.h>
#include <mach/scu.h>


/*
 * control for which core is the next to come out of the secondary
 * boot "holding pen"
 */
volatile int __cpuinitdata pen_release = -1;

static unsigned int __init get_core_count(void)
{
	unsigned int ncores;
	void __iomem *scu_base = OMAP44XX_VA_SCU_BASE;

	if (scu_base) {
		ncores = __raw_readl(scu_base + SCU_CONFIG);
		ncores = (ncores & 0x03) + 1;
	} else
		ncores = 1;

	return ncores;
}
/*
 * Setup the SCU
 */
static void scu_enable(void)
{
	u32 scu_ctrl;
	void __iomem *scu_base = OMAP44XX_VA_SCU_BASE;

	scu_ctrl = __raw_readl(scu_base + SCU_CTRL);
	scu_ctrl |= 1;
	__raw_writel(scu_ctrl, scu_base + SCU_CTRL);
}

static DEFINE_SPINLOCK(boot_lock);

void __cpuinit platform_secondary_init(unsigned int cpu)
{
	trace_hardirqs_off();

	/*
	 * the primary core may have used a "cross call" soft interrupt
	 * to get this processor out of WFI in the BootMonitor - make
	 * sure that we are no longer being sent this soft interrupt
	 */
	smp_cross_call_done(cpumask_of_cpu(cpu));

	/*
	 * if any interrupts are already enabled for the primary
	 * core (e.g. timer irq), then they will not have been enabled
	 * for us: do so
	 */

	gic_cpu_init(0, IO_ADDRESS(OMAP44XX_GIC_CPU_BASE));

	/*
	 * let the primary processor know we're out of the
	 * pen, then head off into the C entry point
	 */
	pen_release = -1;
	smp_wmb();

	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
}

int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;

	/*
	 * set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);

	/*
	 * The secondary processor is waiting to be released from
	 * the holding pen - release it, then wait for it to flag
	 * that it has been released by resetting pen_release.
	 *
	 * Note that "pen_release" is the hardware CPU ID, whereas
	 * "cpu" is Linux's internal ID.
	 */
#if 0	/* Enable this when L2 Support is in place */
	/*
	 * Flush the page tables and page table pointer + initial
	 * stack pointer into main memory.Do this before the write
	 * to pen_release, as the secondary could otherwise see the
	 * release before the startup data is ready.
	 */
	wmb();
	dmac_clean_range(&secondary_data, &secondary_data + 1);
	outer_clean_range(__pa((unsigned)&secondary_data),
			__pa((unsigned)(&secondary_data + 1)));
#endif
	pen_release = cpu;
	flush_cache_all();
#if 0	/* Enable this when L2 Support is in place */
	wmb();

	/*
	 * Other CPUs won't have enabled L2 cache yet, clean requiredi
	 * data to L3 memory
	 */
	dmac_clean_range(&pen_release, &pen_release + 1);
	outer_clean_range(__pa((unsigned)&pen_release),
			__pa((unsigned)(&pen_release + 1)));
#endif
	/*
	 * Send a soft interrupt to wake the secondary core.
	 * Use smp_cross_call() for this
	 */
	smp_cross_call(cpumask_of_cpu(cpu));

	timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, timeout)) {
		smp_rmb();
		if (pen_release == -1)
			break;

		udelay(10);
	}

	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&boot_lock);

	return pen_release != -1 ? -ENOSYS : 0;
}

static void __init poke_milo(void)
{

	/* nobody is to be released from the pen yet */
	pen_release = -1;

	smp_cross_call(cpumask_of_cpu(1));
	mb();
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */
void __init smp_init_cpus(void)
{
	unsigned int i, ncores = get_core_count();

	for (i = 0; i < ncores; i++)
		cpu_set(i, cpu_possible_map);
}

void __init smp_prepare_cpus(unsigned int max_cpus)
{
	unsigned int ncores = get_core_count();
	unsigned int cpu = smp_processor_id();
	int i;

	/* sanity check */
	if (ncores == 0) {
		printk(KERN_ERR
		       "OMAP4: strange core count of 0? Default to 1\n");

		ncores = 1;
	}

	if (ncores > NR_CPUS) {
		printk(KERN_WARNING
		       "OMAP4: no. of cores (%d) greater than configured "
		       "maximum of %d - clipping\n",
		       ncores, NR_CPUS);
		ncores = NR_CPUS;
	}

	smp_store_cpu_info(cpu);

	/*
	 * are we trying to boot more cores than exist?
	 */
	if (max_cpus > ncores)
		max_cpus = ncores;

#ifdef CONFIG_LOCAL_TIMERS
	/*
	 * Enable the local timer for primary CPU. If the device is
	 * dummy (!CONFIG_LOCAL_TIMERS), it was already registers in
	 * omap_timer_init
	 */
	local_timer_setup();
#endif

	/*
	 * Initialise the present map, which describes the set of CPUs
	 * actually populated at the present time.
	 */
	for (i = 0; i < max_cpus; i++)
		cpu_set(i, cpu_present_map);

	/*
	 * Initialise the SCU if there are more than one CPU and let
	 * them know where to start. Note that, on modern versions of
	 * MILO, the "poke" doesn't actually do anything until each
	 * individual core is sent a soft interrupt to get it out of
	 * WFI
	 */
	if (max_cpus > 1) {
		scu_enable();
		poke_milo();
	}
}

