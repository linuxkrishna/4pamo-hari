/*
 * hw_prcm.h
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
 *  ======== hw_prcm.h ========
 *  Description:
 *      PRCM types and API declarations
 *
 *! Revision History:
 *! ================
 *! 16 Feb 2003 sb: Initial version
 */


#ifndef __HW_PRCM_H
#define __HW_PRCM_H

/* ============================================================================
* INCLUDE FILES (only if necessary)
* =============================================================================
*/
#include <linux/types.h>

/* ----------------------------------------------------------------------------
* TYPE:         HW_ClkModule
*
* DESCRIPTION:  Enumerated Type used to specify the clock domain
*
* -----------------------------------------------------------------------------
*/

enum HW_ClkModule_t {

/*CM1 ABE*/
	HW_CLK_TESLA,
	HW_CLK_AESS,
	HW_CLK_PDM,
	HW_CLK_DMIC,
	HW_CLK_MCASP,
	HW_CLK_MCBSP1,
	HW_CLK_MCBSP2,
	HW_CLK_MCBSP3,
	HW_CLK_SLIMBUS,
	HW_CLK_TIMER5,
	HW_CLK_TIMER6,
	HW_CLK_TIMER7,
	HW_CLK_TIMER8,
	HW_CLK_WDT3,

/*CM2 L4PER*/

	HW_CLK_DMTIMER2,
	HW_CLK_DMTIMER3,
	HW_CLK_DMTIMER4,
	HW_CLK_DMTIMER9,
	HW_CLK_DMTIMER10,
	HW_CLK_DMTIMER11,
	HW_CLK_MCBSP4,
	HW_CLK_MCBSP5,


};



/* ----------------------------------------------------------------------------
* TYPE:         HW_RstModule
*
* DESCRIPTION:  Enumerated Type used to specify the module to be reset
*
* -----------------------------------------------------------------------------
*/
enum HW_RstModule_t {
	HW_RST1_TESLA,  /*Reset the DSP*/
	HW_RST2_TESLA,  /* Reset MMU */
};



/* ----------------------------------------------------------------------------
* TYPE:         HW_PwrModule
*
* DESCRIPTION:  Enumerated Type used to specify the power domain
*
* -----------------------------------------------------------------------------
*/

enum HW_PwrModule_t {
/* Domains*/
	HW_PWR_DOMAIN_CORE,
	HW_PWR_DOMAIN_MPU,
	HW_PWR_DOMAIN_WAKEUP,
	HW_PWR_DOMAIN_TESLA,
	HW_PWR_DOMAIN_ABE,
	HW_PWR_DOMAIN_L4PER,
/* Sub-domains */
	HW_PWR_DSP_IPI,        /* IPI = Intrusive Port Interface */
	HW_PWR_IVA_ISP         /* ISP = Intrusive Slave Port*/
} ;


enum HW_PwrState_t {
	HW_PWR_STATE_OFF,
	HW_PWR_STATE_RET,
	HW_PWR_STATE_INACT,
	HW_PWR_STATE_ON = 3
};

enum HW_RetState_t {
	HW_RETSTATE_LOGIC,
	HW_RETSTATE_L1,
	HW_RETSTATE_L2
};

enum HW_WeakUpDep_t {
	HW_WKUP_IVA_MPU,
	HW_WKUP_IVA_DUCATI
};

enum HW_IvaDVFS_t {
	HW_DVFS_PERF_TESLA,
	HW_DVFS_PERF_IVAHD,
	HW_DVFS_PERF_ABE,
	HW_DVFS_RESULT,
	HW_DVFS_CURRENT
};

enum HW_PWR_TransState_t {
	HW_AUTOTRANS_DIS = 0x0,
	HW_SW_SUP_SLEEP,
	HW_SW_SUP_WAKEUP,
	HW_AUTOTRANS_EN
};

enum HW_IdleStatus_t {
	HW_TESLA_FULLFUNC = 0x0, /* Module is fully functional, including OCP*/
	HW_TESLA_TRANSITIONING, /* Transitioning (weakup, sleep or sleep abortion)*/
	HW_TESLA_IDLE,/* Idle mode(only OCP)*/
	HW_TESLA_DISABLED/*Module is disabled and cannot be accessed*/
};


extern HW_STATUS HW_CLK_Enable(const u32 baseAddress,
				enum HW_ClkModule_t c);

extern HW_STATUS HW_CLK_Disable(const u32 baseAddress,
				enum HW_ClkModule_t c);

extern HW_STATUS HW_CLK_AutoIdleEnable(const u32 baseAddress,
				enum HW_ClkModule_t c);

extern HW_STATUS HW_CLK_AutoIdleDisable(const u32 baseAddress,
				enum HW_ClkModule_t c);

extern HW_STATUS HW_CLK_StbyStatus(const u32 baseAddress,  enum HW_SetClear_t *stbyState);

extern HW_STATUS HW_CLK_IdleStatus(const u32 baseAddress,  enum HW_IdleStatus_t *idleState);

extern HW_STATUS HW_RST_Reset(const u32 baseAddress,
				enum HW_RstModule_t r);

extern HW_STATUS HW_RST_UnReset (const u32 baseAddress,
				enum HW_RstModule_t r);

extern HW_STATUS HW_RSTCTRL_RegGet(const u32 baseAddress, u32 *value);

extern HW_STATUS HW_RSTST_RegGet(const u32 baseAddress, u32 *value);

extern HW_STATUS HW_RSTST_RegClear(const u32 baseAddress);

extern HW_STATUS HW_PWRSTCTRL_RegGet(const u32 baseAddress, u32 *value);

extern HW_STATUS HW_PWR_PowerStateGet(const u32 baseAddress,
				enum HW_PwrModule_t p, enum HW_PwrState_t *value);

extern HW_STATUS HW_PWR_PowerStateSet(const u32 baseAddress, enum HW_PwrModule_t p,
				enum HW_PwrState_t value);

extern HW_STATUS HW_PWR_RetentionStateSet(const u32 baseAddress,
				enum HW_PwrModule_t p, enum HW_RetState_t src, enum HW_SetClear_t  value);

extern HW_STATUS HW_PWRST_RegGet(const u32 baseAddress, enum HW_PwrModule_t p, u32 *value);

extern HW_STATUS HW_PWR_WkupDependency_RegGet(const u32 baseAddress, u32 *value);

extern HW_STATUS HW_PWR_WkupDependencySet(const u32 baseAddress, enum HW_PwrModule_t p,
				enum HW_WeakUpDep_t src, enum HW_SetClear_t value);

extern HW_STATUS HW_PWR_ForceStateSet(const u32 baseAddress, enum HW_PwrModule_t p,
				enum HW_PWR_TransState_t value);

extern HW_STATUS HW_TESLA_RST_WriteVal(const u32 baseAddress, enum HW_RstModule_t p,
				enum HW_SetClear_t value);

extern HW_STATUS HW_TESLA_CONTEXT_RegGet(const u32 baseAddress, u32 *value);

extern HW_STATUS HW_TESLA_CONTEXT_ClrSet(const u32 baseAddress);

extern HW_STATUS HW_ALWON_CONTEXT_RegGet(const u32 baseAddress, u32 *value);

extern HW_STATUS HW_IVA_DVFSSet(const u32 baseAddress, enum HW_IvaDVFS_t src, u32 value);

extern HW_STATUS HW_IVA_DVFS_RegGet(const u32 baseAddress, enum HW_IvaDVFS_t src, u32 *value);


#endif  /* __HW_PRCM_H */

