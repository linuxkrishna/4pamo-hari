
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
#define      BaseDucatiL2MMUPhys          0x55082000
#else
#define      BaseDucatiL2MMUPhys     0x5D000000
#endif


#define PG_MASK(pg_size) (~((pg_size)-1))
#define PG_ALIGN_LOW(addr, pg_size) ((addr) & PG_MASK(pg_size))
#define PG_ALIGN_HIGH(addr, pg_size) (((addr)+(pg_size)-1) & PG_MASK(pg_size))

/* Attributes used to manage the DSP MMU page tables */
struct PgTableAttrs {
	struct SYNC_CSOBJECT *hCSObj;	/* Critical section object handle */

	u32 L1BasePa;		/* Physical address of the L1 PT */
	u32 L1BaseVa;		/* Virtual  address of the L1 PT */
	u32 L1size;		/* Size of the L1 PT */
	u32 L1TblAllocPa;
	/* Physical address of Allocated mem for L1 table. May not be aligned */
	u32 L1TblAllocVa;
	/* Virtual address of Allocated mem for L1 table. May not be aligned */
	u32 L1TblAllocSz;
	/* Size of consistent memory allocated for L1 table.
	 * May not be aligned */

	u32 L2BasePa;		/* Physical address of the L2 PT */
	u32 L2BaseVa;		/* Virtual  address of the L2 PT */
	u32 L2size;		/* Size of the L2 PT */
	u32 L2TblAllocPa;
	/* Physical address of Allocated mem for L2 table. May not be aligned */
	u32 L2TblAllocVa;
	/* Virtual address of Allocated mem for L2 table. May not be aligned */
	u32 L2TblAllocSz;
	/* Size of consistent memory allocated for L2 table.
	 * May not be aligned */

	u32 L2NumPages;	/* Number of allocated L2 PT */
	struct PageInfo *pgInfo;  /* Array [L2NumPages] of L2 PT info structs */

} ;

enum pagetype {

	SECTION    = 0,
	LARGE_PAGE = 1,
	SMALL_PAGE = 2,
	TINY_PAGE  = 3
};

static struct PgTableAttrs *pPtAttrs;
static u32 idwNextMMUIndex;
static u32 BaseDucatiL2MMU;

