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
 *      mem_alloc
 *      mem_alloc_phymem
 *      mem_calloc
 *      mem_exit
 *      mem_flush_cache
 *      mem_free
 *      mem_free_phymem
 *      services_mem_init
 *      mem_ext_phypool_init
 *
 *! Revision History:
 *! =================
 *! 18-Jan-2004 hp: Added support for External physical memory pool
 *! 19-Apr-2004 sb: Added Alloc/Free PhysMem, FlushCache, VirtualToPhysical
 *! 01-Sep-2001 ag: Code cleanup.
 *! 02-May-2001 ag: MEM_[UnMap]linear_address revamped to align Phys to Virt.
 *!		 Set PAGE_PHYSICAL if phy addr <= 512MB. Opposite uSoft doc!
 *! 29-Aug-2000 rr: MEM_linear_address does not check for 512MB for non-x86.
 *! 28-Mar-2000 rr: MEM_linear_address changed.Handles address larger than 512MB
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

/*  ----------------------------------- Host OS */
#include<linux/list.h>
#include <host_os.h>

/*  ----------------------------------- DSP/BIOS Bridge */
#include <std.h>
#include <dbdefs.h>
#include <errbase.h>

/*  ----------------------------------- Trace & Debug */
#include <dbc.h>
#include <gt.h>

/*  ----------------------------------- This */
#include <mem.h>
#include <dspbridge/list.h>

/*  ----------------------------------- Defines */
#define MEM_512MB   0x1fffffff
#define mem_infoSign 0x464E494D	/* "MINF" (in reverse). */

#ifdef DEBUG
#define MEM_CHECK		/* Use to detect source of memory leaks */
#endif

/*  ----------------------------------- Globals */
#if GT_TRACE
static struct gt_mask mem_debug_mask = { NULL, NULL };	/* GT trace variable */
#endif

static u32 c_refs;		/* module reference count */

static bool ext_phys_mempool_enabled;

struct ext_phys_mem_pool {
	u32 phy_mem_base;
	u32 phys_mem_size;
	u32 virt_mem_base;
	u32 next_phy_alloc_p;
};

static struct ext_phys_mem_pool ext_mem_pool;

/*  Information about each element allocated on heap */
struct mem_info {
	struct list_head link;		/* Must be first */
	size_t size;
	void *caller;
	u32 dw_signature;	/* Should be last */
};

#ifdef MEM_CHECK

/*
 *  This structure holds a linked list to all memory elements allocated on
 *  heap by DSP/BIOS Bridge. This is used to report memory leaks and free
 *  such elements while removing the DSP/BIOS Bridge driver
 */
struct mem_man {
	struct lst_list lst;
	spinlock_t lock;
};

static struct mem_man m_man;


static void mem_check(void)
{
	struct mem_info *p_mem;
	struct list_head *last = &m_man.lst.head;
	struct list_head *curr = m_man.lst.head.next;

	if (!list_empty((struct list_head *)&m_man.lst)) {
		gt_0trace(mem_debug_mask, GT_7CLASS, "*** MEMORY LEAK ***\n");
		gt_0trace(mem_debug_mask, GT_7CLASS,
			  "Addr      Size      Caller\n");
		while (curr != last) {
			p_mem = (struct mem_info *)curr;
			curr = curr->next;
			if ((u32)p_mem > PAGE_OFFSET &&
			    mem_is_invalid_handle(p_mem, mem_infoSign)) {
				gt_3trace(mem_debug_mask, GT_7CLASS,
					"%lx  %d\t [<%p>]\n",
					(u32) p_mem + sizeof(struct mem_info),
					p_mem->size, p_mem->caller);
				list_del((struct list_head *) p_mem);
				kfree(p_mem);
			} else {
				gt_1trace(mem_debug_mask, GT_7CLASS,
					  "Invalid allocation or "
					  "Buffer underflow at %x\n",
				  (u32)p_mem +	sizeof(struct mem_info));
				break;
			}
		}
	}
	dbc_ensure(list_empty((struct list_head *)&m_man.lst));
}
EXPORT_SYMBOL(mem_check);
#endif

