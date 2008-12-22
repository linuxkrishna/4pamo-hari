/*
 * hw_prcm.c
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
 *  ======== hw_prcm.c ========
 *  Description:
 *      API definitions to configure PRCM (Power, Reset & Clocks Manager)
 *
 *! Revision History:
 *! ================
 *! 16 Feb 2003 sb: Initial version
 */


#include <GlobalTypes.h>
#include <PRCMRegAcM.h>
#include <hw_defs.h>
#include <hw_prcm.h>

static HW_STATUS HW_CLK_WriteVal (const u32 baseAddress, enum HW_ClkModule_t c, enum HW_SetClear_t val);
static HW_STATUS HW_CLK_AutoIdleWriteVal (const u32 baseAddress, enum HW_ClkModule_t c, enum HW_SetClear_t val);
static HW_STATUS HW_RST_WriteVal (const u32 baseAddress, enum HW_RstModule_t r, enum HW_SetClear_t val);

/* ============================================================================
* EXPORTED FUNCTIONS
* =============================================================================
*/

HW_STATUS HW_CLK_Enable(const u32 baseAddress, enum HW_ClkModule_t c)
{
	return HW_CLK_WriteVal(baseAddress, c, HW_SET);
}

HW_STATUS HW_CLK_Disable(const u32 baseAddress, enum HW_ClkModule_t c)
{
	return HW_CLK_WriteVal(baseAddress, c, HW_CLEAR);
}

static HW_STATUS HW_CLK_WriteVal (const u32 baseAddress, enum HW_ClkModule_t c, enum HW_SetClear_t val)
{
	HW_STATUS status = RET_OK;
	u32 val_clk = HW_CLEAR;

	if (val == HW_SET)
		val_clk = 0x2;


	switch (c) {
	case HW_CLK_TESLA:
	CM_TESLA_TESLA_CLKCTRLWriteRegister32(baseAddress, val);
	break;
	case HW_CLK_MCBSP1:
	CM_ABEEN_MCBSP1Write32(baseAddress, val_clk);
	break;
	case HW_CLK_MCBSP2:
	CM_ABEEN_MCBSP2Write32(baseAddress, val_clk);
	break;
	case HW_CLK_MCBSP3:
	CM_ABEEN_MCBSP3Write32(baseAddress, val_clk);
	break;
	case HW_CLK_MCBSP4:
	CM_L4PEREN_MCBSP4Write32(baseAddress, val_clk);
	break;
	case HW_CLK_MCBSP5:
	CM_L4PEREN_MCBSP5Write32(baseAddress, val_clk);
	break;
	case HW_CLK_TIMER5:
	CM_ABEEN_TIMER5Write32(baseAddress, val_clk);
	break;
	case HW_CLK_TIMER6:
	CM_ABEEN_TIMER6Write32(baseAddress, val_clk);
	break;
	case HW_CLK_TIMER7:
	CM_ABEEN_TIMER7Write32(baseAddress, val_clk);
	break;
	case HW_CLK_TIMER8:
	CM_ABEEN_TIMER8Write32(baseAddress, val_clk);
	break;
	case HW_CLK_DMTIMER2:
	CM_L4PEREN_DMTIMER2Write32(baseAddress, val_clk);
	break;
	case HW_CLK_DMTIMER3:
	CM_L4PEREN_DMTIMER3Write32(baseAddress, val_clk);
	break;
	case HW_CLK_DMTIMER4:
	CM_L4PEREN_DMTIMER4Write32(baseAddress, val_clk);
	break;
	case HW_CLK_DMTIMER9:
	CM_L4PEREN_DMTIMER9Write32(baseAddress, val_clk);
	break;
	case HW_CLK_DMTIMER10:
	CM_L4PEREN_DMTIMER10Write32(baseAddress, val_clk);
	break;
	case HW_CLK_DMTIMER11:
	CM_L4PEREN_DMTIMER11Write32(baseAddress, val_clk);
	break;

	default:
	status = RET_FAIL;
	break;
	}

	return status;
}



HW_STATUS HW_CLK_AutoIdleEnable(const u32 baseAddress, enum HW_ClkModule_t c)
{
	return HW_CLK_AutoIdleWriteVal(baseAddress, c, HW_SET);
}

HW_STATUS HW_CLK_AutoIdleDisable(const u32 baseAddress, enum  HW_ClkModule_t c)
{
	return HW_CLK_AutoIdleWriteVal(baseAddress, c, HW_CLEAR);
}

static HW_STATUS HW_CLK_AutoIdleWriteVal(const u32 baseAddress, enum HW_ClkModule_t c,
				enum HW_SetClear_t val)
{
	HW_STATUS status = RET_OK;

	switch (c) {

	default:
	status = RET_FAIL;
	break;
	}

	return status;
}

HW_STATUS HW_CLK_StbyStatus(const u32 baseAddress, enum HW_SetClear_t *stbyState)
{
	HW_STATUS status = RET_OK;

	*stbyState = (enum HW_SetClear_t)CM_TESLA_STBYST_Read32(baseAddress);

	return status;
}


