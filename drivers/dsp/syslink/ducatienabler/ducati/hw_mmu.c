/*
 * hw_mmu.c
 *
 * DSP-BIOS Bridge driver support functions for TI OMAP processors.
 *
 * Copyright (C) 2007 Texas Instruments, Inc.
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
 *  ======== hw_mmu.c ========
 *  Description:
 *      API definitions to setup MMU TLB and PTE
 *
 *! Revision History:
 *! ================
 *! 19-Apr-2004 sb  TLBAdd and TLBFlush input the page size in bytes instead
*of an enum. TLBAdd inputs mapping attributes struct instead
*of individual arguments.
*Removed MMU.h and other cosmetic updates.
 *! 08-Mar-2004 sb  Added the Page Table Management APIs
 *! 16 Feb 2003 sb: Initial version
 */

/* ============================================================================
* STANDARD INCLUDE FILES
* =============================================================================
*/

/* ============================================================================
* PROJECT SPECIFIC INCLUDE FILES
* =============================================================================
*/
#include <GlobalTypes.h>
#include <MMURegAcM.h>
#include <hw_defs.h>
#include <hw_mmu.h>
#include<linux/kernel.h>
#include<linux/module.h>

/* ============================================================================
* GLOBAL VARIABLES DECLARATIONS
* =============================================================================
*/

/* ============================================================================
* LOCAL TYPES AND DEFINITIONS
* =============================================================================
*/
#define MMU_BASE_VAL_MASK        0xFC00
#define MMU_PAGE_MAX             3
#define MMU_ELEMENTSIZE_MAX      3
#define MMU_ADDR_MASK            0xFFFFF000
#define MMU_TTB_MASK             0xFFFFC000
#define MMU_SECTION_ADDR_MASK    0xFFF00000
#define MMU_SSECTION_ADDR_MASK   0xFF000000
#define MMU_PAGE_TABLE_MASK      0xFFFFFC00
#define MMU_LARGE_PAGE_MASK      0xFFFF0000
#define MMU_SMALL_PAGE_MASK      0xFFFFF000

#define MMU_LOAD_TLB        0x00000001
#define NUM_TLB_ENTRIES 32



/* ----------------------------------------------------------------------------
* TYPE:         hw_mmu_pgsiz_t
*
* DESCRIPTION:  Enumerated Type used to specify the MMU Page Size(SLSS)
*
* -----------------------------------------------------------------------------
*/
enum hw_mmu_pgsiz_t {
	HW_MMU_SECTION,
	HW_MMU_LARGE_PAGE,
	HW_MMU_SMALL_PAGE,
	HW_MMU_SUPERSECTION

};

/* ============================================================================
* LOCAL VARIABLES DECLARATIONS
* =============================================================================
*/

/* ============================================================================
* LOCAL FUNCTIONS PROTOTYPES
* =============================================================================
*/
/*
-------------------------------------------------------------------------
* FUNCTION              : mmu_flsh_entry
*
* INPUTS:
*
*       Identifier      : base_address
*       Type            : const u32
*       Description     : Base Address of instance of MMU module
*
* RETURNS:
*
*       Type            : hw_status
*       Description     : RET_OK                 -- No errors occured
*                         RET_BAD_NULL_PARAM     -- A Pointer Paramater was set to NULL
*
* PURPOSE:              : Flush the TLB entry pointed by the lock counter register
*                         even if this entry is set protected
*
* METHOD:               : Check the Input parameter and Flush a single entry in the TLB.
-------------------------------------------------------------------------
*/

static hw_status mmu_flsh_entry(const u32   base_address);

 /*
-------------------------------------------------------------------------
* FUNCTION              : mme_set_cam_entry
*
* INPUTS:
*
*       Identifier      : base_address
*       Type            : const u32
*       Description     : Base Address of instance of MMU module
*
*       Identifier      : page_size
*       Type            : const u32
*       Description     : It indicates the page size
*
*       Identifier      : preserve_bit
*       Type            : const u32
*       Description     : It indicates the TLB entry is preserved entry or not
*
*       Identifier      : valid_bit
*       Type            : const u32
*       Description     : It indicates the TLB entry is valid entry or not
*
*
*       Identifier      : virt_addr_tag
*       Type            : const u32
*       Description     : virtual Address
*
* RETURNS:
*
*       Type            : hw_status
*       Description     : RET_OK                 -- No errors occured
*                         RET_BAD_NULL_PARAM     -- A Pointer Paramater was set to NULL
*                         RET_PARAM_OUT_OF_RANGE -- Input Parameter out of Range
*
* PURPOSE:              : Set MMU_CAM reg
*
* METHOD:               : Check the Input parameters and set the CAM entry.
-------------------------------------------------------------------------
*/

