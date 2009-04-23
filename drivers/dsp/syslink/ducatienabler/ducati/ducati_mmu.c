
/*
 * ducati_mmu
 *
 * Syslink support functions for TI OMAP processors.
 *
 * Copyright (C) 2009 Texas Instruments, Inc.
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
#include <hw_mmu.h>
#include <hw_defs.h>
#include <mem.h>
#include <errbase.h>
#include <global_var.h>


#ifdef DEBUG_DUCATI_IPC
#  define DPRINTK(fmt, args...) printk(KERN_INFO "%s: " fmt, __func__ , \
								## args)
#else
#  define DPRINTK(fmt, args...)
#endif

#define     MMU_CNTL_OFFSET                    0x44
#define     MMU_DISABLE_MASK                0xFFFFFFFD
#define     MMU_TWLDISABLE_MASK                0xFFFFFFFA
#define     DUCATI_BASEADDRESS                0x81100000
#define     EXTMEM_CORE0_BASEADDR_OFFSET    0x1000
#define     EXTMEM_CORE0_LENGTH                0x7D000
#define     EXTMEM_CORE1_BASEADDR_OFFSET    0x7E000
#define     EXTMEM_CORE1_LENGTH                0x7E000
#define     INTERM3SHM_BASEADDR_OFFSET        0xFC000
#define     INTERM3SHM_LENGTH                0x4000

/* TESLA_DEBUG  flag is only for initial debugging and should be removed once
     this code is stabilized on Ducati
 */
#ifndef TESLA_DEBUG
#define      base_ducati_l2_mmuPhys          0x55082000
#else
#define      base_ducati_l2_mmuPhys     0x5D000000
#endif


#define PG_MASK(pg_size) (~((pg_size)-1))
#define PG_ALIGN_LOW(addr, pg_size) ((addr) & PG_MASK(pg_size))
#define PG_ALIGN_HIGH(addr, pg_size) (((addr)+(pg_size)-1) & PG_MASK(pg_size))

/* Attributes used to manage the DSP MMU page tables */
struct pg_table_attrs {
	struct sync_cs_object *hcs_object;/* Critical section object handle */

	u32 l1_base_pa;		/* Physical address of the L1 PT */
	u32 l1_base_va;		/* Virtual  address of the L1 PT */
	u32 l1_size;		/* Size of the L1 PT */
	u32 l1_tbl_alloc_pa;
	/* Physical address of Allocated mem for L1 table. May not be aligned */
	u32 l1_tbl_alloc_va;
	/* Virtual address of Allocated mem for L1 table. May not be aligned */
	u32 l1_tbl_alloc_sz;
	/* Size of consistent memory allocated for L1 table.
	 * May not be aligned */

	u32 l2_base_pa;		/* Physical address of the L2 PT */
	u32 l2_base_va;		/* Virtual  address of the L2 PT */
	u32 l2_size;		/* Size of the L2 PT */
	u32 l2_tbl_alloc_pa;
	/* Physical address of Allocated mem for L2 table. May not be aligned */
	u32 l2_tbl_alloc_va;
	/* Virtual address of Allocated mem for L2 table. May not be aligned */
	u32 ls_tbl_alloc_sz;
	/* Size of consistent memory allocated for L2 table.
	 * May not be aligned */

	u32 l2_num_pages;	/* Number of allocated L2 PT */
	struct page_info *pg_info;

} ;

enum pagetype {

	SECTION    = 0,
	LARGE_PAGE = 1,
	SMALL_PAGE = 2,
	TINY_PAGE  = 3
};

static struct pg_table_attrs *p_pt_attrs;
static u32 idw_next_mmu_index;
static u32 base_ducati_l2_mmu;

