/*
 * arch/arm/plat-omap/include/mach/smp.h
 *
 * Copyright (C) 2007-2008 Texas Instruments, Inc.
 * Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * Interface functions needed for the SMP. This file is based on arm
 * realview file.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef OMAP_ARCH_SMP_H
#define OMAP_ARCH_SMP_H


#include <asm/hardware/gic.h>

#define hard_smp_processor_id()			\
	({						\
		unsigned int cpunum;			\
		__asm__("mrc p15, 0, %0, c0, c0, 5"	\
			: "=r" (cpunum));		\
		cpunum &= 0x0F;				\
	})

/*
 * We use IRQ1 as the IPI
 */
static inline void smp_cross_call(cpumask_t callmap)
{
	gic_raise_softirq(callmap, 1);
}

/*
 * Do nothing on MPcore.
 */
static inline void smp_cross_call_done(cpumask_t callmap)
{
}

#endif