u32 initpage_attributes(u32 L1Size, u32 L1Align, u32 L2NumOfPages)
{
	u32   pg_tbl_pa;
	u32   pg_tbl_va;
	u32   align_size;

#ifndef TESLA_DEBUG
	BaseDucatiL2MMU = (u32)ioremap(BaseDucatiL2MMUPhys, 0x4000);
#else
	/* TODO: HK FOR DEBUGGING PRUPOSE*/
	BaseDucatiL2MMU = (u32)ioremap(BaseDucatiL2MMUPhys, 0x1000);
#endif
	pPtAttrs = (struct PgTableAttrs *)
	MEM_Calloc(sizeof(struct PgTableAttrs), MEM_NONPAGED);

	if (pPtAttrs != NULL) {
		pPtAttrs->L1size = L1Size;
		align_size = pPtAttrs->L1size;
		/* Align sizes are expected to be power of 2 */
		/* we like to get aligned on L1 table size */
		pg_tbl_va = (u32)MEM_AllocPhysMem(pPtAttrs->L1size,
		    align_size, &pg_tbl_pa);
		/* Check if the PA is aligned for us */
		if ((pg_tbl_pa) & (align_size-1)) {
			/* PA not aligned to page table size ,*/
			/* try with more allocation and align */
					MEM_FreePhysMem(
						(void *)pg_tbl_va
						, pg_tbl_pa,
						pPtAttrs->L1size);
			/* we like to get aligned on L1 table size */
			pg_tbl_va = (u32) MEM_AllocPhysMem((pPtAttrs->L1size)*2,
				 align_size, &pg_tbl_pa);
			/* We should be able to get aligned table now */
			pPtAttrs->L1TblAllocPa = pg_tbl_pa;
			pPtAttrs->L1TblAllocVa = pg_tbl_va;
			pPtAttrs->L1TblAllocSz = pPtAttrs->L1size * 2;
			/* Align the PA to the next 'align'  boundary */
			pPtAttrs->L1BasePa = ((pg_tbl_pa) + (align_size-1)) &
					 (~(align_size-1));
			pPtAttrs->L1BaseVa = pg_tbl_va + (pPtAttrs->L1BasePa -
					 pg_tbl_pa);
		} else {
			    /* We got aligned PA, cool */
			    pPtAttrs->L1TblAllocPa = pg_tbl_pa;
			    pPtAttrs->L1TblAllocVa = pg_tbl_va;
			    pPtAttrs->L1TblAllocSz = pPtAttrs->L1size;
			    pPtAttrs->L1BasePa = pg_tbl_pa;
			    pPtAttrs->L1BaseVa = pg_tbl_va;
		}

		if (pPtAttrs->L1BaseVa) {
			memset((u8 *)pPtAttrs->L1BaseVa, 0x00,
			pPtAttrs->L1size);
		}
		pPtAttrs->L2NumPages = L2NumOfPages;
		pPtAttrs->L2size = HW_MMU_COARSE_PAGE_SIZE *
				   pPtAttrs->L2NumPages;
		align_size = 4; /* Make it u32 aligned  */
		/* we like to get aligned on L1 table size */
		pg_tbl_va = (u32)MEM_AllocPhysMem(pPtAttrs->L2size,
			    align_size, &pg_tbl_pa);
		pPtAttrs->L2TblAllocPa = pg_tbl_pa;
		pPtAttrs->L2TblAllocVa = pg_tbl_va;
		pPtAttrs->L2TblAllocSz = pPtAttrs->L2size;
		pPtAttrs->L2BasePa = pg_tbl_pa;
		pPtAttrs->L2BaseVa = pg_tbl_va;
		if (pPtAttrs->L2BaseVa) {
				memset((u8 *)pPtAttrs->L2BaseVa, 0x00,
				pPtAttrs->L2size);
		}
		pPtAttrs->pgInfo = MEM_Calloc(pPtAttrs->L2NumPages *
				sizeof(struct PageInfo), MEM_NONPAGED);
		DPRINTK("L1 pa %x, va %x, size %x\n L2 pa %x, va "
			 "%x, size %x\n", pPtAttrs->L1BasePa,
			 pPtAttrs->L1BaseVa, pPtAttrs->L1size,
			 pPtAttrs->L2BasePa, pPtAttrs->L2BaseVa,
			 pPtAttrs->L2size);
		DPRINTK("pPtAttrs %x L2 NumPages %x pgInfo %x\n",
			 pPtAttrs, pPtAttrs->L2NumPages, (u32)pPtAttrs->pgInfo);
	}
	return 0;
}


