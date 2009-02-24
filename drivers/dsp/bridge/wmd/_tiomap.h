/*
 * _tiomap.h
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
 *  ======== _tiomap.h ========
 *  Description:
 *      Definitions and types private to this WMD.
 *
 *! Revision History
 *! ================
 *! 12-Apr-2004 hp:  Removed the L4 entry of Dsp DMA , not needed
 *! 08-Mar-2004 sb:  Added the Dynamic Memory Mapping feature - PgTableAttrs
 *! 21-Mar-2003 sb:  Added numTLBEntries to struct WMD_DEV_CONTEXT
 *! 20-Feb-2003 vp:  Ported to Linux platform.
 *! 30-Jul-2002 rr:  Modified TBC_ID_VALUE to 0xb47002f.
 *! 10-May-2002 sg:  Added ARM_SYSST_OFFSET and IDLE_DSP_MASK.
 *! 14-Mar-2002 rr:  Added Boot vector field to pDevContext.
 *!                  Added TBC Register defines and value for OMAP1510.
 *! 12-Jan-2002 ag:  wIntrVal2Dsp Added.
 *! 05-Nov-2001 kc:  Added MMU fault related definitions.
 *! 03-Aug-2001 ag   Added TLB table for OEM cofig of DSP-MMU.
 *! 23-Aug-2001 rr:  Added API_SIZE define.
 *! 16-Aug-2001 rr:  Added dwDspExtBaseAddr to access the Ext memory.
 *! 24-Jul-2001 ag:  Added Internal SRAM MMU table entry.
 *! 27-Jun-2001 rr:  Name changed to _tihelen.h.
 *! 27-Jun-2001 ag:  Added dwIntAddr in WMD_DEV_CONTEXT used for MB INTRs msk.
 *! 07-May-2001 ag:  Added DSP Clock Module CLKM2.
 *!                  Added TIHEL_SDRAMPHYSBASE used for DSP MMU init.
 *!                  Added ClearBit() MACRO.
 *! 18-Jan-2001 rr:  Created
 */

#ifndef _TIOMAP_
#define _TIOMAP_

#include <dspbridge/devdefs.h>
#include <hw_defs.h>
#include <hw_mbox.h>
#include <dspbridge/wmdioctl.h>		/* for WMDIOCTL_EXTPROC defn */
#include <dspbridge/sync.h>
#include <dspbridge/clk.h>

struct MAP_L4PERIPHERAL {
	u32 physAddr;
	u32 dspVirtAddr;
	u32 ulSize;
} ;

#define ARM_MAILBOX_START               0xfffcf000
#define ARM_MAILBOX_LENGTH              0x800

/* New Registers in OMAP3.1 */

#define TESTBLOCK_ID_START              0xfffed400
#define TESTBLOCK_ID_LENGTH             0xff

/* ID Returned by OMAP1510 */
#define TBC_ID_VALUE                    0xB47002F

#define SPACE_LENGTH                    0x2000
#define API_CLKM_DPLL_DMA               0xfffec000
#define ARM_INTERRUPT_OFFSET            0xb00

#define BIOS_24XX

#define L4_PERIPHERAL_NULL          0x0
#define DSPVA_PERIPHERAL_NULL       0x0

#define MAX_LOCK_TLB_ENTRIES 15

#define L4_PERIPHERAL_UART3                      0x48020000
#define DSPVA_PERIPHERAL_UART3                  L4_PERIPHERAL_UART3

#define L4_PERIPHERAL_GPIO2                      0x48055000
#define DSPVA_PERIPHERAL_GPIO2                   L4_PERIPHERAL_GPIO2
#define L4_PERIPHERAL_GPIO3                      0x48057000
#define DSPVA_PERIPHERAL_GPIO3                   L4_PERIPHERAL_GPIO3
#define L4_PERIPHERAL_GPIO4                      0x48059000
#define DSPVA_PERIPHERAL_GPIO4                   L4_PERIPHERAL_GPIO4
#define L4_PERIPHERAL_GPIO5                      0x4805B000
#define DSPVA_PERIPHERAL_GPIO5                   L4_PERIPHERAL_GPIO5

