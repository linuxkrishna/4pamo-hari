/*
 * notify_dispatcher.h
 *
 * Notify driver support for OMAP Processors.
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


#ifndef __TMBX_H__
#define __TMBX_H__


#include <gpptypes.h>
#include <notifydefs.h>
#include <linux/interrupt.h>

#include <notifyerr.h>

#define MAX_MBOX_MODULES 2
#define MAX_MBOX_ISRS 32
#define KErrNone 0
#define KErrNotSupported 1
#define KErrNotReady 2
#define KErrArgument 2

typedef void (*isr_call_back)(void *);

struct mbox_config {
	unsigned long int mbox_linear_addr;
	unsigned long int mbox_modules;
	signed long int interrupt_lines[MAX_MBOX_MODULES];
	signed long int mailboxes[MAX_MBOX_MODULES];
};

struct mbox_isrs {
	signed long int isrNo[MAX_MBOX_MODULES];
	/* TODO: Remove this - seems to be unused.*/
	isr_call_back isrs[MAX_MBOX_MODULES][MAX_MBOX_ISRS];
	void *isr_params[MAX_MBOX_MODULES][MAX_MBOX_ISRS];
};

extern const unsigned long *linear_address;

irqreturn_t notify_mailbx0_user0_isr(int temp, void *anArg, struct pt_regs *p);

/*
 *func    ntfy_disp_bind_interrupt
 * 
 * desc    Bind an ISR to the HW interrupt line coming into the processor
 */
signed long int ntfy_disp_bind_interrupt(signed long int interrupt_no,
			isr_call_back hw_isr,
			void *isr_arg);


/*
 * func    Mailbx::Debug
 * desc    Print the mailbox registers and other useful debug information
 * 
 */
void Mailbx_Debug(void);


/*
 * func    ntfy_disp_deinit
 * desc    Uninitialize the Mailbox Manager module
 */
signed long int ntfy_disp_deinit(void);


/*
 * func    Mailbx::Getmbox_config
 * desc    Return the pointer to the Mailbox Manager's configuration object
 */
struct mbox_config *mailbx_get_config(void);


/*
 * func    Mailbx::Init
 * desc    Initialize the Mailbox Manager module
 */
signed long int mailbx_init(void);


/*
 * func    Mailbx::InterruptDisable
 * 
 * desc    Disable a particular IRQ bit on a Mailbox IRQ Enable Register
 */
signed long int mailbx_interrupt_disable(unsigned long int mbox_module_no,
				unsigned long int a_irq_bit);


/*
 * func    Mailbx::InterruptEnable
 * desc    Enable a particular IRQ bit on a Mailbox IRQ Enable Register
 */
signed long int mailbx_interrupt_enable(unsigned long int mbox_module_no,
				unsigned long int a_irq_bit);


/*
 * func    Mailbx::Read
 * desc    Read a message on a Mailbox FIFO queue
 */
signed long int mailbx_read(unsigned long int mbox_module_no,
				unsigned long int a_mbox_no,
				unsigned long int *messages,
				unsigned long int *num_messages,
				short int read_all);


/*
 * func    Mailbx::Register
 * desc    Register a ISR callback associated with a particular IRQ bit on a
 * Mailbox IRQ Enable Register
 */
signed long int mailbx_register(unsigned long int mbox_module_no,
			unsigned long int a_irq_bit,
			isr_call_back isr_cbck_fn,
			void *isrCallbackArgs);


/*
 * func    Mailbx::Send
 * desc    Send a message on a Mailbox FIFO queue
 */
signed long int mailbx_send(unsigned long int mbox_module_no,
			unsigned long int a_mbox_no,
			unsigned long int message);


/*
 * func    Mailbx::UnBindInterrupt
 * desc    Remove the ISR to the HW interrupt line coming into the processor
 */
signed long int mailbx_unbind_interrupt(signed long int interrupt_no);


/*
 * func    Mailbx::Unregister
 * desc    Unregister a ISR callback associated with a particular IRQ bit on a
 * Mailbox IRQ Enable Register
 */
signed long int mailbx_unregister(unsigned long int mbox_module_no,
			unsigned long int a_irq_bit);


#ifdef DResource
DResourceHandler *iResourceHandler;
#endif

#endif