u32 initpage_attributes(u32 l1_size, u32 l1_allign, u32 ls_num_of_pages)
{
	u32   pg_tbl_pa;
	u32   pg_tbl_va;
	u32   align_size;

#ifndef TESLA_DEBUG
	base_ducati_l2_mmu = (u32)ioremap(base_ducati_l2_mmuPhys, 0x4000);
#else
	/* TODO: HK FOR DEBUGGING PRUPOSE*/
	base_ducati_l2_mmu = (u32)ioremap(base_ducati_l2_mmuPhys, 0x1000);
#endif
	p_pt_attrs = (struct pg_table_attrs *)
	mem_calloc(sizeof(struct pg_table_attrs), MEM_NONPAGED);

	if (p_pt_attrs != NULL) {
		p_pt_attrs->l1_size = l1_size;
		align_size = p_pt_attrs->l1_size;
		/* Align sizes are expected to be power of 2 */
		/* we like to get aligned on L1 table size */
		pg_tbl_va = (u32)mem_alloc_phymem(p_pt_attrs->l1_size,
		    align_size, &pg_tbl_pa);
		/* Check if the PA is aligned for us */
		if ((pg_tbl_pa) & (align_size-1)) {
			/* PA not aligned to page table size ,*/
			/* try with more allocation and align */
					mem_free_phymem(
						(void *)pg_tbl_va
						, pg_tbl_pa,
						p_pt_attrs->l1_size);
			/* we like to get aligned on L1 table size */
			pg_tbl_va = (u32)
				mem_alloc_phymem((p_pt_attrs->l1_size)*2,
				 align_size, &pg_tbl_pa);
			/* We should be able to get aligned table now */
			p_pt_attrs->l1_tbl_alloc_pa = pg_tbl_pa;
			p_pt_attrs->l1_tbl_alloc_va = pg_tbl_va;
			p_pt_attrs->l1_tbl_alloc_sz = p_pt_attrs->l1_size * 2;
			/* Align the PA to the next 'align'  boundary */
			p_pt_attrs->l1_base_pa = ((pg_tbl_pa) +
					(align_size-1)) &
					 (~(align_size-1));

			p_pt_attrs->l1_base_va = pg_tbl_va +
					(p_pt_attrs->l1_base_pa -
					 pg_tbl_pa);
		} else {
			    /* We got aligned PA, cool */
			    p_pt_attrs->l1_tbl_alloc_pa = pg_tbl_pa;
			    p_pt_attrs->l1_tbl_alloc_va = pg_tbl_va;
			    p_pt_attrs->l1_tbl_alloc_sz = p_pt_attrs->l1_size;
			    p_pt_attrs->l1_base_pa = pg_tbl_pa;
			    p_pt_attrs->l1_base_va = pg_tbl_va;
		}

		if (p_pt_attrs->l1_base_va) {
			memset((u8 *)p_pt_attrs->l1_base_va, 0x00,
			p_pt_attrs->l1_size);
		}
		p_pt_attrs->l2_num_pages = ls_num_of_pages;
		p_pt_attrs->l2_size = HW_MMU_COARSE_PAGE_SIZE *
				   p_pt_attrs->l2_num_pages;
		align_size = 4; /* Make it u32 aligned  */
		/* we like to get aligned on L1 table size */
		pg_tbl_va = (u32)mem_alloc_phymem(p_pt_attrs->l2_size,
			    align_size, &pg_tbl_pa);
		p_pt_attrs->l2_tbl_alloc_pa = pg_tbl_pa;
		p_pt_attrs->l2_tbl_alloc_va = pg_tbl_va;
		p_pt_attrs->ls_tbl_alloc_sz = p_pt_attrs->l2_size;
		p_pt_attrs->l2_base_pa = pg_tbl_pa;
		p_pt_attrs->l2_base_va = pg_tbl_va;
		if (p_pt_attrs->l2_base_va) {
				memset((u8 *)p_pt_attrs->l2_base_va, 0x00,
				p_pt_attrs->l2_size);
		}
		p_pt_attrs->pg_info = mem_calloc(p_pt_attrs->l2_num_pages *
				sizeof(struct page_info), MEM_NONPAGED);
		DPRINTK("L1 pa %x, va %x, size %x\n L2 pa %x, va "
			 "%x, size %x\n", p_pt_attrs->l1_base_pa,
			 p_pt_attrs->l1_base_va, p_pt_attrs->l1_size,
			 p_pt_attrs->l2_base_pa, p_pt_attrs->l2_base_va,
			 p_pt_attrs->l2_size);
		DPRINTK("p_pt_attrs %x L2 NumPages %x pg_info %x\n",
			 (u32)p_pt_attrs, p_pt_attrs->l2_num_pages,
			(u32)p_pt_attrs->pg_info);
	}
	return 0;
}