#define L4_PERIPHERAL_GPTIMER5                   0x49038000
#define DSPVA_PERIPHERAL_GPTIMER5                L4_PERIPHERAL_GPTIMER5
#define L4_PERIPHERAL_GPTIMER6                   0x4903A000
#define DSPVA_PERIPHERAL_GPTIMER6                L4_PERIPHERAL_GPTIMER6
#define L4_PERIPHERAL_GPTIMER7                   0x4903C000
#define DSPVA_PERIPHERAL_GPTIMER7                L4_PERIPHERAL_GPTIMER7
#define L4_PERIPHERAL_GPTIMER8                   0x4903E000
#define DSPVA_PERIPHERAL_GPTIMER8                L4_PERIPHERAL_GPTIMER8

#define L4_PERIPHERAL_UART1                      0x4806A000
#define DSPVA_PERIPHERAL_UART1                   L4_PERIPHERAL_UART1
#define L4_PERIPHERAL_UART2                      0x4806C000
#define DSPVA_PERIPHERAL_UART2                   L4_PERIPHERAL_UART2
#define L4_PERIPHERAL_UART4                      0x4806E000
#define DSPVA_PERIPHERAL_UART4                   L4_PERIPHERAL_UART4

#define L4_PERIPHERAL_MCBSP4                     0x48074000
#define DSPVA_PERIPHERAL_MCBSP4                  L4_PERIPHERAL_MCBSP4
#define L4_PERIPHERAL_MCBSP5                     0x48096000
#define DSPVA_PERIPHERAL_MCBSP5                  L4_PERIPHERAL_MCBSP5

#define L4_PERIPHERAL_MCSPI1                     0x48098000
#define DSPVA_PERIPHERAL_MCSPI1                  L4_PERIPHERAL_MCSPI1
#define L4_PERIPHERAL_MCSPI2                     0x4809A000
#define DSPVA_PERIPHERAL_MCSPI2                  L4_PERIPHERAL_MCSPI2

#define L4_PERIPHERAL_MCBSP1                     0x49022000
#define DSPVA_PERIPHERAL_MCBSP1                  L4_PERIPHERAL_MCBSP1
#define L4_PERIPHERAL_MCBSP2                     0x49024000
#define DSPVA_PERIPHERAL_MCBSP2                  L4_PERIPHERAL_MCBSP2
#define L4_PERIPHERAL_MCBSP3                     0x49028000
#define DSPVA_PERIPHERAL_MCBSP3                  L4_PERIPHERAL_MCBSP3

#define L4_PERIPHERAL_WDT3                       0x49030000
#define DSPVA_PERIPHERAL_WDT3                    L4_PERIPHERAL_WDT3

#define L4_PERIPHERAL_MBOX                       0x4A0F4000
#define DSPVA_PERIPHERAL_MBOX                    L4_PERIPHERAL_MBOX


#define L4_PERIPHERAL_CM1                        0x4A004000
#define DSPVA_PERIPHERAL_CM1                     L4_PERIPHERAL_CM1
#define L4_PERIPHERAL_CM2_P1                     0x4A008000 /*(4Kb)*/
#define DSPVA_PERIPHERAL_CM2_P1                  L4_PERIPHERAL_CM2_P1
#define L4_PERIPHERAL_CM2_P2                     0x4A009000 /*(4Kb)*/
#define DSPVA_PERIPHERAL_CM2_P2                  L4_PERIPHERAL_CM2_P2


#define L4_PERIPHERAL_PRM_P1                     0x4A306000 /*(4Kb)*/
#define DSPVA_PERIPHERAL_PRM_P1			 L4_PERIPHERAL_PRM_P1
#define L4_PERIPHERAL_PRM_P2                     0x4A307000 /*(4Kb)*/
#define DSPVA_PERIPHERAL_PRM_P2			 L4_PERIPHERAL_PRM_P2



#define L4_PERIPHERAL_SCRM                       0x4A30A000
#define DSPVA_PERIPHERAL_SCRM                    L4_PERIPHERAL_SCRM

#define L4_PERIPHERAL_GPIO1                      0x4A310000
#define DSPVA_PERIPHERAL_GPIO1                   L4_PERIPHERAL_GPIO1

#define NOTIFY_TESLA_EVENTNUMBER 0x00000000