HW_STATUS HW_CLK_IdleStatus(const u32 baseAddress, enum HW_IdleStatus_t *idleState)
{
	HW_STATUS status = RET_OK;

	*idleState = (enum HW_IdleStatus_t)CM_TESLA_IDLEST_Read32(baseAddress);

	return status;
}


HW_STATUS HW_RST_Reset(const u32 baseAddress, enum HW_RstModule_t r)
{
	return HW_RST_WriteVal(baseAddress, r, HW_SET);
}

HW_STATUS HW_RST_UnReset(const u32 baseAddress, enum HW_RstModule_t r)
{
	return HW_RST_WriteVal(baseAddress, r, HW_CLEAR);
}

HW_STATUS HW_RSTCTRL_RegGet(const u32 baseAddress, u32 *value)
{
	HW_STATUS status = RET_OK;

	*value = PRM_TESLA_RSTCTRLReadRegister32(baseAddress);

	return status;
}

static HW_STATUS HW_RST_WriteVal(const u32 baseAddress, enum HW_RstModule_t p,
				enum HW_SetClear_t value)
{
	HW_STATUS status = RET_OK;

	switch (p) {
	case HW_RST1_TESLA:
	PRM_TESLA_RSTCTRL_RST1_Write32(baseAddress, value);
	break;
	case HW_RST2_TESLA:
	PRM_TESLA_RSTCTRL_RST2_Write32(baseAddress, value);
	break;

	default:
	status = RET_FAIL;
	break;
	}

	return status;
}

HW_STATUS HW_RSTST_RegGet (const u32 baseAddress, u32 *value)
{
	HW_STATUS status = RET_OK;

	*value = PRM_TESLA_RSTSTReadRegister32(baseAddress);

	return status;
}


HW_STATUS HW_RSTST_RegClear (const u32 baseAddress)
{
	HW_STATUS status = RET_OK;

	PRM_TESLA_RSTST_Clear32(baseAddress);

	return status;
}


HW_STATUS HW_PWRSTCTRL_RegGet(const u32 baseAddress, u32 *value)
{
	HW_STATUS status = RET_OK;

	*value = PRM_TESLA_PWRSTCTRLReadRegister32(baseAddress);

	return status;
}

HW_STATUS HW_PWR_PowerStateGet(const u32 baseAddress,
	enum HW_PwrModule_t p, enum  HW_PwrState_t *value)
{
	HW_STATUS status = RET_OK;
	switch (p) {
	case HW_PWR_DOMAIN_TESLA:
	*value = (enum HW_PwrState_t)PRM_TESLA_PWRSTSTGet32(baseAddress);
	break;

	default:
	status = RET_FAIL;
	break;
	}

	return status;
}

HW_STATUS HW_PWR_PowerStateSet(const u32 baseAddress,
	enum HW_PwrModule_t p, enum HW_PwrState_t value)
{
	HW_STATUS status = RET_OK;

	switch (p) {
	case HW_PWR_DOMAIN_TESLA:
		switch (value) {
		case HW_PWR_STATE_ON:
		PRM_TESLA_PWRSTCTRLWriteON32(baseAddress);
		break;
		case HW_PWR_STATE_INACT:
		PRM_TESLA_PWRSTCTRLWriteINACTIVE32(baseAddress);
		break;
		case HW_PWR_STATE_RET:
		PRM_TESLA_PWRSTCTRLWriteRET32(baseAddress);
		break;
		case HW_PWR_STATE_OFF:
		PRM_TESLA_PWRSTCTRLWriteOFF32(baseAddress);
		break;
		default:
		status = RET_FAIL;
		break;
		}
	break;
	case HW_PWR_DOMAIN_CORE:
		switch (value) {
		case HW_PWR_STATE_ON:
		case HW_PWR_STATE_INACT:
		case HW_PWR_STATE_RET:
		case HW_PWR_STATE_OFF:
		PRM_CORE_PWRSTCTRLWrite32(baseAddress, value);
		break;
		default:
		status = RET_FAIL;
		break;
		}
	break;
	default:
	status = RET_FAIL;
	break;
	}

	return status;
}


HW_STATUS HW_PWR_RetentionStateSet(const u32 baseAddress,
	enum HW_PwrModule_t p, enum  HW_RetState_t src, enum HW_SetClear_t  value)
{
	HW_STATUS status = RET_OK;

	switch (p) {
	case HW_PWR_DOMAIN_TESLA:
		switch (src) {
		case HW_RETSTATE_LOGIC:
		PRM_TESLA_PWRSTCTRLWriteLOGICRETSTATE32(baseAddress, value);
		break;
		case HW_RETSTATE_L1:
		PRM_TESLA_PWRSTCTRLWriteL1RETSTATE32(baseAddress, value);
		break;
		case HW_RETSTATE_L2:
		PRM_TESLA_PWRSTCTRLWriteL2RETSTATE32(baseAddress, value);
		break;
		default:
		status = RET_FAIL;
		break;
		}
	break;

	default:
	status = RET_FAIL;
	break;
	}

