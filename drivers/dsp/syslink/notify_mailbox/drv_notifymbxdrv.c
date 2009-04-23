/*
 * drv_notifymbxdrv.c
 *
 * Syslink driver support functions for TI OMAP processors.
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


/*  ----------------------------------- OS Specific Headers         */
#include <linux/autoconf.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#include <linux/io.h>
#include <asm/pgtable.h>

/*  ----------------------------------- IPC headers                 */
#include <gpptypes.h>
#include <notifyerr.h>
#include <notifydefs.h>
#include <GlobalTypes.h>

/*  ----------------------------------- Notify headers              */
#include <notify_mbxDriver.h>
#include <global_var.h>


const unsigned long *linear_address;
EXPORT_SYMBOL(linear_address);


/* Major number of driver */
static signed long int major = 233 ;

#define OMAP_MBOX_BASE 0x4A0F4000
#define OMAP_MBOX_SIZE 0x2000

/* driver function to open the notify mailbox driver object. */
static int drvmbx_open(struct inode *inode, struct file *filp);

/* notify mailbox driver initialization function. */
static int __init drvmbx_initialize_module(void) ;

/* notify mailbox driver cleanup function. */
static void  __exit drvmbx_finalize_module(void) ;


/* Function to invoke the APIs through ioctl. */
static struct file_operations driver_ops = {
	.open = drvmbx_open,
};

/* Initialization function */
static int __init drvmbx_initialize_module(void)
{
	int result = 0;

	linear_address = (const unsigned long *)ioremap(OMAP_MBOX_BASE,
					OMAP_MBOX_SIZE);

	result = register_chrdev(major, "notifyshmdrv", &driver_ops);
	if (result < 0)
		pr_err("Notify mailbox driver initialization file\n");
	else
		notify_mbxdrv_init();
	return result ;
}

/* Finalization function */
static void __exit drvmbx_finalize_module(void)
{
	unregister_chrdev(major, "notifyshmdrv");
	notify_mbxdrv_exit();
}

static int drvmbx_open(struct inode *inode, struct file *filp)
{
	return 0 ;
}

MODULE_LICENSE("GPL");
module_init(drvmbx_initialize_module);
module_exit(drvmbx_finalize_module);
