/*
 * mem.c
 *
 * DSP-BIOS Bridge driver support functions for TI OMAP processors.
 *
 * Copyright (C) 2005-2006 Texas Instruments, Inc.
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
 *  ======== mem.c ========
 *  Purpose:
 *      Implementation of platform specific memory services.
 *
 *  Public Functions:
 *      MEM_Alloc
 *      MEM_AllocPhysMem
 *      MEM_Calloc
 *      MEM_Exit
 *      MEM_FlushCache
 *      MEM_Free
 *      MEM_FreePhysMem
 *      MEM_Init
 *      MEM_ExtPhysPoolInit
 *
 *! Revision History:
 *! =================
 *! 18-Jan-2004 hp: Added support for External physical memory pool
 *! 19-Apr-2004 sb: Added Alloc/Free PhysMem, FlushCache, VirtualToPhysical
 *! 01-Sep-2001 ag: Code cleanup.
 *! 02-May-2001 ag: MEM_[UnMap]LinearAddress revamped to align Phys to Virt.
 *!		 Set PAGE_PHYSICAL if phy addr <= 512MB. Opposite uSoft doc!
 *! 29-Aug-2000 rr: MEM_LinearAddress does not check for 512MB for non-x86.
 *! 28-Mar-2000 rr: MEM_LinearAddress changed.Handles address larger than 512MB
 *! 03-Feb-2000 rr: Module init/exit is handled by SERVICES Init/Exit.
 *!		 GT Changes.
 *! 22-Nov-1999 kc: Added changes from code review.
 *! 16-Aug-1999 kc: modified for WinCE.
 *! 20-Mar-1999 ag: SP 4 fix in MEM_UMBCalloc().
 *!		 Mdl offset now ORed not added to userBuf.
 *! 23-Dec-1997 cr: Code review changes.
 *! 08-Dec-1997 cr: Prepared for code review.
 *! 24-Jun-1997 cr: Created.
 */

/*  ----------------------------------- This */
#include <mem.h>



static bool extPhysMemPoolEnabled;

struct extPhysMemPool {
	u32 physMemBase;
	u32 physMemSize;
	u32 virtMemBase;
	u32 nextPhysAllocPtr;
};

static struct extPhysMemPool extMemPool;




void MEM_ExtPhysPoolInit(u32 poolPhysBase, u32 poolSize)
{
	u32 poolVirtBase;

	/* get the virtual address for the physical memory pool passed */
	poolVirtBase = (u32)ioremap(poolPhysBase, poolSize);

	if ((void **)poolVirtBase == NULL) {
		printk( "[PHYS_POOL]Mapping External "
			  "physical memory to virt failed \n");
		extPhysMemPoolEnabled = false;
	} else {
		extMemPool.physMemBase = poolPhysBase;
		extMemPool.physMemSize = poolSize;
		extMemPool.virtMemBase = poolVirtBase;
		extMemPool.nextPhysAllocPtr = poolPhysBase;
		extPhysMemPoolEnabled = true;
		printk( "ExtMemory Pool details " "Pool"
			  "Physical mem base = %0x " "Pool Physical mem size "
			  "= %0x" "Pool Virtual mem base = %0x \n",
			  poolPhysBase, poolSize, poolVirtBase);
	}
}

static void MEM_ExtPhysPoolRelease(void)
{
	printk(  "Releasing External memory pool \n");
	if (extPhysMemPoolEnabled) {
		iounmap((void *)(extMemPool.virtMemBase));
		extPhysMemPoolEnabled = false;
	}
}

/*
 *  ======== MEM_ExtPhysMemAlloc ========
 *  Purpose:
 *     Allocate physically contiguous, uncached memory from external memory pool
 */

static void *MEM_ExtPhysMemAlloc(u32 bytes, u32 align, u32 *pPhysAddr)
{
	u32 newAllocPtr;
	u32 offset;
	u32 virtAddr;

	printk( "Ext Memory Allocation" "bytes=0x%x , "
		  "align=0x%x \n", bytes, align);
	if (align == 0) {
		printk(  "ExtPhysical Memory Allocation "
			  "No alignment request in allocation call !! \n");
		align = 1;
	}
	if (bytes > ((extMemPool.physMemBase + extMemPool.physMemSize)
	    - extMemPool.nextPhysAllocPtr)) {
		printk(  "ExtPhysical Memory Allocation "
			  "unable to allocate memory for bytes = 0x%x \n",
			  bytes);
		pPhysAddr = NULL;
		return NULL;
	} else {
		offset = (extMemPool.nextPhysAllocPtr & (align - 1));
		if (offset == 0)
			newAllocPtr = extMemPool.nextPhysAllocPtr;
		else
			newAllocPtr = (extMemPool.nextPhysAllocPtr) +
				      (align - offset);
		if ((newAllocPtr + bytes) <=
		    (extMemPool.physMemBase + extMemPool.physMemSize)) {
			/* we can allocate */
			*pPhysAddr = newAllocPtr;
			extMemPool.nextPhysAllocPtr = newAllocPtr + bytes;
			virtAddr = extMemPool.virtMemBase + (newAllocPtr -
				   extMemPool.physMemBase);
			printk( "Ext Memory Allocation succedded "
				  "phys address=0x%x , virtaddress=0x%x \n",
				  newAllocPtr, virtAddr);
			return (void *)virtAddr;
		} else {
			*pPhysAddr = 0;
			return NULL;
		}
	}
}