void mem_ext_phypool_init(u32 pool_phys_base, u32 poolSize)
{
	u32 pool_virt_base;

	/* get the virtual address for the physical memory pool passed */
	pool_virt_base = (u32)ioremap(pool_phys_base, poolSize);

	if ((void **)pool_virt_base == NULL) {
		gt_0trace(mem_debug_mask, GT_7CLASS,
			  "[PHYS_POOL]Mapping External "
			  "physical memory to virt failed \n");
		ext_phys_mempool_enabled = false;
	} else {
		ext_mem_pool.phy_mem_base = pool_phys_base;
		ext_mem_pool.phys_mem_size = poolSize;
		ext_mem_pool.virt_mem_base = pool_virt_base;
		ext_mem_pool.next_phy_alloc_p = pool_phys_base;
		ext_phys_mempool_enabled = true;
		gt_3trace(mem_debug_mask, GT_1CLASS,
			  "ExtMemory Pool details " "Pool"
			  "Physical mem base = %0x " "Pool Physical mem size "
			  "= %0x" "Pool Virtual mem base = %0x \n",
			  pool_phys_base, poolSize, pool_virt_base);
	}
}
EXPORT_SYMBOL(mem_ext_phypool_init);


static void mem_ext_phypool_release(void)
{
	gt_0trace(mem_debug_mask, GT_1CLASS,
		  "Releasing External memory pool \n");
	if (ext_phys_mempool_enabled) {
		iounmap((void *)(ext_mem_pool.virt_mem_base));
		ext_phys_mempool_enabled = false;
	}
}
EXPORT_SYMBOL(mem_ext_phypool_release);

/*
 *  ======== mem_ext_phymem_alloc ========
 *  Purpose:
 *     Allocate physically contiguous, uncached memory from external memory pool
 */

static void *mem_ext_phymem_alloc(u32 bytes, u32 align, OUT u32 *p_phy_addr)
{
	u32 new_alloc_p;
	u32 offset;
	u32 virt_addr;

	gt_2trace(mem_debug_mask, GT_1CLASS,
		  "Ext Memory Allocation" "bytes=0x%x , "
		  "align=0x%x \n", bytes, align);
	if (align == 0) {
		gt_0trace(mem_debug_mask, GT_7CLASS,
			  "ExtPhysical Memory Allocation "
			  "No alignment request in allocation call !! \n");
		align = 1;
	}
	if (bytes > ((ext_mem_pool.phy_mem_base + ext_mem_pool.phys_mem_size)
	    - ext_mem_pool.next_phy_alloc_p)) {
		gt_1trace(mem_debug_mask, GT_7CLASS,
			  "ExtPhysical Memory Allocation "
			  "unable to allocate memory for bytes = 0x%x \n",
			  bytes);
		p_phy_addr = NULL;
		return NULL;
	} else {
		offset = (ext_mem_pool.next_phy_alloc_p & (align - 1));
		if (offset == 0)
			new_alloc_p = ext_mem_pool.next_phy_alloc_p;
		else
			new_alloc_p = (ext_mem_pool.next_phy_alloc_p) +
				      (align - offset);
		if ((new_alloc_p + bytes) <=
		    (ext_mem_pool.phy_mem_base + ext_mem_pool.phys_mem_size)) {
			/* we can allocate */
			*p_phy_addr = new_alloc_p;
			ext_mem_pool.next_phy_alloc_p = new_alloc_p + bytes;
			virt_addr = ext_mem_pool.virt_mem_base + (new_alloc_p -
				   ext_mem_pool.phy_mem_base);
			gt_2trace(mem_debug_mask, GT_1CLASS,
				  "Ext Memory Allocation succedded "
				  "phys address=0x%x , virtaddress=0x%x \n",
				  new_alloc_p, virt_addr);
			return (void *)virt_addr;
		} else {
			*p_phy_addr = 0;
			return NULL;
		}
	}
}
EXPORT_SYMBOL(mem_ext_phymem_alloc);