static hw_status mme_set_cam_entry(const u32    base_address,
	const u32    page_size,
	const u32    preserve_bit,
	const u32    valid_bit,
	const u32    virt_addr_tag);

/*
-------------------------------------------------------------------------
* FUNCTION              : mmu_set_ram_entry
*
* INPUTS:
*
*       Identifier      : base_address
*       Type            : const u32
*       Description     : Base Address of instance of MMU module
*
*       Identifier      : physical_addr
*       Type            : const u32
*       Description     : Physical Address to which the corresponding virtual
*                         Address should point
*
*       Identifier      : endianism
*       Type            : hw_endianism_t
*       Description     : endianism for the given page
*
*       Identifier      : element_size
*       Type            : hw_elemnt_siz_t
*       Description     : The element size ( 8,16, 32 or 64 bit)
*
*       Identifier      : mixedSize
*       Type            : hw_mmu_mixed_size_t
*       Description     : Element Size to follow CPU or TLB
*
* RETURNS:
*
*       Type            : hw_status
*       Description     : RET_OK                 -- No errors occured
*                         RET_BAD_NULL_PARAM     -- A Pointer Paramater was set to NULL
*                         RET_PARAM_OUT_OF_RANGE -- Input Parameter out of Range
*
* PURPOSE:              : Set MMU_CAM reg
*
* METHOD:               : Check the Input parameters and set the RAM entry.
-------------------------------------------------------------------------
*/
static hw_status mmu_set_ram_entry(const u32        base_address,
	const u32        physical_addr,
	enum hw_endianism_t      endianism,
	enum hw_elemnt_siz_t    element_size,
	enum hw_mmu_mixed_size_t   mixedSize);



/* ============================================================================
* HW FUNCTIONS
* =============================================================================
*/

hw_status hw_mmu_enable(const u32 base_address)
{
	hw_status status = RET_OK;

	MMUMMU_CNTLMMUEnableWrite32(base_address, HW_SET);

	return status;
}
EXPORT_SYMBOL(hw_mmu_enable);

hw_status hw_mmu_disable(const u32 base_address)
{
	hw_status status = RET_OK;

	MMUMMU_CNTLMMUEnableWrite32(base_address, HW_CLEAR);

	return status;
}
EXPORT_SYMBOL(hw_mmu_disable);

hw_status hw_mmu_autoidle_en(const u32 base_address)
{
	hw_status status;

	status = mmu_sisconf_auto_idle_set32(base_address, HW_SET);
	status = RET_OK;
	return status;
}
EXPORT_SYMBOL(hw_mmu_autoidle_en);

hw_status hw_mmu_nulck_set(const u32 base_address, u32 *num_lcked_entries)
{
	hw_status status = RET_OK;

	*num_lcked_entries = MMUMMU_LOCKBaseValueRead32(base_address);

	return status;
}
EXPORT_SYMBOL(hw_mmu_nulck_set);


hw_status hw_mmu_numlocked_set(const u32 base_address, u32 num_lcked_entries)
{
	hw_status status = RET_OK;

	MMUMMU_LOCKBaseValueWrite32(base_address, num_lcked_entries);

	return status;
}
EXPORT_SYMBOL(hw_mmu_numlocked_set);


hw_status hw_mmu_vctm_numget(const u32 base_address, u32 *vctm_entry_num)
{
	hw_status status = RET_OK;

	*vctm_entry_num = MMUMMU_LOCKCurrentVictimRead32(base_address);

	return status;
}
EXPORT_SYMBOL(hw_mmu_vctm_numget);


hw_status hw_mmu_victim_numset(const u32 base_address, u32 vctm_entry_num)
{
	hw_status status = RET_OK;

	mmu_lck_crnt_vctmwite32(base_address, vctm_entry_num);

	return status;
}
EXPORT_SYMBOL(hw_mmu_victim_numset);

hw_status hw_mmu_tlb_flushAll(const u32 base_address)
{
	hw_status status = RET_OK;

	MMUMMU_GFLUSHGlobalFlushWrite32(base_address, HW_SET);

	return status;
}
EXPORT_SYMBOL(hw_mmu_tlb_flushAll);