/*  ........................................................................
  func    print_ptes

  desc    Print the DSP MMU Table Entries

  modif   None
  ........................................................................
*/
void  print_ptes(bool ashow_inv_entries, bool ashow_repeat_entries)
{
	u32               pte_val;
	u32               pte_size;
	u32               last_sect = 0;
	u32               this_sect = 0;
	u32               cur_l1_entry;
	u32               cur_l2_entry;
	u32               pg_tbl_va;
	u32               l1_base_va;
	u32               l2_base_va    = 0;
	u32               l2_base_pa    = 0;

	l1_base_va = p_pt_attrs->l1_base_va;
	pg_tbl_va = l1_base_va;

	DPRINTK("\n*** Currently programmed PTEs : Max possible L1 Entries"
			"[%d] ***\n", (p_pt_attrs->l1_size / sizeof(u32)));

	/*  Walk all L1 entries, dump out info.  Dive into L2 if necessary  */
	for (cur_l1_entry = 0;
	cur_l1_entry < (p_pt_attrs->l1_size / sizeof(u32));
					cur_l1_entry++) {

		/*pte_val = pL1PgTbl[cur_l1_entry];*/
		pte_val = *((u32 *)(pg_tbl_va + (cur_l1_entry * sizeof(u32))));
		pte_size = hw_mmu_pte_sizel1(pte_val);

		if (pte_size == HW_PAGE_SIZE_16MB) {
			this_sect = hw_mmu_pte_phyaddr(pte_val, pte_size);
			if (this_sect != last_sect) {

				last_sect = this_sect;

				DPRINTK("PTE L1 [16 MB] -> VA =  \
						0x%x PA = 0x%x\n",
						cur_l1_entry << 24, this_sect);

			} else if (ashow_repeat_entries != FALSE)
				DPRINTK("    {REPEAT}\n");
		} else if (pte_size == HW_PAGE_SIZE_1MB) {
			this_sect = hw_mmu_pte_phyaddr(pte_val, pte_size);
			if (this_sect != last_sect) {

				last_sect = this_sect;

				DPRINTK("PTE L1 [1 MB ] -> VA =  \
						0x%x PA = 0x%x\n",
						cur_l1_entry << 20, this_sect);

			} else if (ashow_repeat_entries != FALSE)
				DPRINTK("    {REPEAT}\n");

		} else if (pte_size == HW_MMU_COARSE_PAGE_SIZE) {
			/*  Get the L2 data for this  */
			DPRINTK("PTE L1 [L2   ] -> VA = \
					0x%x\n", cur_l1_entry << 20);

/* Get the L2 PA from the L1 PTE, and find corresponding L2 VA*/
			l2_base_pa    = hw_mmu_pte_coarsel1(pte_val) ;
			l2_base_va    = l2_base_pa - p_pt_attrs->l2_base_pa +
						p_pt_attrs->l2_base_va ;

			for (cur_l2_entry = 0;
			 cur_l2_entry < (HW_MMU_COARSE_PAGE_SIZE / sizeof(u32));
			 cur_l2_entry++) {

				pte_val = *((u32 *)(l2_base_va +
						(cur_l2_entry * sizeof(u32))));

				pte_size = hw_mmu_pte_sizel2(pte_val);

				if ((pte_size == HW_PAGE_SIZE_64KB) ||
				    (pte_size == HW_PAGE_SIZE_4KB)) {

					this_sect = hw_mmu_pte_phyaddr
						(pte_val, pte_size);

					if (this_sect != last_sect) {
						last_sect = this_sect;
						DPRINTK("PTE L2 [%s KB] ->"
						"VA = 0x%x   PA = 0x%x\n",
						(pte_size ==
						HW_PAGE_SIZE_64KB) ?
						"64" : "4",
						((cur_l1_entry << 20)
						| (cur_l2_entry << 12)),
						this_sect);

					} else if (ashow_repeat_entries
								!= FALSE)

						DPRINTK("        {REPEAT}");
				} else  if (ashow_inv_entries != FALSE) {


					DPRINTK("PTE L2 [INVALID] -> VA = \
						0x%x",
						((cur_l1_entry << 20) |
						(cur_l2_entry << 12)));
					continue;
				}
			 }
		} else if (ashow_inv_entries != FALSE) {
			/*  Entry is invalid (not set), skip it  */
			DPRINTK("PTE L1 [INVALID] -> VA = 0x%x",
						cur_l1_entry << 20);
			continue;
		}
	}
	/*  Dump the TLB entries as well  */
	DPRINTK("\n*** Currently programmed TLBs ***\n");
	hw_mmi_tlb_dump(base_ducati_l2_mmu, FALSE);
	DPRINTK("*** DSP MMU DUMP COMPLETED ***\n");
}



/*
 *  ======== pte_set ========
 *      This function calculates PTE address (MPU virtual) to be updated
 *      It also manages the L2 page tables
 */