static const struct MAP_L4PERIPHERAL L4PeripheralTable[] = {
/* Mailbox 4KB */
{L4_PERIPHERAL_MBOX, DSPVA_PERIPHERAL_MBOX, HW_PAGE_SIZE_4KB},
/* SCM 4KB */
{L4_PERIPHERAL_SCRM, DSPVA_PERIPHERAL_SCRM, HW_PAGE_SIZE_4KB},
/* PRM 8KB */
{L4_PERIPHERAL_PRM_P1, DSPVA_PERIPHERAL_PRM_P1, HW_PAGE_SIZE_4KB},
{L4_PERIPHERAL_PRM_P2, DSPVA_PERIPHERAL_PRM_P2, HW_PAGE_SIZE_4KB},

/* CM1 4KB */
{L4_PERIPHERAL_CM1, DSPVA_PERIPHERAL_CM1, HW_PAGE_SIZE_4KB},
/* CM2 8KB */
{L4_PERIPHERAL_CM2_P1, DSPVA_PERIPHERAL_CM2_P1, HW_PAGE_SIZE_4KB},
{L4_PERIPHERAL_CM2_P2, DSPVA_PERIPHERAL_CM2_P2, HW_PAGE_SIZE_4KB},

/* GP timer1 4KB */
{L4_PERIPHERAL_GPTIMER5, DSPVA_PERIPHERAL_GPTIMER5, HW_PAGE_SIZE_4KB},
/* GP timer6 4KB */
{L4_PERIPHERAL_GPTIMER6, DSPVA_PERIPHERAL_GPTIMER6, HW_PAGE_SIZE_4KB},
/* GP timer7 4KB */
{L4_PERIPHERAL_GPTIMER7, DSPVA_PERIPHERAL_GPTIMER7, HW_PAGE_SIZE_4KB},
/* GP timer8 4KB */
{L4_PERIPHERAL_GPTIMER8, DSPVA_PERIPHERAL_GPTIMER8, HW_PAGE_SIZE_4KB},
/* GPIO1 4KB */
{L4_PERIPHERAL_GPIO1, DSPVA_PERIPHERAL_GPIO1, HW_PAGE_SIZE_4KB},
/* GPIO2 4KB */
{L4_PERIPHERAL_GPIO2, DSPVA_PERIPHERAL_GPIO2, HW_PAGE_SIZE_4KB},
/* GPIO3 4KB */
{L4_PERIPHERAL_GPIO3, DSPVA_PERIPHERAL_GPIO3, HW_PAGE_SIZE_4KB},
/* GPIO4 4KB */
{L4_PERIPHERAL_GPIO4, DSPVA_PERIPHERAL_GPIO4, HW_PAGE_SIZE_4KB},
/* GPIO5 4KB */
{L4_PERIPHERAL_GPIO5, DSPVA_PERIPHERAL_GPIO5, HW_PAGE_SIZE_4KB},
/* Watch dog timer 4KB */
{L4_PERIPHERAL_WDT3, DSPVA_PERIPHERAL_WDT3, HW_PAGE_SIZE_4KB},
/* Mcbsp1 4KB */
{L4_PERIPHERAL_MCBSP1, DSPVA_PERIPHERAL_MCBSP1, HW_PAGE_SIZE_4KB},
/* Mcbsp2 4KB */
{L4_PERIPHERAL_MCBSP2, DSPVA_PERIPHERAL_MCBSP2, HW_PAGE_SIZE_4KB},
/* Mcbsp3 4KB */
{L4_PERIPHERAL_MCBSP3, DSPVA_PERIPHERAL_MCBSP3, HW_PAGE_SIZE_4KB},
/* Mcbsp4 4KB */
{L4_PERIPHERAL_MCBSP4, DSPVA_PERIPHERAL_MCBSP4, HW_PAGE_SIZE_4KB},
/* Mcbsp5 4KB */
{L4_PERIPHERAL_MCBSP5, DSPVA_PERIPHERAL_MCBSP5, HW_PAGE_SIZE_4KB},
/* UART1 4KB */
{L4_PERIPHERAL_UART1, DSPVA_PERIPHERAL_UART1, HW_PAGE_SIZE_4KB},
/* UART2 4KB */
{L4_PERIPHERAL_UART2, DSPVA_PERIPHERAL_UART2, HW_PAGE_SIZE_4KB},
/* UART3 4KB */
{L4_PERIPHERAL_UART3, DSPVA_PERIPHERAL_UART3, HW_PAGE_SIZE_4KB},
/* SPI1 4KB */
{L4_PERIPHERAL_MCSPI1, DSPVA_PERIPHERAL_MCSPI1, HW_PAGE_SIZE_4KB},
/* SPI2 4KB */
{L4_PERIPHERAL_MCSPI2, DSPVA_PERIPHERAL_MCSPI2, HW_PAGE_SIZE_4KB},

{L4_PERIPHERAL_NULL, DSPVA_PERIPHERAL_NULL}

};