/*  ........................................................................
  func    PrintPTEs

  desc    Print the DSP MMU Table Entries

  modif   None
  ........................................................................
*/
void  PrintPTEs(bool aShowInvEntries, bool aShowRepeatEntries)
{
	u32               pteVal;
	u32               pteSize;
	u32               lastSect = 0;
	u32               thisSect = 0;
	u32               curL1Entry;
	u32               curL2Entry;
	u32               pgTblVa;
	u32               L1BaseVa;
	u32               L2BaseVa    = 0;
	u32               L2BasePa    = 0;

	L1BaseVa = pPtAttrs->L1BaseVa;
	pgTblVa = L1BaseVa;

	DPRINTK("\n*** Currently programmed PTEs : Max possible L1 Entries"
			"[%d] ***\n", (pPtAttrs->L1size / sizeof(u32)));

	/*  Walk all L1 entries, dump out info.  Dive into L2 if necessary  */
	for (curL1Entry = 0; curL1Entry < (pPtAttrs->L1size / sizeof(u32));
						curL1Entry++) {

		/*pteVal = pL1PgTbl[curL1Entry];*/
		pteVal = *((u32 *)(pgTblVa + (curL1Entry * sizeof(u32))));
		pteSize = HW_MMU_PteSizeL1(pteVal);

		if (pteSize == HW_PAGE_SIZE_16MB) {
			thisSect = HW_MMU_PtePhyAddr(pteVal, pteSize);
			if (thisSect != lastSect) {

				lastSect = thisSect;

				DPRINTK("PTE L1 [16 MB] -> VA =  \
						0x%x PA = 0x%x\n",
						curL1Entry << 24, thisSect);

			} else if (aShowRepeatEntries != FALSE)
				DPRINTK("    {REPEAT}\n");
		} else if (pteSize == HW_PAGE_SIZE_1MB) {
			thisSect = HW_MMU_PtePhyAddr(pteVal, pteSize);
			if (thisSect != lastSect) {

				lastSect = thisSect;

				DPRINTK("PTE L1 [1 MB ] -> VA =  \
						0x%x PA = 0x%x\n",
						curL1Entry << 20, thisSect);

			} else if (aShowRepeatEntries != FALSE)
				DPRINTK("    {REPEAT}\n");

		} else if (pteSize == HW_MMU_COARSE_PAGE_SIZE) {
			/*  Get the L2 data for this  */
			DPRINTK("PTE L1 [L2   ] -> VA = \
					0x%x\n", curL1Entry << 20);

/* Get the L2 PA from the L1 PTE, and find corresponding L2 VA*/
			L2BasePa    = HW_MMU_PteCoarseL1(pteVal) ;
			L2BaseVa    = L2BasePa - pPtAttrs->L2BasePa +
						pPtAttrs->L2BaseVa ;

			for (curL2Entry = 0;
			 curL2Entry < (HW_MMU_COARSE_PAGE_SIZE / sizeof(u32));
			 curL2Entry++) {

				pteVal = *((u32 *)(L2BaseVa +
						(curL2Entry * sizeof(u32))));

				pteSize = HW_MMU_PteSizeL2(pteVal);

				if ((pteSize == HW_PAGE_SIZE_64KB) ||
				    (pteSize == HW_PAGE_SIZE_4KB)) {

					thisSect = HW_MMU_PtePhyAddr
						(pteVal, pteSize);

					if (thisSect != lastSect) {
						lastSect = thisSect;
						DPRINTK("PTE L2 [%s KB] ->"
						"VA = 0x%x   PA = 0x%x\n",
						(pteSize == HW_PAGE_SIZE_64KB) ?
						"64" : "4",
						((curL1Entry << 20)
						| (curL2Entry << 12)),
						thisSect);
					} else if (aShowRepeatEntries != FALSE)
						DPRINTK("        {REPEAT}");
				} else  if (aShowInvEntries != FALSE) {


					DPRINTK("PTE L2 [INVALID] -> VA = \
						0x%x",
						((curL1Entry << 20) |
						(curL2Entry << 12)));
					continue;
				}
			 }
		} else if (aShowInvEntries != FALSE) {
			/*  Entry is invalid (not set), skip it  */
			DPRINTK("PTE L1 [INVALID] -> VA = 0x%x",
						curL1Entry << 20);
			continue;
		}
	}
	/*  Dump the TLB entries as well  */
	DPRINTK("\n*** Currently programmed TLBs ***\n");
	HW_MMU_TLBDump(BaseDucatiL2MMU, FALSE);
	DPRINTK("*** DSP MMU DUMP COMPLETED ***\n");
}



/*
 *  ======== PteSet ========
 *      This function calculates PTE address (MPU virtual) to be updated
 *      It also manages the L2 page tables
 */