static DSP_STATUS pte_set(u32 pa, u32 va,
			 u32 size, struct hw_mmu_map_attrs_t *attrs)
{
	u32 i;
	u32 pte_val;
	u32 pte_addr_l1;
	u32 pte_size;
	u32 pg_tbl_va;      /* Base address of the PT that will be updated */
	u32 l1_base_va;
	 /* Compiler warns that the next three variables might be used
	 * uninitialized in this function. Doesn't seem so. Working around,
	 * anyways.  */
	u32 l2_base_va = 0;
	u32 l2_base_pa = 0;
	u32 l2_page_num = 0;
	struct pg_table_attrs *pt = p_pt_attrs;
	DSP_STATUS status = DSP_SOK;
	DPRINTK("> pte_set ppg_table_attrs %x, pa %x, va %x, "
		 "size %x, attrs %x\n", (u32)pt, pa, va, size, (u32)attrs);
	l1_base_va = pt->l1_base_va;
	pg_tbl_va = l1_base_va;
	if ((size == HW_PAGE_SIZE_64KB) || (size == HW_PAGE_SIZE_4KB)) {
		/* Find whether the L1 PTE points to a valid L2 PT */
		pte_addr_l1 = hw_mmu_pte_addr_l1(l1_base_va, va);
		if (pte_addr_l1 <= (pt->l1_base_va + pt->l1_size)) {
			pte_val = *(u32 *)pte_addr_l1;
			pte_size = hw_mmu_pte_sizel1(pte_val);
		} else {
			return DSP_EFAIL;
		}
		/* TODO: ADD synchronication element*/
		/*		sync_enter_cs(pt->hcs_object);*/
		if (pte_size == HW_MMU_COARSE_PAGE_SIZE) {
			/* Get the L2 PA from the L1 PTE, and find
			 * corresponding L2 VA */
			l2_base_pa = hw_mmu_pte_coarsel1(pte_val);
			l2_base_va = l2_base_pa - pt->l2_base_pa +
			pt->l2_base_va;
			l2_page_num = (l2_base_pa - pt->l2_base_pa) /
				    HW_MMU_COARSE_PAGE_SIZE;
		} else if (pte_size == 0) {
			/* L1 PTE is invalid. Allocate a L2 PT and
			 * point the L1 PTE to it */
			/* Find a free L2 PT. */
			for (i = 0; (i < pt->l2_num_pages) &&
			    (pt->pg_info[i].num_entries != 0); i++)
				;;
			if (i < pt->l2_num_pages) {
				l2_page_num = i;
				l2_base_pa = pt->l2_base_pa + (l2_page_num *
					   HW_MMU_COARSE_PAGE_SIZE);
				l2_base_va = pt->l2_base_va + (l2_page_num *
					   HW_MMU_COARSE_PAGE_SIZE);
				/* Endianness attributes are ignored for
				 * HW_MMU_COARSE_PAGE_SIZE */
				status =
				hw_mmu_pte_set(l1_base_va, l2_base_pa, va,
					 HW_MMU_COARSE_PAGE_SIZE, attrs);
			} else {
				status = DSP_EMEMORY;
			}
		} else {
			/* Found valid L1 PTE of another size.
			 * Should not overwrite it. */
			status = DSP_EFAIL;
		}
		if (DSP_SUCCEEDED(status)) {
			pg_tbl_va = l2_base_va;
			if (size == HW_PAGE_SIZE_64KB)
				pt->pg_info[l2_page_num].num_entries += 16;
			else
				pt->pg_info[l2_page_num].num_entries++;

			DPRINTK("L2 BaseVa %x, BasePa %x, "
				 "PageNum %x num_entries %x\n", l2_base_va,
				 l2_base_pa, l2_page_num,
				 pt->pg_info[l2_page_num].num_entries);
		}
/*		sync_leave_cs(pt->hcs_object);*/
	}
	if (DSP_SUCCEEDED(status)) {
		DPRINTK("PTE pg_tbl_va %x, pa %x, va %x, size %x\n",
			 pg_tbl_va, pa, va, size);
		DPRINTK("PTE endianism %x, element_size %x, "
			  "mixedSize %x\n", attrs->endianism,
			  attrs->element_size, attrs->mixedSize);
		status = hw_mmu_pte_set(pg_tbl_va, pa, va, size, attrs);
	}
	DPRINTK("< pte_set status %x\n", status);
	return status;
}


/*
 *  ======== pte_update ========
 *      This function calculates the optimum page-aligned addresses and sizes
 *      Caller must pass page-aligned values
 */