hw_status hw_mmu_eventack(const u32 base_address, u32 irq_mask)
{
	hw_status status = RET_OK;

	MMUMMU_IRQSTATUSWriteRegister32(base_address, irq_mask);

	return status;
}
EXPORT_SYMBOL(hw_mmu_eventack);

hw_status hw_mmu_event_disable(const u32 base_address, u32 irq_mask)
{
	hw_status status = RET_OK;
	u32 irqReg;
	irqReg = MMUMMU_IRQENABLEReadRegister32(base_address);

	MMUMMU_IRQENABLEWriteRegister32(base_address, irqReg & ~irq_mask);

	return status;
}
EXPORT_SYMBOL(hw_mmu_event_disable);

hw_status hw_mmu_event_enable(const u32 base_address, u32 irq_mask)
{
	hw_status status = RET_OK;
	u32 irqReg;

	irqReg = MMUMMU_IRQENABLEReadRegister32(base_address);

	MMUMMU_IRQENABLEWriteRegister32(base_address, irqReg | irq_mask);

	return status;
}
EXPORT_SYMBOL(hw_mmu_event_enable);

hw_status hw_mmu_event_status(const u32 base_address, u32 *irq_mask)
{
	hw_status status = RET_OK;

	*irq_mask = MMUMMU_IRQSTATUSReadRegister32(base_address);

	return status;
}
EXPORT_SYMBOL(hw_mmu_event_status);

hw_status hw_mmu_flt_adr_rd(const u32 base_address, u32 *addr)
{
	hw_status status = RET_OK;

	/*Check the input Parameters*/
	CHECK_INPUT_PARAM(base_address, 0, RET_BAD_NULL_PARAM,
					RES_MMU_BASE + RES_INVALID_INPUT_PARAM);
	/* read values from register */
	*addr = MMUMMU_FAULT_ADReadRegister32(base_address);

	return status;
}
EXPORT_SYMBOL(hw_mmu_flt_adr_rd);


hw_status hw_mmu_ttbset(const u32 base_address, u32 TTBPhysAddr)
{
	hw_status status = RET_OK;
	u32 loadTTB;

	/*Check the input Parameters*/
	CHECK_INPUT_PARAM(base_address, 0, RET_BAD_NULL_PARAM,
					RES_MMU_BASE + RES_INVALID_INPUT_PARAM);

	loadTTB = TTBPhysAddr & ~0x7FUL;
	/* write values to register */
	MMUMMU_TTBWriteRegister32(base_address, loadTTB);

	return status;
}
EXPORT_SYMBOL(hw_mmu_ttbset);

hw_status hw_mmu_twl_enable(const u32 base_address)
{
	hw_status status = RET_OK;

	MMUMMU_CNTLTWLEnableWrite32(base_address, HW_SET);

	return status;
}
EXPORT_SYMBOL(hw_mmu_twl_enable);

hw_status hw_mmu_twl_disable(const u32 base_address)
{
	hw_status status = RET_OK;

	MMUMMU_CNTLTWLEnableWrite32(base_address, HW_CLEAR);

	return status;
}
EXPORT_SYMBOL(hw_mmu_twl_disable);


hw_status HW_MMU_TWLIsEnabled (const u32 base_address,
	u32 *twlIsEnabled)
{
	hw_status status = RET_OK;

	*twlIsEnabled = MMUMMU_CNTLTWLEnableRead32(base_address);

	return status;
}
EXPORT_SYMBOL(HW_MMU_TWLIsEnabled);


hw_status HW_MMU_TWLIsRunning(const u32   base_address,
	u32 *const  twl_is_running)
{
	hw_status status = RET_OK;

	/*Check the input Parameters*/
	CHECK_INPUT_PARAM(base_address, 0, RET_BAD_NULL_PARAM,
					RES_MMU_BASE + RES_INVALID_INPUT_PARAM);

	/* read values from register */
	*twl_is_running = MMUMMU_WALKING_STTWLRunningRead32(base_address);

	return status;

}
EXPORT_SYMBOL(HW_MMU_TWLIsRunning);