/*
 *   15         10                  0
 *   ---------------------------------
 *  |0|0|1|0|0|0|c|c|c|i|i|i|i|i|i|i|
 *  ---------------------------------
 *  |  (class)  | (module specific) |
 *
 *  where  c -> Externel Clock Command: Clk & Autoidle Disable/Enable
 *  i -> External Clock ID Timers 5,6,7,8, McBSP1,2 and WDT3
 */

/* MBX_PM_CLK_IDMASK: DSP External clock id mask. */
#define MBX_PM_CLK_IDMASK   0x7F

/* MBX_PM_CLK_CMDSHIFT: DSP External clock command shift. */
#define MBX_PM_CLK_CMDSHIFT 7

/* MBX_PM_CLK_CMDMASK: DSP External clock command mask. */
#define MBX_PM_CLK_CMDMASK 7

/* MBX_PM_MAX_RESOURCES: CORE 1 Clock resources. */
#define MBX_CORE1_RESOURCES 7

/* MBX_PM_MAX_RESOURCES: CORE 2 Clock Resources. */
#define MBX_CORE2_RESOURCES 1

/* MBX_PM_MAX_RESOURCES: TOTAL Clock Reosurces. */
#define MBX_PM_MAX_RESOURCES 11

/*  Power Management Commands */
enum BPWR_ExtClockCmd {
	BPWR_DisableClock = 0,
	BPWR_EnableClock,
	BPWR_DisableAutoIdle,
	BPWR_EnableAutoIdle
} ;

/* OMAP242x specific resources */
enum BPWR_ExtClockId {
	BPWR_GPTimer5 = 0x10,
	BPWR_GPTimer6,
	BPWR_GPTimer7,
	BPWR_GPTimer8,
	BPWR_WDTimer3,
	BPWR_MCBSP1,
	BPWR_MCBSP2,
	BPWR_MCBSP3,
	BPWR_MCBSP4,
	BPWR_MCBSP5,
	BPWR_SSI = 0x20
} ;

static const u32 BPWR_CLKID[] = {
	(u32) BPWR_GPTimer5,
	(u32) BPWR_GPTimer6,
	(u32) BPWR_GPTimer7,
	(u32) BPWR_GPTimer8,
	(u32) BPWR_WDTimer3,
	(u32) BPWR_MCBSP1,
	(u32) BPWR_MCBSP2,
	(u32) BPWR_MCBSP3,
	(u32) BPWR_MCBSP4,
	(u32) BPWR_MCBSP5,
	(u32) BPWR_SSI
};

struct BPWR_Clk_t {
	u32 clkId;
	enum SERVICES_ClkId funClk;
	enum SERVICES_ClkId intClk;
} ;

static const struct BPWR_Clk_t BPWR_Clks[] = {
	{(u32) BPWR_GPTimer5, SERVICESCLK_gpt5_fck, SERVICESCLK_gpt5_ick},
	{(u32) BPWR_GPTimer6, SERVICESCLK_gpt6_fck, SERVICESCLK_gpt6_ick},
	{(u32) BPWR_GPTimer7, SERVICESCLK_gpt7_fck, SERVICESCLK_gpt7_ick},
	{(u32) BPWR_GPTimer8, SERVICESCLK_gpt8_fck, SERVICESCLK_gpt8_ick},
	{(u32) BPWR_WDTimer3, SERVICESCLK_wdt3_fck, SERVICESCLK_wdt3_ick},
	{(u32) BPWR_MCBSP1, SERVICESCLK_mcbsp1_fck, SERVICESCLK_mcbsp1_ick},
	{(u32) BPWR_MCBSP2, SERVICESCLK_mcbsp2_fck, SERVICESCLK_mcbsp2_ick},
	{(u32) BPWR_MCBSP3, SERVICESCLK_mcbsp3_fck, SERVICESCLK_mcbsp3_ick},
	{(u32) BPWR_MCBSP4, SERVICESCLK_mcbsp4_fck, SERVICESCLK_mcbsp4_ick},
	{(u32) BPWR_MCBSP5, SERVICESCLK_mcbsp5_fck, SERVICESCLK_mcbsp5_ick},
	{(u32) BPWR_SSI, SERVICESCLK_ssi_fck, SERVICESCLK_ssi_ick}
};

/* Interrupt Register Offsets */
#define INTH_IT_REG_OFFSET              0x00	/* Interrupt register offset  */
#define INTH_MASK_IT_REG_OFFSET         0x04	/* Mask Interrupt reg offset  */

#define   DSP_MAILBOX1_INT              10