static DSP_STATUS pte_update(u32 pa, u32 va, u32 size,
			    struct hw_mmu_map_attrs_t *map_attrs)
{
	u32 i;
	u32 all_bits;
	u32 pa_curr = pa;
	u32 va_curr = va;
	u32 num_bytes = size;
	DSP_STATUS status = DSP_SOK;
	u32 pg_size[] = { HW_PAGE_SIZE_16MB, HW_PAGE_SIZE_1MB,
			   HW_PAGE_SIZE_64KB, HW_PAGE_SIZE_4KB };
	DPRINTK("> pte_update  pa %x, va %x, "
		 "size %x, map_attrs %x\n", pa, va, size, (u32)map_attrs);
	while (num_bytes && DSP_SUCCEEDED(status)) {
		/* To find the max. page size with which both PA & VA are
		 * aligned */
		all_bits = pa_curr | va_curr;
		DPRINTK("all_bits %x, pa_curr %x, va_curr %x, "
			 "num_bytes %x\n ",
			all_bits, pa_curr, va_curr, num_bytes);

		for (i = 0; i < 4; i++) {
			if ((num_bytes >= pg_size[i]) && ((all_bits &
			   (pg_size[i] - 1)) == 0)) {
				DPRINTK("pg_size %x\n", pg_size[i]);
				status = pte_set(pa_curr,
					va_curr, pg_size[i], map_attrs);
				pa_curr += pg_size[i];
				va_curr += pg_size[i];
				num_bytes -= pg_size[i];
				 /* Don't try smaller sizes. Hopefully we have
				 * reached an address aligned to a bigger page
				 * size */
				break;
			}
		}
	}
	DPRINTK("< pte_update status %x num_bytes %x\n", status, num_bytes);
	return status;
}


/*
 *  ======== WMD_BRD_MemMap ========
 *      This function maps MPU buffer to the DSP address space. It performs
 *  linear to physical address translation if required. It translates each
 *  page since linear addresses can be physically non-contiguous
 *  All address & size arguments are assumed to be page aligned (in proc.c)
 *
 */
static DSP_STATUS ducati_mem_map(u32 ul_mpu_addr, u32 ul_virt_addr,
				 u32 ul_num_bytes, u32 ul_map_attr)
{
	u32 attrs;
	DSP_STATUS status = DSP_SOK;
	struct hw_mmu_map_attrs_t hw_attrs;

	DPRINTK("> WMD_BRD_MemMap  pa %x, va %x, "
		 "size %x, ul_map_attr %x\n", ul_mpu_addr, ul_virt_addr,
		 ul_num_bytes, ul_map_attr);
	if (ul_num_bytes == 0)
		return DSP_EINVALIDARG;

	if (ul_map_attr != 0) {
		attrs = ul_map_attr;
	} else {
		/* Assign default attributes */
		attrs = DSP_MAPVIRTUALADDR | DSP_MAPELEMSIZE16;
	}
	/* Take mapping properties */
	if (attrs & DSP_MAPBIGENDIAN)
		hw_attrs.endianism = HW_BIG_ENDIAN;
	else
		hw_attrs.endianism = HW_LITTLE_ENDIAN;

	hw_attrs.mixedSize = (enum hw_mmu_mixed_size_t)
			     ((attrs & DSP_MAPMIXEDELEMSIZE) >> 2);
	/* Ignore element_size if mixedSize is enabled */
	if (hw_attrs.mixedSize == 0) {
		if (attrs & DSP_MAPELEMSIZE8) {
			/* Size is 8 bit */
			hw_attrs.element_size = HW_ELEM_SIZE_8BIT;
		} else if (attrs & DSP_MAPELEMSIZE16) {
			/* Size is 16 bit */
			hw_attrs.element_size = HW_ELEM_SIZE_16BIT;
		} else if (attrs & DSP_MAPELEMSIZE32) {
			/* Size is 32 bit */
			hw_attrs.element_size = HW_ELEM_SIZE_32BIT;
		} else if (attrs & DSP_MAPELEMSIZE64) {
			/* Size is 64 bit */
			hw_attrs.element_size = HW_ELEM_SIZE_64BIT;
		} else {
			/* Mixedsize isn't enabled, so size can't be
			 * zero here */
			DPRINTK("WMD_BRD_MemMap: MMU element size is zero\n");
			return DSP_EINVALIDARG;
		}
	}
	status = pte_update(ul_mpu_addr, ul_virt_addr,
			 ul_num_bytes, &hw_attrs);

	 /* In any case, flush the TLB
	 * This is called from here instead from pte_update to avoid unnecessary
	 * repetition while mapping non-contiguous physical regions of a virtual
	 * region */
	hw_mmu_tlb_flushAll(base_ducati_l2_mmu);
	DPRINTK("< WMD_BRD_MemMap status %x\n", status);
	return status;
}