hw_status hw_mmu_tlb_flush(const u32 base_address,
	u32 virtual_addr,
	u32 page_size)
{
	hw_status status = RET_OK;
	u32 virt_addr_tag;
	enum hw_mmu_pgsiz_t pg_sizeBits;

	switch (page_size) {
	case HW_PAGE_SIZE_4KB:
	pg_sizeBits = HW_MMU_SMALL_PAGE;
	break;

	case HW_PAGE_SIZE_64KB:
	pg_sizeBits = HW_MMU_LARGE_PAGE;
	break;

	case HW_PAGE_SIZE_1MB:
	pg_sizeBits = HW_MMU_SECTION;
	break;

	case HW_PAGE_SIZE_16MB:
	pg_sizeBits = HW_MMU_SUPERSECTION;
	break;

	default:
	return RET_FAIL;
	}

	/* Generate the 20-bit tag from virtual address */
	virt_addr_tag = ((virtual_addr & MMU_ADDR_MASK) >> 12);

	mme_set_cam_entry (base_address, pg_sizeBits, 0, 0, virt_addr_tag);

	mmu_flsh_entry (base_address);

	return status;
}
EXPORT_SYMBOL(hw_mmu_tlb_flush);


hw_status hw_mmu_tlb_add(const u32        base_address,
	u32              physical_addr,
	u32              virtual_addr,
	u32              page_size,
	u32              entryNum,
	struct hw_mmu_map_attrs_t    *map_attrs,
	enum HW_SetClear_t       preserve_bit,
	enum HW_SetClear_t       valid_bit)
{
	hw_status  status = RET_OK;
	u32 lockReg;
	u32 virt_addr_tag;
	enum hw_mmu_pgsiz_t mmu_pg_size;