/*
 *  ======== mem_alloc ========
 *  Purpose:
 *      Allocate memory from the paged or non-paged pools.
 */
void *mem_alloc(u32 c_bytes, enum mem_pool_attrs type)
{
	struct mem_info *p_mem = NULL;

	gt_2trace(mem_debug_mask, GT_ENTER,
		  "mem_alloc: c_bytes 0x%x\ttype 0x%x\n", c_bytes, type);
	if (c_bytes > 0) {
		switch (type) {
		case MEM_NONPAGED:
		/* If non-paged memory required, see note at top of file. */
		case MEM_PAGED:
#ifndef MEM_CHECK
			p_mem = kmalloc(c_bytes, GFP_ATOMIC);
#else
			p_mem = kmalloc(c_bytes + sizeof(struct mem_info),
			       GFP_ATOMIC);
			if (p_mem) {
				p_mem->size = c_bytes;
				p_mem->caller = __builtin_return_address(0);
				p_mem->dw_signature = mem_infoSign;

				spin_lock(&m_man.lock);

				list_add_tail((struct list_head *)p_mem,
					(struct list_head *)&m_man.lst);

				spin_unlock(&m_man.lock);

				p_mem = (void *)((u32)p_mem +
					sizeof(struct mem_info));
			}
#endif
			break;
		case MEM_LARGEVIRTMEM:
#ifndef MEM_CHECK
			/* FIXME - Replace with 'vmalloc' after BP fix */
			p_mem = __vmalloc(c_bytes, GFP_ATOMIC, PAGE_KERNEL);
#else
			/* FIXME - Replace with 'vmalloc' after BP fix */
			p_mem = __vmalloc((c_bytes + sizeof(struct mem_info)),
				GFP_ATOMIC, PAGE_KERNEL);
			if (p_mem) {
				p_mem->size = c_bytes;
				p_mem->caller = __builtin_return_address(0);
				p_mem->dw_signature = mem_infoSign;

				spin_lock(&m_man.lock);
				list_add_tail((struct list_head *)
					p_mem, (struct list_head *)&m_man.lst);
				spin_unlock(&m_man.lock);

				p_mem = (void *)((u32)p_mem +
					sizeof(struct mem_info));
			}
#endif
			break;

		default:
			gt_0trace(mem_debug_mask, GT_6CLASS,
				  "mem_alloc: unexpected "
				  "mem_pool_attrs value\n");
			break;
		}
	}

	return p_mem;
}
EXPORT_SYMBOL(mem_alloc);

/*
 *  ======== mem_alloc_phymem ========
 *  Purpose:
 *      Allocate physically contiguous, uncached memory
 */
void *mem_alloc_phymem(u32 c_bytes, u32 u_allign, OUT u32 *p_physical_addr)
{
	void *p_va_mem = NULL;
	dma_addr_t paMem;

	dbc_require(c_refs > 0);

	gt_2trace(mem_debug_mask, GT_ENTER,
		  "mem_alloc_phymem: c_bytes 0x%x\tu_allign"
		  "0x%x\n", c_bytes, u_allign);

	if (c_bytes > 0) {
		if (ext_phys_mempool_enabled) {
			p_va_mem = mem_ext_phymem_alloc(c_bytes, u_allign,
						    (u32 *)&paMem);
		} else
			p_va_mem = dma_alloc_coherent(NULL, c_bytes, &paMem,
						   GFP_ATOMIC);
		if (p_va_mem == NULL) {
			*p_physical_addr = 0;
			gt_1trace(mem_debug_mask, GT_6CLASS,
				  "mem_alloc_phymem failed: "
				  "0x%x\n", p_va_mem);
		} else {
			*p_physical_addr = paMem;
		}
	}
	return p_va_mem;
}
EXPORT_SYMBOL(mem_alloc_phymem);