/*  ............................................................................
*  func    get_mmu_entry_size
*
*  desc    Decides a TLB entry size
*
*  modif   none
*............................................................................
*/
static DSP_STATUS
get_mmu_entry_size(u32  dw_phy_addr,
				u32  dw_size,
				enum pagetype *size_tlb,
				u32  *entry_size)
{
	DSP_STATUS  status        = DSP_SOK ;
	bool        page_align_4kb  = false;
	bool        page_align_64kb = false ;
	bool        page_align_1mb  = false ;

	/*  First check the page alignment*/
	if ((dw_phy_addr % PAGE_SIZE_4KB)  == 0)
		page_align_4kb  = true ;
	if ((dw_phy_addr % PAGE_SIZE_64KB) == 0)
		page_align_64kb = true ;
	if ((dw_phy_addr % PAGE_SIZE_1MB)  == 0)
		page_align_1mb  = true ;

	if ((!page_align_64kb) && (!page_align_1mb)  && (!page_align_4kb))
		status = DSP_EFAIL;;

	if (DSP_SUCCEEDED(status)) {

		/*  Now decide the entry size */
		if (dw_size >= PAGE_SIZE_1MB) {

			if (page_align_1mb) {

				*size_tlb   = SECTION ;
				*entry_size = PAGE_SIZE_1MB ;
			} else if (page_align_64kb) {

				*size_tlb   = LARGE_PAGE ;
				*entry_size = PAGE_SIZE_64KB ;
			} else if (page_align_4kb) {

				*size_tlb   = SMALL_PAGE ;
				*entry_size = PAGE_SIZE_4KB ;
			} else
				status = DSP_EFAIL;;
		} else if (dw_size > PAGE_SIZE_4KB &&
				dw_size < PAGE_SIZE_1MB) {

			if (page_align_64kb) {

				*size_tlb   = LARGE_PAGE ;
				*entry_size = PAGE_SIZE_64KB ;
			} else if (page_align_4kb) {

				*size_tlb   = SMALL_PAGE ;
				*entry_size = PAGE_SIZE_4KB ;
			} else
				status = DSP_EFAIL;
		} else if (dw_size == PAGE_SIZE_4KB) {

				if (page_align_4kb) {

					*size_tlb   = SMALL_PAGE ;
					*entry_size = PAGE_SIZE_4KB ;
				} else
					status = DSP_EFAIL;
			} else
			    status = DSP_EFAIL;
	}

    return status ;
}


/*  ............................................................................
  func    add_dsp_mmu_entry

  desc    Add DSP MMU entries corresponding to given MPU-Physical address
				and DSP-virtual address

  modif   none
  ............................................................................
*/
static DSP_STATUS add_dsp_mmu_entry(u32  *dw_phy_addr, u32  *dw_dsp_addr,
						u32  dw_entry_size)
{
	u32       mapped_size = 0 ;
	enum pagetype      size_tlb    = SECTION ;
	u32       entry_size  = 0 ;
	DSP_STATUS  status     = 0;/*DSP_SOK ;*/
	u32 page_size   = HW_PAGE_SIZE_1MB ;

	struct hw_mmu_map_attrs_t  map_attrs = { HW_LITTLE_ENDIAN,
						HW_ELEM_SIZE_16BIT,
						HW_MMU_CPUES };

	DPRINTK("Entered add_dsp_mmu_entry dw_phy_addr = \
						0x%x, dw_dsp_addr = 0x%x, "
						"dw_entry_size = 0x%x\n",
		*dw_phy_addr, *dw_dsp_addr, dw_entry_size);


	while ((mapped_size < dw_entry_size) && (DSP_SUCCEEDED(status))) {



		status = get_mmu_entry_size(*dw_phy_addr,
				(dw_entry_size - mapped_size),
				&size_tlb,
				&entry_size);

		if (size_tlb == SECTION)
			page_size = HW_PAGE_SIZE_1MB ;

		else if (size_tlb == LARGE_PAGE)
			page_size = HW_PAGE_SIZE_64KB ;

		else if (size_tlb == SMALL_PAGE)
			page_size = HW_PAGE_SIZE_4KB ;

		if (DSP_SUCCEEDED(status)) {

			hw_mmu_tlb_add((base_ducati_l2_mmu),
						*dw_phy_addr,
						*dw_dsp_addr,
						page_size,
						idw_next_mmu_index++,
						&map_attrs,
						HW_SET, /* preserved*/
						HW_SET); /* valid*/

			mapped_size  += entry_size ;
			*dw_phy_addr   += entry_size ;
			*dw_dsp_addr   += entry_size ;

			if (idw_next_mmu_index > 32)
				status = DSP_EFAIL;
/*set  a valid value here;DSP_EFAIL ;*/
		}
	}
	DPRINTK("Exited add_dsp_mmu_entrydw_phy_addr = \
		0x%x, dw_dsp_addr = 0x%x\n",
		*dw_phy_addr, *dw_dsp_addr);
	return status ;
 }