/*
 *  INTH_InterruptKind_t
 *  Identify the kind of interrupt: either FIQ/IRQ
 */
enum INTH_InterruptKind_t {
	INTH_IRQ = 0,
	INTH_FIQ = 1
} ;

enum INTH_SensitiveEdge_t {
	FALLING_EDGE_SENSITIVE = 0,
	LOW_LEVEL_SENSITIVE = 1
} ;

/*
 *  Bit definition of  Interrupt  Level  Registers
 */

/* Mail Box defines */
#define MB_ARM2DSP1_REG_OFFSET          0x00

#define MB_ARM2DSP1B_REG_OFFSET         0x04

#define MB_DSP2ARM1B_REG_OFFSET         0x0C

#define MB_ARM2DSP1_FLAG_REG_OFFSET     0x18

#define MB_ARM2DSP_FLAG                 0x0001

#define MBOX_ARM2DSP HW_MBOX_ID_0
#define MBOX_DSP2ARM HW_MBOX_ID_1
#define MBOX_ARM HW_MBOX_U0_ARM
#define MBOX_DSP HW_MBOX_U1_DSP1

#define ENABLE                          true
#define DISABLE                         false

#define HIGH_LEVEL                      true
#define LOW_LEVEL                       false

/* Macro's */
#define REG16(A)    (*(REG_UWORD16 *)(A))

#define ClearBit(reg, mask)             (reg &= ~mask)
#define SetBit(reg, mask)               (reg |= mask)

#define SetGroupBits16(reg, position, width, value) \
	do {\
		reg &= ~((0xFFFF >> (16 - (width))) << (position)) ; \
		reg |= ((value & (0xFFFF >> (16 - (width)))) << (position)); \
	} while (0);

#define ClearBitIndex(reg, index)   (reg &= ~(1 << (index)))

/* This mini driver's device context: */
struct WMD_DEV_CONTEXT {
	struct DEV_OBJECT *hDevObject;	/* Handle to WCD device object. */
	u32 dwDspBaseAddr;	/* Arm's API to DSP virtual base addr */
	/*
	 * DSP External memory prog address as seen virtually by the OS on
	 * the host side.
	 */
	u32 dwDspExtBaseAddr;	/* See the comment above        */
	u32 dwAPIRegBase;	/* API memory mapped registers  */
	u32 dwDSPMmuBase;	/* DSP MMU Mapped registers     */
	u32 dwMailBoxBase;	/* Mail box mapped registers    */
	u32 dwAPIClkBase;	/* CLK Registers                */
	u32 dwDSPClkM2Base;	/* DSP Clock Module m2          */
	u32 dwPublicRhea;	/* Pub Rhea                     */
	u32 dwIntAddr;	/* MB INTR reg                  */
	u32 dwTCEndianism;	/* TC Endianism register        */
	u32 dwTestBase;	/* DSP MMU Mapped registers     */
	u32 dwSelfLoop;	/* Pointer to the selfloop      */
	u32 dwDSPStartAdd;	/* API Boot vector              */
	u32 dwInternalSize;	/* Internal memory size         */

	/*
	 * Processor specific info is set when prog loaded and read from DCD.
	 * [See WMD_BRD_Ctrl()]  PROC info contains DSP-MMU TLB entries.
	 */
	/* DMMU TLB entries */
	struct WMDIOCTL_EXTPROC aTLBEntry[WMDIOCTL_NUMOFMMUTLB];
	u32 dwBrdState;	/* Last known board state.      */
	u32 ulIntMask;	/* int mask                     */
	u16 ioBase;	/* Board I/O base               */
	u16 wIntrVal2Dsp;	/* MBX value to DSP. See mbx_sh.h */
	u32 numTLBEntries;	/* DSP MMU TLB entry counter    */
	u32 fixedTLBEntries;	/* Fixed DSPMMU TLB entry count */

	/* TC Settings */
	bool tcWordSwapOn;	/* Traffic Controller Word Swap */
	struct PgTableAttrs *pPtAttrs;
	u32 uDspPerClks;
} ;

	/*
	 * ======== WMD_TLB_DspVAToMpuPA ========
	 *     Given a DSP virtual address, traverse the page table and return
	 *     a corresponding MPU physical address and size.
	 */
extern DSP_STATUS WMD_TLB_DspVAToMpuPA(struct WMD_DEV_CONTEXT *pDevContext,
				       IN u32 ulVirtAddr,
				       OUT u32 *ulPhysAddr,
				       OUT u32 *sizeTlb);

#endif				/* _TIOMAP_ */