/*
 *  ======== mem_calloc ========
 *  Purpose:
 *      Allocate zero-initialized memory from the paged or non-paged pools.
 */
void *mem_calloc(u32 c_bytes, enum mem_pool_attrs type)
{
	struct mem_info *p_mem = NULL;

	gt_2trace(mem_debug_mask, GT_ENTER,
		  "mem_calloc: c_bytes 0x%x\ttype 0x%x\n",
		  c_bytes, type);

	if (c_bytes > 0) {
		switch (type) {
		case MEM_NONPAGED:
		/* If non-paged memory required, see note at top of file. */
		case MEM_PAGED:
#ifndef MEM_CHECK
			p_mem = kmalloc(c_bytes, GFP_ATOMIC);
			if (p_mem)
				memset(p_mem, 0, c_bytes);

#else
			p_mem = kmalloc(c_bytes + sizeof(struct mem_info),
				      GFP_ATOMIC);
			if (p_mem) {
				memset((void *)((u32)p_mem +
					sizeof(struct mem_info)), 0, c_bytes);
				p_mem->size = c_bytes;
				p_mem->caller = __builtin_return_address(0);
				p_mem->dw_signature = mem_infoSign;
				spin_lock(&m_man.lock);
				list_add_tail((struct list_head *) p_mem,
					(struct list_head *)&m_man.lst);
				spin_unlock(&m_man.lock);
				p_mem = (void *)((u32)p_mem +
					sizeof(struct mem_info));
			}
#endif
			break;
		case MEM_LARGEVIRTMEM:
#ifndef MEM_CHECK
			/* FIXME - Replace with 'vmalloc' after BP fix */
			p_mem = __vmalloc(c_bytes, GFP_ATOMIC, PAGE_KERNEL);
			if (p_mem)
				memset(p_mem, 0, c_bytes);

#else
			/* FIXME - Replace with 'vmalloc' after BP fix */
			p_mem = __vmalloc(c_bytes + sizeof(struct mem_info),
				GFP_ATOMIC, PAGE_KERNEL);
			if (p_mem) {
				memset((void *)((u32)p_mem +
					sizeof(struct mem_info)), 0, c_bytes);
				p_mem->size = c_bytes;
				p_mem->caller = __builtin_return_address(0);
				p_mem->dw_signature = mem_infoSign;
				spin_lock(&m_man.lock);

				list_add_tail((struct list_head *)p_mem,
					(struct list_head *)&m_man.lst);

				spin_unlock(&m_man.lock);
				p_mem = (void *)((u32)p_mem +
					sizeof(struct mem_info));
			}
#endif
			break;
		default:
			gt_1trace(mem_debug_mask, GT_6CLASS,
				  "mem_calloc: unexpected "
				  "mem_pool_attrs value 0x%x\n", type);
			break;
		}
	}

	return p_mem;
}
EXPORT_SYMBOL(mem_calloc);

/*
 *  ======== mem_exit ========
 *  Purpose:
 *      Discontinue usage of the MEM module.
 */
void mem_exit(void)
{
	dbc_require(c_refs > 0);

	gt_1trace(mem_debug_mask, GT_5CLASS, "mem_exit: c_refs 0x%x\n", c_refs);

	c_refs--;
#ifdef MEM_CHECK
	if (c_refs == 0)
		mem_check();

#endif
	mem_ext_phypool_release();
	dbc_ensure(c_refs >= 0);
}
EXPORT_SYMBOL(mem_exit);

/*
 *  ======== mem_flush_cache ========
 *  Purpose:
 *      Flush cache
 */