static DSP_STATUS PteSet(u32 pa, u32 va,
			 u32 size, struct HW_MMUMapAttrs_t *attrs)
{
	u32 i;
	u32 pteVal;
	u32 pteAddrL1;
	u32 pteSize;
	u32 pgTblVa;      /* Base address of the PT that will be updated */
	u32 L1BaseVa;
	 /* Compiler warns that the next three variables might be used
	 * uninitialized in this function. Doesn't seem so. Working around,
	 * anyways.  */
	u32 L2BaseVa = 0;
	u32 L2BasePa = 0;
	u32 L2PageNum = 0;
	struct PgTableAttrs *pt = pPtAttrs;
	DSP_STATUS status = DSP_SOK;
	DPRINTK("> PteSet pPgTableAttrs %x, pa %x, va %x, "
		 "size %x, attrs %x\n", (u32)pt, pa, va, size, (u32)attrs);
	L1BaseVa = pt->L1BaseVa;
	pgTblVa = L1BaseVa;
	if ((size == HW_PAGE_SIZE_64KB) || (size == HW_PAGE_SIZE_4KB)) {
		/* Find whether the L1 PTE points to a valid L2 PT */
		pteAddrL1 = HW_MMU_PteAddrL1(L1BaseVa, va);
		if (pteAddrL1 <= (pt->L1BaseVa + pt->L1size)) {
			pteVal = *(u32 *)pteAddrL1;
			pteSize = HW_MMU_PteSizeL1(pteVal);
		} else {
			return DSP_EFAIL;
		}
		/* TODO: ADD synchronication element*/
		/*		SYNC_EnterCS(pt->hCSObj);*/
		if (pteSize == HW_MMU_COARSE_PAGE_SIZE) {
			/* Get the L2 PA from the L1 PTE, and find
			 * corresponding L2 VA */
			L2BasePa = HW_MMU_PteCoarseL1(pteVal);
			L2BaseVa = L2BasePa - pt->L2BasePa + pt->L2BaseVa;
			L2PageNum = (L2BasePa - pt->L2BasePa) /
				    HW_MMU_COARSE_PAGE_SIZE;
		} else if (pteSize == 0) {
			/* L1 PTE is invalid. Allocate a L2 PT and
			 * point the L1 PTE to it */
			/* Find a free L2 PT. */
			for (i = 0; (i < pt->L2NumPages) &&
			    (pt->pgInfo[i].numEntries != 0); i++)
				;;
			if (i < pt->L2NumPages) {
				L2PageNum = i;
				L2BasePa = pt->L2BasePa + (L2PageNum *
					   HW_MMU_COARSE_PAGE_SIZE);
				L2BaseVa = pt->L2BaseVa + (L2PageNum *
					   HW_MMU_COARSE_PAGE_SIZE);
				/* Endianness attributes are ignored for
				 * HW_MMU_COARSE_PAGE_SIZE */
				status = HW_MMU_PteSet(L1BaseVa, L2BasePa, va,
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
			pgTblVa = L2BaseVa;
			if (size == HW_PAGE_SIZE_64KB)
				pt->pgInfo[L2PageNum].numEntries += 16;
			else
				pt->pgInfo[L2PageNum].numEntries++;

			DPRINTK("L2 BaseVa %x, BasePa %x, "
				 "PageNum %x numEntries %x\n", L2BaseVa,
				 L2BasePa, L2PageNum,
				 pt->pgInfo[L2PageNum].numEntries);
		}
/*		SYNC_LeaveCS(pt->hCSObj);*/
	}
	if (DSP_SUCCEEDED(status)) {
		DPRINTK("PTE pgTblVa %x, pa %x, va %x, size %x\n",
			 pgTblVa, pa, va, size);
		DPRINTK("PTE endianism %x, elementSize %x, "
			  "mixedSize %x\n", attrs->endianism,
			  attrs->elementSize, attrs->mixedSize);
		status = HW_MMU_PteSet(pgTblVa, pa, va, size, attrs);
	}
	DPRINTK("< PteSet status %x\n", status);
	return status;
}


/*
 *  ======== PteUpdate ========
 *      This function calculates the optimum page-aligned addresses and sizes
 *      Caller must pass page-aligned values
 */
static DSP_STATUS PteUpdate(u32 pa, u32 va, u32 size,
			    struct HW_MMUMapAttrs_t *mapAttrs)
{
	u32 i;
	u32 allBits;
	u32 paCurr = pa;
	u32 vaCurr = va;
	u32 numBytes = size;
	DSP_STATUS status = DSP_SOK;
	u32 pgSize[] = { HW_PAGE_SIZE_16MB, HW_PAGE_SIZE_1MB,
			   HW_PAGE_SIZE_64KB, HW_PAGE_SIZE_4KB };
	DPRINTK("> PteUpdate  pa %x, va %x, "
		 "size %x, mapAttrs %x\n", pa, va, size, (u32)mapAttrs);
	while (numBytes && DSP_SUCCEEDED(status)) {
		/* To find the max. page size with which both PA & VA are
		 * aligned */
		allBits = paCurr | vaCurr;
		DPRINTK("allBits %x, paCurr %x, vaCurr %x, "
			 "numBytes %x\n ", allBits, paCurr, vaCurr, numBytes);
		for (i = 0; i < 4; i++) {
			if ((numBytes >= pgSize[i]) && ((allBits &
			   (pgSize[i] - 1)) == 0)) {
				DPRINTK("pgSize %x\n", pgSize[i]);
				status = PteSet(paCurr,
						vaCurr, pgSize[i], mapAttrs);
				paCurr += pgSize[i];
				vaCurr += pgSize[i];
				numBytes -= pgSize[i];
				 /* Don't try smaller sizes. Hopefully we have
				 * reached an address aligned to a bigger page
				 * size */
				break;
			}
		}
	}
	DPRINTK("< PteUpdate status %x numBytes %x\n", status, numBytes);
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
static DSP_STATUS Ducati_MemMap(u32 ulMpuAddr, u32 ulVirtAddr,
				 u32 ulNumBytes, u32 ulMapAttr)
{
	u32 attrs;
	DSP_STATUS status = DSP_SOK;
	struct HW_MMUMapAttrs_t hwAttrs;

	DPRINTK("> WMD_BRD_MemMap  pa %x, va %x, "
		 "size %x, ulMapAttr %x\n", ulMpuAddr, ulVirtAddr,
		 ulNumBytes, ulMapAttr);
	if (ulNumBytes == 0)
		return DSP_EINVALIDARG;

	if (ulMapAttr != 0) {
		attrs = ulMapAttr;
	} else {
		/* Assign default attributes */
		attrs = DSP_MAPVIRTUALADDR | DSP_MAPELEMSIZE16;
	}
	/* Take mapping properties */
	if (attrs & DSP_MAPBIGENDIAN)
		hwAttrs.endianism = HW_BIG_ENDIAN;
	else
		hwAttrs.endianism = HW_LITTLE_ENDIAN;

	hwAttrs.mixedSize = (enum HW_MMUMixedSize_t)
			     ((attrs & DSP_MAPMIXEDELEMSIZE) >> 2);
	/* Ignore elementSize if mixedSize is enabled */
	if (hwAttrs.mixedSize == 0) {
		if (attrs & DSP_MAPELEMSIZE8) {
			/* Size is 8 bit */
			hwAttrs.elementSize = HW_ELEM_SIZE_8BIT;
		} else if (attrs & DSP_MAPELEMSIZE16) {
			/* Size is 16 bit */
			hwAttrs.elementSize = HW_ELEM_SIZE_16BIT;
		} else if (attrs & DSP_MAPELEMSIZE32) {
			/* Size is 32 bit */
			hwAttrs.elementSize = HW_ELEM_SIZE_32BIT;
		} else if (attrs & DSP_MAPELEMSIZE64) {
			/* Size is 64 bit */
			hwAttrs.elementSize = HW_ELEM_SIZE_64BIT;
		} else {
			/* Mixedsize isn't enabled, so size can't be
			 * zero here */
			DPRINTK("WMD_BRD_MemMap: MMU element size is zero\n");
			return DSP_EINVALIDARG;
		}
	}
	status = PteUpdate(ulMpuAddr, ulVirtAddr,
			 ulNumBytes, &hwAttrs);

	 /* In any case, flush the TLB
	 * This is called from here instead from PteUpdate to avoid unnecessary
	 * repetition while mapping non-contiguous physical regions of a virtual
	 * region */
	HW_MMU_TLBFlushAll(BaseDucatiL2MMU);
	DPRINTK("< WMD_BRD_MemMap status %x\n", status);
	return status;
}

/*  ............................................................................
*  func    GetMMUEntrySize
*
*  desc    Decides a TLB entry size
*
*  modif   none
*............................................................................
*/
static DSP_STATUS
GetMMUEntrySize(u32  dwPhyAddr,
				u32  dwSize,
				enum pagetype *sizeTlb,
				u32  *entrySize)
{
	DSP_STATUS  status        = DSP_SOK ;
	bool        pageAlign4KB  = false;
	bool        pageAlign64KB = false ;
	bool        pageAlign1MB  = false ;

	/*  First check the page alignment*/
	if ((dwPhyAddr % PAGE_SIZE_4KB)  == 0)
		pageAlign4KB  = true ;
	if ((dwPhyAddr % PAGE_SIZE_64KB) == 0)
		pageAlign64KB = true ;
	if ((dwPhyAddr % PAGE_SIZE_1MB)  == 0)
		pageAlign1MB  = true ;

	if ((!pageAlign64KB) && (!pageAlign1MB)  && (!pageAlign4KB))
		status = DSP_EFAIL;;

	if (DSP_SUCCEEDED(status)) {

		/*  Now decide the entry size */
		if (dwSize >= PAGE_SIZE_1MB) {

			if (pageAlign1MB) {

				*sizeTlb   = SECTION ;
				*entrySize = PAGE_SIZE_1MB ;
			} else if (pageAlign64KB) {

				*sizeTlb   = LARGE_PAGE ;
				*entrySize = PAGE_SIZE_64KB ;
			} else if (pageAlign4KB) {

				*sizeTlb   = SMALL_PAGE ;
				*entrySize = PAGE_SIZE_4KB ;
			} else
				status = DSP_EFAIL;;
		} else if (dwSize > PAGE_SIZE_4KB && dwSize < PAGE_SIZE_1MB) {

			if (pageAlign64KB) {

				*sizeTlb   = LARGE_PAGE ;
				*entrySize = PAGE_SIZE_64KB ;
			} else if (pageAlign4KB) {

				*sizeTlb   = SMALL_PAGE ;
				*entrySize = PAGE_SIZE_4KB ;
			} else
				status = DSP_EFAIL;
		} else if (dwSize == PAGE_SIZE_4KB) {

				if (pageAlign4KB) {

					*sizeTlb   = SMALL_PAGE ;
					*entrySize = PAGE_SIZE_4KB ;
				} else
					status = DSP_EFAIL;
			} else
			    status = DSP_EFAIL;
	}

    return status ;
}


/*  ............................................................................
  func    AddDSPMMUEntry

  desc    Add DSP MMU entries corresponding to given MPU-Physical address
				and DSP-virtual address

  modif   none
  ............................................................................
*/
static DSP_STATUS AddDSPMMUEntry(u32  *dwPhyAddr, u32  *dwDspAddr,
						u32  dwEntrySize)
{
	u32       mappedSize = 0 ;
	enum pagetype      sizeTlb    = SECTION ;
	u32       entrySize  = 0 ;
	DSP_STATUS  status     = 0;/*DSP_SOK ;*/
	u32 pageSize   = HW_PAGE_SIZE_1MB ;

	struct HW_MMUMapAttrs_t  mapAttrs = { HW_LITTLE_ENDIAN,
						HW_ELEM_SIZE_16BIT,
						HW_MMU_CPUES };

	DPRINTK("Entered AddDSPMMUEntry dwPhyAddr = \
						0x%x, dwDspAddr = 0x%x, "
						"dwEntrySize = 0x%x\n",
		*dwPhyAddr, *dwDspAddr, dwEntrySize);


	while ((mappedSize < dwEntrySize) && (DSP_SUCCEEDED(status))) {



		status = GetMMUEntrySize(*dwPhyAddr,
				(dwEntrySize - mappedSize),
				&sizeTlb,
				&entrySize);

		if (sizeTlb == SECTION)
			pageSize = HW_PAGE_SIZE_1MB ;

		else if (sizeTlb == LARGE_PAGE)
			pageSize = HW_PAGE_SIZE_64KB ;

		else if (sizeTlb == SMALL_PAGE)
			pageSize = HW_PAGE_SIZE_4KB ;

		if (DSP_SUCCEEDED(status)) {

			HW_MMU_TLBAdd((BaseDucatiL2MMU),
						*dwPhyAddr,
						*dwDspAddr,
						pageSize,
						idwNextMMUIndex++,
						&mapAttrs,
						HW_SET, /* preserved*/
						HW_SET); /* valid*/

			mappedSize  += entrySize ;
			*dwPhyAddr   += entrySize ;
			*dwDspAddr   += entrySize ;

			if (idwNextMMUIndex > 32)
				status = DSP_EFAIL;
/*set  a valid value here;DSP_EFAIL ;*/
		}
	}
	DPRINTK("Exited AddDSPMMUEntrydwPhyAddr = 0x%x, dwDspAddr = 0x%x\n",
		*dwPhyAddr, *dwDspAddr);
	return status ;
 }


/*  ............................................................................
*  func    AddDSPMMUEntry
*
*  desc    Add DSP MMU entries corresponding to given MPU-Physical address
*          and DSP-virtual address
*
*  modif   none
*  ............................................................................
*/
static DSP_STATUS AddEntryExt(u32  *dwPhyAddr, u32  *dwDspAddr,
					u32  dwEntrySize)
{
	u32       mappedSize = 0 ;
	enum pagetype     sizeTlb    = SECTION ;
	u32       entrySize  = 0 ;
	DSP_STATUS  status     = 0;
	u32       pageSize   = HW_PAGE_SIZE_1MB ;
	u32      flags = DSP_MAPELEMSIZE32;

	while ((mappedSize < dwEntrySize) && (DSP_SUCCEEDED(status))) {

		/*  GetMMUEntrySize fills the sizeTlb and entrySize
		based on alignment and size of memory to map
		to DSP - dwEntrySize */
		status = GetMMUEntrySize(*dwPhyAddr,
				(dwEntrySize - mappedSize),
				&sizeTlb,
				&entrySize);

		if (sizeTlb == SECTION)
			pageSize = HW_PAGE_SIZE_1MB ;
		else if (sizeTlb == LARGE_PAGE)
			pageSize = HW_PAGE_SIZE_64KB ;
		else if (sizeTlb == SMALL_PAGE)
			pageSize = HW_PAGE_SIZE_4KB ;

		if (DSP_SUCCEEDED(status)) {

			Ducati_MemMap(*dwPhyAddr, *dwDspAddr, pageSize, flags);
			mappedSize  += entrySize ;
			*dwPhyAddr   += entrySize ;
			*dwDspAddr   += entrySize ;
		}
	}
	return status ;
 }



/*  ========================================================================
*  func    MMUInit
*
* desc    Initialize the Ducati MMU.
*  =========================================================================
*/
u32 MMUInit(u32 aPhysAddr, int aSize)
{
	u32 retVal = 0;
	u32 ducatiMMULinAddr = BaseDucatiL2MMU;
	u32 regValue = 0;
	DSP_STATUS status = 0 ;
	u32 dwPhyAddr = 0 ;
	u32 numL4Entries;
	u32 dwDspTotalSize;
	u32 dwExtMemMapBeg      = 0 ;
	u32 dwINTERM3SHMEnd      = 0 ;
	u32 i = 0;
	u32 mapAttrs;
	u32 dwDucatiBootAddr = 0;

	numL4Entries = (sizeof(L4Map) / sizeof(struct MMUEntry));


	DPRINTK("\n  Programming Ducati MMU using linear address [0x%x]",
						ducatiMMULinAddr);

	/*  Disable the MMU & TWL */
	HW_MMU_Disable(BaseDucatiL2MMU);
	HW_MMU_TWLDisable(BaseDucatiL2MMU);

	idwNextMMUIndex = 0 ;

	dwINTERM3SHMEnd = DUCATI_BASEADDRESS +
		INTERM3SHM_BASEADDR_OFFSET + INTERM3SHM_LENGTH - 0x1;

	/*   Calculate Total Size of Memory */
	dwDspTotalSize = dwINTERM3SHMEnd + 0x1 - DUCATI_BASEADDRESS;
	dwExtMemMapBeg = DUCATI_BASEADDRESS + PAGE_SIZE_4KB;
	dwDspTotalSize = PG_ALIGN_HIGH(dwDspTotalSize, PAGE_SIZE_4KB);

	dwDspTotalSize -= PAGE_SIZE_4KB;

	dwPhyAddr = aPhysAddr;
	DPRINTK("Value before calling AddDSPMMUEntry dwPhyAddr = 0x%x,"
		"dwDucatiBootAddr = 0x%x\n", dwPhyAddr, dwDucatiBootAddr);
		AddDSPMMUEntry(&dwPhyAddr, &dwDucatiBootAddr, PAGE_SIZE_4KB);
	DPRINTK("Value after calling AddDSPMMUEntry dwPhyAddr = 0x%x,"
		"dwDucatiBootAddr = 0x%x\n", dwPhyAddr, dwDucatiBootAddr);

	/* Lock the base counter*/
	HW_MMU_NumLockedSet(ducatiMMULinAddr,
						idwNextMMUIndex);

	HW_MMU_VictimNumSet(ducatiMMULinAddr,
						idwNextMMUIndex);


	mapAttrs = 0x00000000;
	mapAttrs |= DSP_MAPLITTLEENDIAN;
	mapAttrs |= DSP_MAPPHYSICALADDR;
	mapAttrs |= DSP_MAPELEMSIZE32;

	for (i = 0; i < numL4Entries; i++) {


		Ducati_MemMap(L4Map[i].ulPhysAddr,  L4Map[i].ulVirtAddr,
						L4Map[i].ulSize, mapAttrs);

		if (DSP_FAILED(status)) {

			DPRINTK("**** Failed to map Peripheral ****");
			DPRINTK("Phys addr [0x%x] Virt addr [0x%x] size [0x%x]",
			    L4Map[i].ulPhysAddr, L4Map[i].ulVirtAddr,
			    L4Map[i].ulSize);
			DPRINTK(" Status [0x%x]", status);
			break ;
		}
	}

	DPRINTK(" >> AddEntryExt2 entry [0x%x]  [0x%x] [0x%x]\n",
		dwPhyAddr, dwExtMemMapBeg, dwDspTotalSize);
	status = AddEntryExt(&dwPhyAddr, &dwExtMemMapBeg, dwDspTotalSize);

	/*    Set the TTB to point to the L1 page table's physical address */
	HW_MMU_TTBSet(ducatiMMULinAddr, pPtAttrs->L1BasePa);

	/*    Enable the TWL */
	HW_MMU_TWLEnable(ducatiMMULinAddr);
	/*HW_MMU_AutoIdleEnable(ducatiMMULinAddr);*/

	HW_MMU_Enable(ducatiMMULinAddr);

	/*  MMU Debug Statements */
	regValue = *((REG u32 *)(ducatiMMULinAddr + 0x40));
	DPRINTK("  Ducati TWL Status [0x%x]\n", regValue);

	regValue = *((REG u32 *)(ducatiMMULinAddr + 0x4C));
	DPRINTK("  Ducati TTB Address [0x%x]\n", regValue);

	regValue = *((REG u32 *)(ducatiMMULinAddr + 0x44));
	DPRINTK("  Ducati MMU Status [0x%x]\n", regValue);

	/*  Dump the MMU Entries */
	PrintPTEs(FALSE, FALSE);

	DPRINTK("  Programmed Ducati BootVectors 0x0 to first page at [0x%x]",
							aPhysAddr);
	DPRINTK("  Programmed Mailbox into Ducati MMU at [0xAA0F4000]\n");

	DPRINTK("  Make sure your BaseImage Code is within [0x%x] & [0x%x]",
			aPhysAddr + 0x1000, aPhysAddr + aSize);

	DPRINTK("  Leaving DDucati_MMUManager::MMUInit [0x%x]", retVal);

	return retVal;
}

/*  ========================================================================
*  func    MMUDeInit
*
* desc    De-Initialize the Ducati MMU and free the
* memory allocation for L1 and L2 pages
*  =========================================================================
*/
u32 MMUDeInit(void)
{
	DPRINTK("  Freeing memory allocated in MMUDeInit\n");
	if (pPtAttrs->L2TblAllocVa) {
		MEM_FreePhysMem((void *)pPtAttrs->L2TblAllocVa,
				pPtAttrs->L2TblAllocPa, pPtAttrs->L2TblAllocSz);
	}
	if (pPtAttrs->L1TblAllocVa) {
		MEM_FreePhysMem((void *)pPtAttrs->L1TblAllocVa,
				pPtAttrs->L1TblAllocPa, pPtAttrs->L1TblAllocSz);
	}
	if (pPtAttrs)
		MEM_Free((void *)pPtAttrs);
	return 0;
}