	/*Check the input Parameters*/
	CHECK_INPUT_PARAM(base_address, 0, RET_BAD_NULL_PARAM,
				RES_MMU_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(page_size, MMU_PAGE_MAX, RET_PARAM_OUT_OF_RANGE,
				RES_MMU_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(map_attrs->element_size,
			MMU_ELEMENTSIZE_MAX, RET_PARAM_OUT_OF_RANGE,
				RES_MMU_BASE + RES_INVALID_INPUT_PARAM);

		switch (page_size) {
		case HW_PAGE_SIZE_4KB:
		mmu_pg_size = HW_MMU_SMALL_PAGE;
		break;

		case HW_PAGE_SIZE_64KB:
		mmu_pg_size = HW_MMU_LARGE_PAGE;
		break;

		case HW_PAGE_SIZE_1MB:
		mmu_pg_size = HW_MMU_SECTION;
		break;

		case HW_PAGE_SIZE_16MB:
		mmu_pg_size = HW_MMU_SUPERSECTION;
		break;

		default:
		return RET_FAIL;
		}

	lockReg = mmu_lckread_reg_32(base_address);

	/* Generate the 20-bit tag from virtual address */
	virt_addr_tag = ((virtual_addr & MMU_ADDR_MASK) >> 12);

	/* Write the fields in the CAM Entry Register */
	mme_set_cam_entry(base_address,  mmu_pg_size, preserve_bit, valid_bit,
	virt_addr_tag);

	/* Write the different fields of the RAM Entry Register */
	/* endianism of the page,Element Size of the page (8, 16, 32, 64 bit) */
	mmu_set_ram_entry(base_address, physical_addr,
	map_attrs->endianism, map_attrs->element_size, map_attrs->mixedSize);

	/* Update the MMU Lock Register */
	/* currentVictim between lockedBaseValue and (MMU_Entries_Number - 1) */
	mmu_lck_crnt_vctmwite32(base_address, entryNum);

	/* Enable loading of an entry in TLB by writing 1 into LD_TLB_REG register */
	mmu_ld_tlbwrt_reg32(base_address, MMU_LOAD_TLB);


	mmu_lck_write_reg32(base_address, lockReg);

	return status;
}
EXPORT_SYMBOL(hw_mmu_tlb_add);



hw_status hw_mmu_pte_set(const u32        pg_tbl_va,
	u32              physical_addr,
	u32              virtual_addr,
	u32              page_size,
	struct hw_mmu_map_attrs_t    *map_attrs)
{
	hw_status status = RET_OK;
	u32 pte_addr, pte_val;
	long int num_entries = 1;

	switch (page_size) {

	case HW_PAGE_SIZE_4KB:
	pte_addr = hw_mmu_pte_addr_l2(pg_tbl_va, virtual_addr & MMU_SMALL_PAGE_MASK);
	pte_val = ((physical_addr & MMU_SMALL_PAGE_MASK) |
	(map_attrs->endianism << 9) |
	(map_attrs->element_size << 4) |
	(map_attrs->mixedSize << 11) | 2
	);
	break;

	case HW_PAGE_SIZE_64KB:
	num_entries = 16;
	pte_addr = hw_mmu_pte_addr_l2(pg_tbl_va, virtual_addr & MMU_LARGE_PAGE_MASK);
	pte_val = ((physical_addr & MMU_LARGE_PAGE_MASK) |
	(map_attrs->endianism << 9) |
	(map_attrs->element_size << 4) |
	(map_attrs->mixedSize << 11) | 1
	);
	break;

	case HW_PAGE_SIZE_1MB:
	pte_addr = hw_mmu_pte_addr_l1(pg_tbl_va, virtual_addr & MMU_SECTION_ADDR_MASK);
	pte_val = ((((physical_addr & MMU_SECTION_ADDR_MASK) |
	(map_attrs->endianism << 15) |
	(map_attrs->element_size << 10) |
	(map_attrs->mixedSize << 17)) &
	~0x40000) | 0x2
	);
	break;

	case HW_PAGE_SIZE_16MB:
	num_entries = 16;
	pte_addr = hw_mmu_pte_addr_l1(pg_tbl_va, virtual_addr & MMU_SSECTION_ADDR_MASK);
	pte_val = (((physical_addr & MMU_SSECTION_ADDR_MASK) |
	(map_attrs->endianism << 15) |
	(map_attrs->element_size << 10) |
	(map_attrs->mixedSize << 17)
	) | 0x40000 | 0x2
	);
	break;

	case HW_MMU_COARSE_PAGE_SIZE:
	pte_addr = hw_mmu_pte_addr_l1(pg_tbl_va, virtual_addr & MMU_SECTION_ADDR_MASK);
	pte_val = (physical_addr & MMU_PAGE_TABLE_MASK) | 1;
	break;

	default:
	return RET_FAIL;
	}

	while (--num_entries >= 0)
	((u32 *)pte_addr)[num_entries] = pte_val;


	return status;
}
EXPORT_SYMBOL(hw_mmu_pte_set);

hw_status hw_mmu_pt_clear(const u32  pg_tbl_va,
	u32        virtual_addr,
	u32        pg_size)
{
	hw_status status = RET_OK;
	u32 pte_addr;
	long int num_entries = 1;

		switch (pg_size) {
		case HW_PAGE_SIZE_4KB:
		pte_addr = hw_mmu_pte_addr_l2(pg_tbl_va,
				virtual_addr & MMU_SMALL_PAGE_MASK);
		break;

		case HW_PAGE_SIZE_64KB:
		num_entries = 16;
		pte_addr = hw_mmu_pte_addr_l2(pg_tbl_va,
				virtual_addr & MMU_LARGE_PAGE_MASK);
		break;

		case HW_PAGE_SIZE_1MB:
		case HW_MMU_COARSE_PAGE_SIZE:
		pte_addr = hw_mmu_pte_addr_l1(pg_tbl_va,
				virtual_addr & MMU_SECTION_ADDR_MASK);
		break;

		case HW_PAGE_SIZE_16MB:
		num_entries = 16;
		pte_addr = hw_mmu_pte_addr_l1(pg_tbl_va,
				virtual_addr & MMU_SSECTION_ADDR_MASK);
		break;

		default:
		return RET_FAIL;
		}

	while (--num_entries >= 0)
		((u32 *)pte_addr)[num_entries] = 0;

	return status;
}
EXPORT_SYMBOL(hw_mmu_pt_clear);

/* ============================================================================
*  LOCAL FUNCTIONS
* =============================================================================
*/

  /*
-----------------------------------------------------------------------------
 NAME        : mmu_flsh_entry                                                  -
-----------------------------------------------------------------------------
*/
static hw_status mmu_flsh_entry(const u32 base_address)
{
	hw_status status = RET_OK;
	u32 flushEntryData = 0x1;

	/*Check the input Parameters*/
	CHECK_INPUT_PARAM(base_address, 0, RET_BAD_NULL_PARAM,
				RES_MMU_BASE + RES_INVALID_INPUT_PARAM);

	/* write values to register */
	MMUMMU_FLUSH_ENTRYWriteRegister32(base_address, flushEntryData);

	return status;
}
EXPORT_SYMBOL(mmu_flsh_entry);
/*
-----------------------------------------------------------------------------
 NAME        : mme_set_cam_entry                                                -
-----------------------------------------------------
*/
static hw_status mme_set_cam_entry(const u32    base_address,
	const u32    page_size,
	const u32    preserve_bit,
	const u32    valid_bit,
	const u32    virt_addr_tag)
{
	hw_status status = RET_OK;
	u32 mmuCamReg;

	/*Check the input Parameters*/
	CHECK_INPUT_PARAM(base_address, 0, RET_BAD_NULL_PARAM,
				RES_MMU_BASE + RES_INVALID_INPUT_PARAM);

	mmuCamReg = (virt_addr_tag << 12);
	mmuCamReg = (mmuCamReg) | (page_size) |  (valid_bit << 2) | (preserve_bit << 3);

	/* write values to register */
	MMUMMU_CAMWriteRegister32(base_address, mmuCamReg);

	return status;
}
EXPORT_SYMBOL(mme_set_cam_entry);
 /*
-----------------------------------------------------------------------------
 NAME        : mmu_set_ram_entry                                                -
-----------------------------------------------------
*/
static hw_status mmu_set_ram_entry(const u32       base_address,
	const u32       physical_addr,
	enum hw_endianism_t     endianism,
	enum hw_elemnt_siz_t   element_size,
	enum hw_mmu_mixed_size_t  mixedSize)
{
	hw_status status = RET_OK;
	u32 mmuRamReg;

	/*Check the input Parameters*/
	CHECK_INPUT_PARAM(base_address, 0, RET_BAD_NULL_PARAM,
					RES_MMU_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(element_size, MMU_ELEMENTSIZE_MAX, RET_PARAM_OUT_OF_RANGE,
					RES_MMU_BASE + RES_INVALID_INPUT_PARAM);


	mmuRamReg = (physical_addr & MMU_ADDR_MASK);
	mmuRamReg = (mmuRamReg) | ((endianism << 9) |  (element_size << 7) | (mixedSize << 6));

	/* write values to register */
	MMUMMU_RAMWriteRegister32(base_address, mmuRamReg);

	return status;

}
EXPORT_SYMBOL(mmu_set_ram_entry);

long hw_mmi_tlb_dump(const u32 base_address, bool shw_inv_entries)
{
	u32                       i;
	u32                       lockSave;
	u32                       cam;
	u32                       ram;


	/*  Save off the lock register contents,
	we'll restore it when we are done  */

	lockSave = mmu_lckread_reg_32(base_address);

	printk(KERN_ALERT "TLB locked entries = %u, current victim = %u\n",
		((lockSave & MMU_MMU_LOCK_BaseValue_MASK)
		>> MMU_MMU_LOCK_BaseValue_OFFSET),
		((lockSave & MMU_MMU_LOCK_CurrentVictim_MASK)
		>> MMU_MMU_LOCK_CurrentVictim_OFFSET));

	for (i = 0; i < NUM_TLB_ENTRIES; i++) {

		mmu_lck_crnt_vctmwite32(base_address, i);
		cam = MMUMMU_CAMReadRegister32(base_address);
		ram = MMUMMU_RAMReadRegister32(base_address);

	if ((cam & 0x4) != 0) {

		printk(KERN_ALERT "TLB Entry [0x%x]: VA = 0x%x   PA = 0x%x\
		Protected = 0x%x\n)",
						i,
		(cam & MMU_ADDR_MASK),
		(ram & MMU_ADDR_MASK),
		(cam & 0x8) ? 1 : 0);

	} else if (shw_inv_entries != FALSE) {
			printk(KERN_ALERT "TLB Entry [0x%x]: <INVALID>\n", i);
		}
	}
	mmu_lck_write_reg32(base_address, lockSave);
	return RET_OK;
}
EXPORT_SYMBOL(hw_mmi_tlb_dump);

u32 hw_mmu_pte_phyaddr(u32 pte_val, u32 pte_size)
{
	u32	ret_val = 0;

	switch (pte_size) {

	case HW_PAGE_SIZE_4KB:
		ret_val = pte_val & MMU_SMALL_PAGE_MASK;
		break;
	case HW_PAGE_SIZE_64KB:
		ret_val = pte_val & MMU_LARGE_PAGE_MASK;
		break;

	case HW_PAGE_SIZE_1MB:
		ret_val = pte_val & MMU_SECTION_ADDR_MASK;
		break;
	case HW_PAGE_SIZE_16MB:
		ret_val = pte_val & MMU_SSECTION_ADDR_MASK;
		break;
	default:
		/*  Invalid  */
		break;

	}

	return ret_val;
}
EXPORT_SYMBOL(hw_mmu_pte_phyaddr);