void mem_flush_cache(void *p_memBuf, u32 c_bytes, s32 flush_type)
{
	dbc_require(c_refs > 0);

	switch (flush_type) {
	/* invalidate only */
	case PROC_INVALIDATE_MEM:
		dmac_inv_range(p_memBuf, p_memBuf + c_bytes - 1);
		outer_inv_range(__pa((u32)p_memBuf), __pa((u32)p_memBuf +
				c_bytes - 1));
	break;
	/* writeback only */
	case PROC_WRITEBACK_MEM:
		dmac_clean_range(p_memBuf, p_memBuf + c_bytes - 1);
		outer_clean_range(__pa((u32)p_memBuf), __pa((u32)p_memBuf +
				  c_bytes - 1));
	break;
	/* writeback and invalidate */
	case PROC_WRITEBACK_INVALIDATE_MEM:
		dmac_flush_range(p_memBuf, p_memBuf + c_bytes - 1);
		outer_flush_range(__pa((u32)p_memBuf), __pa((u32)p_memBuf +
				  c_bytes - 1));
	break;
	default:
		gt_1trace(mem_debug_mask, GT_6CLASS, "mem_flush_cache: invalid "
			  "FlushMemType 0x%x\n", flush_type);
	break;
	}

}
EXPORT_SYMBOL(mem_flush_cache);


/*
 *  ======== mem_free ========
 *  Purpose:
 *      Free the given block of system memory.
 */
void mem_free(IN void *p_memBuf)
{
#ifdef MEM_CHECK
	struct mem_info *p_mem =
	(void *)((u32)p_memBuf - sizeof(struct mem_info));
#endif

	dbc_require(p_memBuf != NULL);

	gt_1trace(mem_debug_mask, GT_ENTER, "mem_free: p_memBufs 0x%x\n",
		  p_memBuf);

	if (p_memBuf) {
#ifndef MEM_CHECK
		kfree(p_memBuf);
#else
		if (p_mem) {
			if (p_mem->dw_signature == mem_infoSign) {
				spin_lock(&m_man.lock);
				list_del((struct list_head *) p_mem);
				spin_unlock(&m_man.lock);
				p_mem->dw_signature = 0;
				kfree(p_mem);
			} else {
				gt_1trace(mem_debug_mask, GT_7CLASS,
					"Invalid allocation or "
					"Buffer underflow at %x\n",
					(u32) p_mem + sizeof(struct mem_info));
			}
		}
#endif
	}
}
EXPORT_SYMBOL(mem_free);
/*
 *  ======== mem_free_phymem ========
 *  Purpose:
 *      Free the given block of physically contiguous memory.
 */
void mem_free_phymem(void *pVirtualAddress, u32 p_physical_addr,
		     u32 c_bytes)
{
	dbc_require(c_refs > 0);
	dbc_require(pVirtualAddress != NULL);

	gt_1trace(mem_debug_mask, GT_ENTER, "mem_free_phymem: pVirtualAddress "
		  "0x%x\n", pVirtualAddress);

	if (!ext_phys_mempool_enabled)
		dma_free_coherent(NULL, c_bytes, pVirtualAddress,
				 p_physical_addr);
}
EXPORT_SYMBOL(mem_free_phymem);
/*
 *  ======== services_mem_init ========
 *  Purpose:
 *      Initialize MEM module private state.
 */
bool services_mem_init(void)
{
	dbc_require(c_refs >= 0);

	if (c_refs == 0) {
		GT_create(&mem_debug_mask, "MM");	/* MM for MeM module */

#ifdef MEM_CHECK
		m_man.lst.head.next = &m_man.lst.head;
		m_man.lst.head.prev = &m_man.lst.head;
		m_man.lst.head.self = NULL;
#endif

	}

	c_refs++;

	gt_1trace(mem_debug_mask, GT_5CLASS,
	"services_mem_init: c_refs 0x%x\n", c_refs);

	dbc_ensure(c_refs > 0);

	return true;
}
EXPORT_SYMBOL(services_mem_init);