	return status;
}


HW_STATUS HW_PWRST_RegGet(const u32 baseAddress, enum HW_PwrModule_t p, u32 *value)
{
	HW_STATUS status = RET_OK;
	switch (p) {
	case HW_PWR_DOMAIN_TESLA:
	*value = PRM_TESLA_PWRSTSTReadRegister32(baseAddress);
	break;

	default:
	status = RET_FAIL;
	break;
	}

	return status;
}

HW_STATUS HW_PWR_WkupDependency_RegGet(const u32 baseAddress, u32 *value)
{
	HW_STATUS status = RET_OK;

	*value = PRM_WKUP_IVA_ReadRegister32(baseAddress);

	return status;
}

HW_STATUS HW_PWR_WkupDependencySet(const u32 baseAddress, enum HW_PwrModule_t p,
	enum HW_WeakUpDep_t src, enum HW_SetClear_t value)
{
	HW_STATUS status = RET_OK;
	switch (p) {
	case HW_PWR_DOMAIN_TESLA:
		switch (src) {
		case HW_WKUP_IVA_MPU:
		PRM_WKUP_IVA_MPU_Write32(baseAddress, value);
		break;
		case HW_WKUP_IVA_DUCATI:
		PRM_WKUP_IVA_DUCATI_Write32(baseAddress, value);
		break;
		default:
		status = RET_FAIL;
		break;
		}
	break;

	default:
	status = RET_FAIL;
	break;
	}

	return status;
}

HW_STATUS HW_PWR_ForceStateSet(const u32 baseAddress, enum HW_PwrModule_t p,
	enum HW_PWR_TransState_t value)
{
	HW_STATUS status = RET_OK;

	switch (p) {
	case HW_PWR_DOMAIN_TESLA:
	CM_CLKSTCTRL_TESLAWriteRegister32(baseAddress, value);
	break;
	case HW_PWR_DOMAIN_ABE:
	CM_CLKSTCTRL_ABEWriteRegister32(baseAddress, value);
	break;
	case HW_PWR_DOMAIN_L4PER:
	CM_CLKSTCTRL_L4PERWriteRegister32(baseAddress, value);
	break;
	default:
	status = RET_FAIL;
	break;
	}

	return status;
}


HW_STATUS HW_TESLA_CONTEXT_RegGet(const u32 baseAddress, u32 *value)
{
	HW_STATUS status = RET_OK;

	*value = PRM_TESLA_CONTEXTReadRegister32(baseAddress);

	return status;
}

HW_STATUS HW_TESLA_CONTEXT_ClrSet(const u32 baseAddress)
{
	HW_STATUS status = RET_OK;

	PRM_TESLA_CONTEXT_Clear32(baseAddress);

	return status;
}


HW_STATUS HW_ALWON_CONTEXT_RegGet(const u32 baseAddress, u32 *value)
{
	HW_STATUS status = RET_OK;

	*value = PRM_ALWON_CONTEXTReadRegister32(baseAddress);

	return status;
}


HW_STATUS HW_IVA_DVFSSet(const u32 baseAddress, enum HW_IvaDVFS_t src, u32 value)
{
	HW_STATUS status = RET_OK;
	switch (src) {
	case HW_DVFS_PERF_TESLA:
	CM_IVA_DVFS_PERFTESTLA_Write32(baseAddress, value);
	break;
	case HW_DVFS_PERF_IVAHD:
	CM_IVA_DVFS_PERFIVAHD_Write32(baseAddress, value);
	break;
	case HW_DVFS_PERF_ABE:
	CM_IVA_DVFS_PERFABE_Write32(baseAddress, value);
	break;
	default:
	status = RET_FAIL;
	break;
	}
	return status;
}

HW_STATUS HW_IVA_DVFS_RegGet(const u32 baseAddress, enum HW_IvaDVFS_t src, u32 *value)
{
	HW_STATUS status = RET_OK;
	switch (src) {
	case HW_DVFS_PERF_TESLA:
	*value = CM_IVA_DVFS_PERFTESTLA_Read32(baseAddress);
	break;
	case HW_DVFS_PERF_IVAHD:
	*value = CM_IVA_DVFS_PERFIVAHD_Read32(baseAddress);
	break;
	case HW_DVFS_PERF_ABE:
	*value = CM_IVA_DVFS_PERFABE_Read32(baseAddress);
	break;
	case HW_DVFS_RESULT:
	*value = CM_IVA_DVFS_RESULT_Read32(baseAddress);
	break;
	case HW_DVFS_CURRENT:
	*value = CM_IVA_DVFS_CURRENT_Read32(baseAddress);
	break;
	default:
	status = RET_FAIL;
	break;
	}
	return status;

}


