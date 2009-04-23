/*
 * mem.h
 *
 * DSP-BIOS Bridge driver support functions for TI OMAP processors.
 *
 * Copyright (C) 2008 Texas Instruments, Inc.
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
 *  ======== mem.h ========
 *  Purpose:
 *      Memory management and address mapping services for the DSP/BIOS Bridge
 *      class driver and mini-driver.
 *
 *  Public Functions:
 *      mem_alloc
 *      mem_allocObject
 *      mem_alloc_phymem
 *      mem_calloc
 *      mem_exit
 *      mem_flush_cache
 *      mem_free
 *      mem_free_obj
 *      mem_free_phymem
 *      MEM_GetNumPages
 *      services_mem_init
 *      mem_is_invalid_handle
 *      MEM_linear_address
 *      MEM_PageLock
 *      MEM_PageUnlock
 *      MEM_UnMaplinear_address
 *      MEM_VirtualToPhysical
 *
 *  Notes:
 *
 *! Revision History:
 *! ================
 *! 19-Apr-2004 sb: Added Alloc/Free PhysMem, FlushCache, VirtualToPhysical
 *! 01-Sep-2001 ag: Cleaned up notes for MEM_linear_address() does not
 *!                   require phys address to be page aligned!
 *! 02-Dec-1999 rr: stdwin.h included for retail build
 *! 12-Nov-1999 kc: Added warning about use of MEM_linear_address.
 *! 29-Oct-1999 kc: Cleaned up for code review.
 *! 10-Aug-1999 kc: Based on wsx-c18.
 *! 07-Jan-1998 gp: Added mem_allocUMB and MEM_UMBFree for User Mapped Buffers
 *!                 used by WMD_CHNL.
 *! 23-Dec-1997 cr: Code review cleanup, removed dead Ring 3 code.
 *! 04-Aug-1997 cr: Added explicit CDECL identifiers.
 *! 01-Nov-1996 gp: Updated based on code review.
 *! 04-Sep-1996 gp: Added MEM_PageLock() and MEM_PageUnlock() services.
 *! 14-Aug-1996 mg: Added MEM_GetPhysAddr() and MEM_GetNumPages()
 *! 25-Jul-1996 gp: Added mem_is_invalid_handle() macro.
 *! 10-May-1996 gp: Added mem_calloc().
 *! 25-Apr-1996 gp: Added MEM_PhysicalAddress()
 *! 17-Apr-1996 gp: Added mem_exit function; updated to latest naming standard.
 *! 08-Apr-1996 gp: Created.
 */

#ifndef MEM_
#define MEM_
#include <host_os.h>

/* Memory Pool Attributes: */
enum mem_pool_attrs {
	MEM_PAGED = 0,
	MEM_NONPAGED = 1,
	MEM_LARGEVIRTMEM = 2
};


/*
 *  ======== mem_alloc ========
 *  Purpose:
 *      Allocate memory from the paged or non-paged pools.
 *  Parameters:
 *      c_bytes: Number of bytes to allocate.
 *      type:   Type of memory to allocate; one of:
 *              MEM_PAGED: Allocate from pageable memory.
 *              MEM_NONPAGED: Allocate from page locked memory.
 *  Returns:
 *      Pointer to a block of memory;
 *      NULL if memory couldn't be allocated, if c_bytes == 0, or if type is
 *      not one of MEM_PAGED or MEM_NONPAGED.
 *  Requires:
 *      MEM initialized.
 *  Ensures:
 *      The returned pointer, if not NULL, points to a valid memory block of
 *      the size requested.
 */
	extern void *mem_alloc(u32 c_bytes, enum mem_pool_attrs type);

/*  ======== mem_alloc_phymem ========
 *  Purpose:
 *      Allocate physically contiguous, uncached memory
 *  Parameters:
 *      c_bytes:     Number of bytes to allocate.
 *      u_allign:    Alignment Mask.
 *      p_physical_addr: Physical address of allocated memory.
 *  Returns:
 *      Pointer to a block of memory;
 *      NULL if memory couldn't be allocated, or if c_bytes == 0.
 *  Requires:
 *      MEM initialized.
 *  Ensures:
 *      The returned pointer, if not NULL, points to a valid memory block of
 *      the size requested.  Returned physical address refers to physical
 *      location of memory.
 */
	extern void *mem_alloc_phymem(u32 c_bytes,
				      u32 u_allign,
				      u32 *p_physical_addr);

/*
 *  ======== mem_calloc ========
 *  Purpose:
 *      Allocate zero-initialized memory from the paged or non-paged pools.
 *  Parameters:
 *      c_bytes: Number of bytes to allocate.
 *      type:   Type of memory to allocate; one of:
 *              MEM_PAGED:   Allocate from pageable memory.
 *              MEM_NONPAGED: Allocate from page locked memory.
 *  Returns:
 *      Pointer to a block of zeroed memory;
 *      NULL if memory couldn't be allocated, if c_bytes == 0, or if type is
 *      not one of MEM_PAGED or MEM_NONPAGED.
 *  Requires:
 *      MEM initialized.
 *  Ensures:
 *      The returned pointer, if not NULL, points to a valid memory block
 *      of the size requested.
 */
	extern void *mem_calloc(u32 c_bytes, enum mem_pool_attrs type);


/*
 *  ======== mem_flush_cache ========
 *  Purpose:
 *      Performs system cache sync with discard
 *  Parameters:
 *      p_memBuf:    Pointer to memory region to be flushed.
 *      p_memBuf:    Size of the memory region to be flushed.
 *  Returns:
 *  Requires:
 *      MEM is initialized.
 *  Ensures:
 *      Cache is synchronized
 */
	extern void mem_flush_cache(void *p_memBuf,
					u32 c_bytes,
					s32 flush_type);

/*
 *  ======== mem_free ========
 *  Purpose:
 *      Free the given block of system memory.
 *  Parameters:
 *      p_memBuf:    Pointer to memory allocated by mem_calloc/Alloc().
 *  Returns:
 *  Requires:
 *      MEM initialized.
 *      p_memBuf is a valid memory address returned by mem_calloc/Alloc().
 *  Ensures:
 *      p_memBuf is no longer a valid pointer to memory.
 */
	extern void mem_free(void *p_memBuf);

/*
 *  ======== mem_free_phymem ========
 *  Purpose:
 *      Free the given block of physically contiguous memory.
 *  Parameters:
 *      pVirtualAddress:  Pointer to virtual memory region allocated
 *      by mem_alloc_phymem().
 *      p_physical_addr:  Pointer to physical memory region  allocated
 *      by mem_alloc_phymem().
 *      c_bytes:  Size of the memory region allocated by mem_alloc_phymem().
 *  Returns:
 *  Requires:
 *      MEM initialized.
 *      pVirtualAddress is a valid memory address returned by
 *          mem_alloc_phymem()
 *  Ensures:
 *      pVirtualAddress is no longer a valid pointer to memory.
 */
	extern void mem_free_phymem(void *pVirtualAddress,
				    u32 p_physical_addr, u32 c_bytes);


/*
 *  ======== mem_ext_phypool_init ========
 *  Purpose:
 *      Uses the physical memory chunk passed for internal consitent memory
 *      allocations.
 *      physical address based on the page frame address.
 *  Parameters:
 *      pool_phys_base  starting address of the physical memory pool.
 *      poolSize      size of the physical memory pool.
 *  Returns:
 *      none.
 *  Requires:
 *      - MEM initialized.
 *      - valid physical address for the base and size > 0
 */
	extern void mem_ext_phypool_init(u32 pool_phys_base,
						u32 poolSize);

#endif				/* MEM_ */
