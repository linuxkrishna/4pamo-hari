/*
 * ducatienabler.c
 *
 * Syslinksupport functions for TI OMAP processors.
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

/*
 *! Revision History:
 *! ================
 *! 20 Feb 2009 Hari K and Ramesh G: Initial version
 */


#include <ducatienabler.h>

#include <mem.h>
#include <errbase.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>

#ifdef DEBUG_DUCATI_IPC
#  define DPRINTK(fmt, args...) printk(KERN_INFO "%s: " fmt, __FUNCTION__ , \
								## args)
#else
#  define DPRINTK(fmt, args...)
#endif


#define DUCATI_NAME "Ducati"

static int ducatidrv_open(struct inode *inode, struct file *filp) ;


static int ducatidrv_close(struct inode *inode, struct file *filp) ;


static int ducatidrv_read(struct file *filp, char *dst, size_t  size, loff_t *offset) ;

int ducatidrv_ioctl(struct inode *inode, struct file *filp, unsigned int   cmd, unsigned long  args) ;

static int __init ducatidrv_initializeModule(void) ;

static void  __exit ducatidrv_finalizeModule(void) ;

static char *driver_name = DUCATI_NAME;

static s32 driver_major;
static s32 driver_minor;

struct ducati_dev {
	struct cdev cdev;
};

static struct ducati_dev *ducati_device;

static struct class *ducati_class;

/**----------------------------------------------------------------------------
 *@name   DriverOps
 *
 *@desc   Function to invoke the APIs through ioctl.
 *----------------------------------------------------------------------------
 */
static struct file_operations ducati_fops = {

	open :    ducatidrv_open,
	ioctl:    ducatidrv_ioctl,
	release:  ducatidrv_close,
	read:     ducatidrv_read,

} ;

static struct platform_driver ducati_driver_ldm = {
      .driver = {
	      .owner	= THIS_MODULE,
	      .name     = DUCATI_NAME,
	 },
      .probe = NULL,
      .shutdown = NULL,
      .remove = NULL,

};

static struct platform_device omap_ducati_dev = {
		.name = DUCATI_NAME,
		.id = -1,
		.num_resources = 0,
		.dev = {
		.release = ducatidrv_close,
		},
		.resource = NULL,
};

struct ducatiBaseImage {
	u32 physAddr;
	void *mpuVAaddr;
	u32 size;
	u32 align;
};

static struct ducatiBaseImage ducbaseimage = {NULL, NULL, 0x100000, 0x100000};

/**----------------------------------------------------------------------------
 *@func   ducatidrv_initializeModule
 *
 *@desc   Module initialization  function for Linux driver.
 *----------------------------------------------------------------------------
 */
static int __init ducatidrv_initializeModule(void)
{
	int retVal;
	int size = 0;
	int align;
	int status;
	dev_t   dev = 0 ;
	int     result;

	DPRINTK(" driver_init->\n");
		result = alloc_chrdev_region(&dev, driver_minor, 1,
					    driver_name);
		driver_major = MAJOR(dev);

	if (result < 0) {
		DPRINTK("ducati_init: "
				"Can't get Major %d \n", driver_major);
		return result;
	}

	ducati_device = kmalloc(sizeof(struct ducati_dev), GFP_KERNEL);
	if (!ducati_device) {
		result = -ENOMEM;
		unregister_chrdev_region(dev, 1);
		return result;
	}
	memset(ducati_device, 0, sizeof(struct ducati_dev));
	cdev_init(&ducati_device->cdev, &ducati_fops);
	ducati_device->cdev.owner = THIS_MODULE;
	ducati_device->cdev.ops = &ducati_fops;

	status = cdev_add(&ducati_device->cdev, dev, 1);

	if (status) {
		DPRINTK("Failed to add the ducati device \n");
		return status;
	}

	/* udev support */
	ducati_class = class_create(THIS_MODULE, "ti_ducati");

	if (IS_ERR(ducati_class))
		DPRINTK("Error creating ducati class \n");

	device_create(ducati_class, NULL, dev, NULL, "Ducati");

	status = platform_driver_register(&ducati_driver_ldm);
	if (!status)
		status = platform_device_register(&omap_ducati_dev);
	DPRINTK(" <- driver_init\n");

	retVal = initpage_attributes(0x4000, 14, 64) ;

	if (retVal == 0) {
		DPRINTK("  Allocating Physical Memory for Ducati Baseimage: Size [0x%x] Align [0x%x]", size, align);
		ducbaseimage.mpuVAaddr =
			MEM_AllocPhysMem(ducbaseimage.size, ducbaseimage.align, &ducbaseimage.physAddr);
		DPRINTK("  Allocated Memory: Physical Address [0x%x]", (u32)ducbaseimage.physAddr);
	}

	retVal = MMUInit((u32)ducbaseimage.physAddr, size);
	DPRINTK("  Leaving DDucatiEnablerLogicalDevice::Install [0x%x]", retVal);

	return 0;
}


/**----------------------------------------------------------------------------
 *@name   ducatidrv_finalizeModule
 *
 *@desc   Linux driver function to finalize the driver module.
 *----------------------------------------------------------------------------
	 */
static void __exit ducatidrv_finalizeModule(void)
{
	dev_t devno;

	MMUDeInit();
	MEM_FreePhysMem((void *)ducbaseimage.mpuVAaddr,
				ducbaseimage.physAddr, ducbaseimage.size);

	platform_device_unregister(&omap_ducati_dev);
	platform_driver_unregister(&ducati_driver_ldm);

	devno = MKDEV(driver_major, driver_minor);
	if (ducati_device) {
		cdev_del(&ducati_device->cdev);
		kfree(ducati_device);
	}
	unregister_chrdev_region(devno, 1);
	if (ducati_class) {
		/* remove the device from sysfs */
		device_destroy(ducati_class, MKDEV(driver_major, driver_minor));
		class_destroy(ducati_class);

	}
	DPRINTK("EXIT ducatidrv_finalizeModule\n");
}


/**----------------------------------------------------------------------------
 *@name   ducatidrv_open
 *
 *@desc   Linux specific function to open the driver.
 *----------------------------------------------------------------------------
 */
int ducatidrv_open(struct inode *inode, struct file *filp)
{
	return 0 ;
}


/**----------------------------------------------------------------------------
 *@name   DRV_close
 *
 *@desc   Linux specific function to close the driver.
 *----------------------------------------------------------------------------
 */
int ducatidrv_close(struct inode *inode, struct file *filp)
{
	return 0 ;
}


/**============================================================================
 *@name   ducatidrv_read
 *
 *@desc   Linux specific function to read data from the driver.
 *============================================================================
 */
int ducatidrv_read(struct file *filp, char *dst, size_t size, loff_t *offset)
{
	return 0 ;
}


/**----------------------------------------------------------------------------
 *@name   ducatidrv_ioctl
 *
 *@desc   ioctl function for of Linux Notify driver.
 *----------------------------------------------------------------------------
 */
int ducatidrv_ioctl(struct inode *inode,
		struct file *filp,
		unsigned int   cmd,
		unsigned long  args)
{

    return 0 ;
}

/**============================================================================
 *@name   module_init/module_exit
 *
 *@desc   Macro calls that indicate initialization and finalization functions
 *to the kernel.

 *============================================================================
 */
module_init(ducatidrv_initializeModule);
module_exit(ducatidrv_finalizeModule);
MODULE_AUTHOR("Texas Instruments");
MODULE_LICENSE("GPL");