/*
 *  ======== MEM_Alloc ========
 *  Purpose:
 *      Allocate memory from the paged or non-paged pools.
 */
void *MEM_Alloc(u32 cBytes, enum MEM_POOLATTRS type)
{
	struct memInfo *pMem = NULL;

	printk( "MEM_Alloc: cBytes 0x%x\ttype 0x%x\n", cBytes, type);
	if (cBytes > 0) {
		switch (type) {
		case MEM_NONPAGED:
		/* If non-paged memory required, see note at top of file. */
		case MEM_PAGED:
			pMem = kmalloc(cBytes, GFP_ATOMIC);

			break;
		case MEM_LARGEVIRTMEM:
			/* FIXME - Replace with 'vmalloc' after BP fix */
			pMem = __vmalloc(cBytes, GFP_ATOMIC, PAGE_KERNEL);

			break;

		default:
			printk( "MEM_Alloc: unexpected "
				  "MEM_POOLATTRS value\n");
			break;
		}
	}

	return pMem;
}

/*
 *  ======== MEM_AllocPhysMem ========
 *  Purpose:
 *      Allocate physically contiguous, uncached memory
 */
void *MEM_AllocPhysMem(u32 cBytes, u32 ulAlign, u32 *pPhysicalAddress)
{
	void *pVaMem = NULL;
	dma_addr_t paMem;

	printk( "MEM_AllocPhysMem: cBytes 0x%x\tulAlign"
		  "0x%x\n", cBytes, ulAlign);

	if (cBytes > 0) {
		if (extPhysMemPoolEnabled) {
			pVaMem = MEM_ExtPhysMemAlloc(cBytes, ulAlign,
						    (u32 *)&paMem);
		} else
			pVaMem = dma_alloc_coherent(NULL, cBytes, &paMem,
						   GFP_ATOMIC);
		if (pVaMem == NULL) {
			*pPhysicalAddress = 0;
			printk(  "MEM_AllocPhysMem failed: "
				  "0x%x\n", (u32)pVaMem);
		} else {
			*pPhysicalAddress = paMem;
		}
	}
	return pVaMem;
}

/*
 *  ======== MEM_Calloc ========
 *  Purpose:
 *      Allocate zero-initialized memory from the paged or non-paged pools.
 */
void *MEM_Calloc(u32 cBytes, enum MEM_POOLATTRS type)
{
	struct memInfo *pMem = NULL;

	printk( "MEM_Calloc: cBytes 0x%x\ttype 0x%x\n",
		  cBytes, type);

	if (cBytes > 0) {
		switch (type) {
		case MEM_NONPAGED:
		/* If non-paged memory required, see note at top of file. */
		case MEM_PAGED:
			pMem = kmalloc(cBytes, GFP_ATOMIC);
			if (pMem)
				memset(pMem, 0, cBytes);
			break;
		case MEM_LARGEVIRTMEM:
			/* FIXME - Replace with 'vmalloc' after BP fix */
			pMem = __vmalloc(cBytes, GFP_ATOMIC, PAGE_KERNEL);
			if (pMem)
				memset(pMem, 0, cBytes);

			break;
		default:
			printk( "MEM_Calloc: unexpected "
				  "MEM_POOLATTRS value 0x%x\n", type);
			break;
		}
	}

	return pMem;
}


/*
 *  ======== MEM_Free ========
 *  Purpose:
 *      Free the given block of system memory.
 */
void MEM_Free(void *pMemBuf)
{

	if (pMemBuf == NULL)
		return;

	printk("MEM_Free: pMemBufs 0x%x\n",
		  pMemBuf);

	if (pMemBuf) {
		kfree(pMemBuf);
	}
}

/*
 *  ======== MEM_FreePhysMem ========
 *  Purpose:
 *      Free the given block of physically contiguous memory.
 */
void MEM_FreePhysMem(void *pVirtualAddress, u32 pPhysicalAddress,
		     u32 cBytes)
{
	if (pVirtualAddress == NULL)
		return NULL;

	printk( "MEM_FreePhysMem: pVirtualAddress "
		  "0x%x\n", pVirtualAddress);

	if (!extPhysMemPoolEnabled)
		dma_free_coherent(NULL, cBytes, pVirtualAddress,
				 pPhysicalAddress);
}