/*  ............................................................................
*  func    add_dsp_mmu_entry
*
*  desc    Add DSP MMU entries corresponding to given MPU-Physical address
*          and DSP-virtual address
*
*  modif   none
*  ............................................................................
*/
static DSP_STATUS add_entry_ext(u32  *dw_phy_addr, u32  *dw_dsp_addr,
					u32  dw_entry_size)
{
	u32       mapped_size = 0 ;
	enum pagetype     size_tlb    = SECTION ;
	u32       entry_size  = 0 ;
	DSP_STATUS  status     = 0;
	u32       page_size   = HW_PAGE_SIZE_1MB ;
	u32      flags = DSP_MAPELEMSIZE32;

	while ((mapped_size < dw_entry_size) && (DSP_SUCCEEDED(status))) {

		/*  get_mmu_entry_size fills the size_tlb and entry_size
		based on alignment and size of memory to map
		to DSP - dw_entry_size */
		status = get_mmu_entry_size(*dw_phy_addr,
				(dw_entry_size - mapped_size),
				&size_tlb,
				&entry_size);

		if (size_tlb == SECTION)
			page_size = HW_PAGE_SIZE_1MB ;
		else if (size_tlb == LARGE_PAGE)
			page_size = HW_PAGE_SIZE_64KB ;
		else if (size_tlb == SMALL_PAGE)
			page_size = HW_PAGE_SIZE_4KB ;

		if (DSP_SUCCEEDED(status)) {

			ducati_mem_map(*dw_phy_addr,
			*dw_dsp_addr, page_size, flags);
			mapped_size  += entry_size ;
			*dw_phy_addr   += entry_size ;
			*dw_dsp_addr   += entry_size ;
		}
	}
	return status ;
 }



/*  ========================================================================
*  func    ducati_mmu_init
*
* desc    Initialize the Ducati MMU.
*  =========================================================================
*/
u32 ducati_mmu_init(u32 a_phy_addr, int a_size)
{
	u32 ret_val = 0;
	u32 ducati_mmu_linear_addr = base_ducati_l2_mmu;
	u32 reg_value = 0;
	DSP_STATUS status = 0 ;
	u32 dw_phy_addr = 0 ;
	u32 num_l4_entries;
	u32 dw_dsp_total_size;
	u32 dw_ext_mem_mapbeg      = 0 ;
	u32 dw_interm3_shmend      = 0 ;
	u32 i = 0;
	u32 map_attrs;
	u32 dw_ducati_boot_addr = 0;

	num_l4_entries = (sizeof(l4_map) / sizeof(struct mmu_entry));


	DPRINTK("\n  Programming Ducati MMU using linear address [0x%x]",
						ducati_mmu_linear_addr);

	/*  Disable the MMU & TWL */
	hw_mmu_disable(base_ducati_l2_mmu);
	hw_mmu_twl_disable(base_ducati_l2_mmu);

	idw_next_mmu_index = 0 ;

	dw_interm3_shmend = DUCATI_BASEADDRESS +
		INTERM3SHM_BASEADDR_OFFSET + INTERM3SHM_LENGTH - 0x1;

	/*   Calculate Total Size of Memory */
	dw_dsp_total_size = dw_interm3_shmend + 0x1 - DUCATI_BASEADDRESS;
	dw_ext_mem_mapbeg = DUCATI_BASEADDRESS + PAGE_SIZE_4KB;
	dw_dsp_total_size = PG_ALIGN_HIGH(dw_dsp_total_size, PAGE_SIZE_4KB);

	dw_dsp_total_size -= PAGE_SIZE_4KB;

	dw_phy_addr = a_phy_addr;
	DPRINTK("Value before calling add_dsp_mmu_entry dw_phy_addr = 0x%x,"
		"dw_ducati_boot_addr = 0x%x\n",
		dw_phy_addr, dw_ducati_boot_addr);
		add_dsp_mmu_entry(&dw_phy_addr,
		&dw_ducati_boot_addr, PAGE_SIZE_4KB);


	DPRINTK("Value after calling add_dsp_mmu_entry dw_phy_addr = 0x%x,"
		"dw_ducati_boot_addr = 0x%x\n",
		dw_phy_addr, dw_ducati_boot_addr);

	/* Lock the base counter*/
	hw_mmu_numlocked_set(ducati_mmu_linear_addr,
						idw_next_mmu_index);

	hw_mmu_victim_numset(ducati_mmu_linear_addr,
						idw_next_mmu_index);


	map_attrs = 0x00000000;
	map_attrs |= DSP_MAPLITTLEENDIAN;
	map_attrs |= DSP_MAPPHYSICALADDR;
	map_attrs |= DSP_MAPELEMSIZE32;

	for (i = 0; i < num_l4_entries; i++) {


		ducati_mem_map(l4_map[i].ul_phy_addr,  l4_map[i].ul_virt_addr,
						l4_map[i].ulSize, map_attrs);

		if (DSP_FAILED(status)) {

			DPRINTK("**** Failed to map Peripheral ****");
			DPRINTK("Phys addr [0x%x] Virt addr [0x%x] size [0x%x]",
			    l4_map[i].ul_phy_addr, l4_map[i].ul_virt_addr,
			    l4_map[i].ulSize);
			DPRINTK(" Status [0x%x]", status);
			break ;
		}
	}

	DPRINTK(" >> add_entry_ext2 entry [0x%x]  [0x%x] [0x%x]\n",
		dw_phy_addr, dw_ext_mem_mapbeg, dw_dsp_total_size);
	status = add_entry_ext(&dw_phy_addr,
		&dw_ext_mem_mapbeg, dw_dsp_total_size);

	/*    Set the TTB to point to the L1 page table's physical address */
	hw_mmu_ttbset(ducati_mmu_linear_addr, p_pt_attrs->l1_base_pa);

	/*    Enable the TWL */
	hw_mmu_twl_enable(ducati_mmu_linear_addr);
	/*hw_mmu_autoidle_en(ducati_mmu_linear_addr);*/

	hw_mmu_enable(ducati_mmu_linear_addr);

	/*  MMU Debug Statements */
	reg_value = *((REG u32 *)(ducati_mmu_linear_addr + 0x40));
	DPRINTK("  Ducati TWL Status [0x%x]\n", reg_value);

	reg_value = *((REG u32 *)(ducati_mmu_linear_addr + 0x4C));
	DPRINTK("  Ducati TTB Address [0x%x]\n", reg_value);

	reg_value = *((REG u32 *)(ducati_mmu_linear_addr + 0x44));
	DPRINTK("  Ducati MMU Status [0x%x]\n", reg_value);

	/*  Dump the MMU Entries */
	print_ptes(FALSE, FALSE);

	DPRINTK("  Programmed Ducati BootVectors 0x0 to first page at [0x%x]",
							a_phy_addr);
	DPRINTK("  Programmed Mailbox into Ducati MMU at [0xAA0F4000]\n");

	DPRINTK("  Make sure your BaseImage Code is within [0x%x] & [0x%x]",
			a_phy_addr + 0x1000, a_phy_addr + a_size);

	DPRINTK("  Leaving DDucati_MMUManager::ducati_mmu_init [0x%x]",
								ret_val);

	return ret_val;
}

/*  ========================================================================
*  func    mmu_de_init
*
* desc    De-Initialize the Ducati MMU and free the
* memory allocation for L1 and L2 pages
*  =========================================================================
*/
u32 mmu_de_init(void)
{
	DPRINTK("  Freeing memory allocated in mmu_de_init\n");
	if (p_pt_attrs->l2_tbl_alloc_va) {
		mem_free_phymem((void *)p_pt_attrs->l2_tbl_alloc_va,
		p_pt_attrs->l2_tbl_alloc_pa, p_pt_attrs->ls_tbl_alloc_sz);
	}
	if (p_pt_attrs->l1_tbl_alloc_va) {
		mem_free_phymem((void *)p_pt_attrs->l1_tbl_alloc_va,
		p_pt_attrs->l1_tbl_alloc_pa, p_pt_attrs->l1_tbl_alloc_sz);
	}
	if (p_pt_attrs)
		mem_free((void *)p_pt_attrs);
	return 0;
}







