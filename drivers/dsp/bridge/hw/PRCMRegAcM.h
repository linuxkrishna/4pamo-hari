/*
 * PRCMRegAcM.h
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


#ifndef _PRCM_REG_ACM_H
#define _PRCM_REG_ACM_H


#include "GlobalTypes.h"
#include "PRCMAccInt.h"


/*****************************************************************************
* EXPORTED DEFINITIONS
******************************************************************************
*/

#if defined(USE_LEVEL_1_MACROS)


/********************************************************************/

#define PRM_TESLA_PWRSTCTRLReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+PRM_PM_TESLA_PWRSTCTRL_OFFSET))

/********************************************************************/

#define PRM_TESLA_PWRSTCTRLWriteON32(baseAddress)\
{\
	const u32 offset = PRM_PM_TESLA_PWRSTCTRL_OFFSET;\
	const u32 newValue = (u32)PM_TESLA_PWRSTCTRLPowerStateON <<\
	PM_TESLA_PWRSTCTRL_PowerState_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((u32)(baseAddress)+offset);\
	data &= ~(PM_TESLA_PWRSTCTRL_PowerState_MASK);\
	data |= newValue;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, data);\
}

/********************************************************************/

#define PRM_TESLA_PWRSTCTRLWriteINACTIVE32(baseAddress)\
{\
	const u32 offset = PRM_PM_TESLA_PWRSTCTRL_OFFSET;\
	const u32 newValue = (u32)PM_TESLA_PWRSTCTRLPowerStateINACTIVE <<\
	PM_TESLA_PWRSTCTRL_PowerState_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((u32)(baseAddress)+offset);\
	data &= ~(PM_TESLA_PWRSTCTRL_PowerState_MASK);\
	data |= newValue;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, data);\
}

/********************************************************************/

#define PRM_TESLA_PWRSTCTRLWriteRET32(baseAddress)\
{\
	const u32 offset = PRM_PM_TESLA_PWRSTCTRL_OFFSET;\
	const u32 newValue = (u32)PM_TESLA_PWRSTCTRLPowerStateRET <<\
	PM_TESLA_PWRSTCTRL_PowerState_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((u32)(baseAddress)+offset);\
	data &= ~(PM_TESLA_PWRSTCTRL_PowerState_MASK);\
	data |= newValue;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, data);\
}

/********************************************************************/

#define PRM_TESLA_PWRSTCTRLWriteOFF32(baseAddress)\
{\
	const u32 offset = PRM_PM_TESLA_PWRSTCTRL_OFFSET;\
	const u32 newValue = (u32)PM_TESLA_PWRSTCTRLPowerStateOFF <<\
	PM_TESLA_PWRSTCTRL_PowerState_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((u32)(baseAddress)+offset);\
	data &= ~(PM_TESLA_PWRSTCTRL_PowerState_MASK);\
	data |= newValue;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, data);\
}

/********************************************************************/

#define PRM_TESLA_PWRSTCTRLWriteLOGICRETSTATE32(baseAddress, value)\
{\
	const u32 offset = PRM_PM_TESLA_PWRSTCTRL_OFFSET;\
	register u32 data = \
	RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(PM_TESLA_PWRSTCTRL_LogicRetState_MASK);\
	newValue <<= PM_TESLA_PWRSTCTRL_LogicRetState_OFFSET;\
	newValue &= PM_TESLA_PWRSTCTRL_LogicRetState_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define PRM_TESLA_PWRSTCTRLWriteL1RETSTATE32(baseAddress, value)\
{\
	const u32 offset = PRM_PM_TESLA_PWRSTCTRL_OFFSET;\
	register u32 data = \
	RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(PM_TESLA_PWRSTCTRL_L1RetState_MASK);\
	newValue <<= PM_TESLA_PWRSTCTRL_L1RetState_OFFSET;\
	newValue &= PM_TESLA_PWRSTCTRL_L1RetState_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define PRM_TESLA_PWRSTCTRLWriteL2RETSTATE32(baseAddress, value)\
{\
	const u32 offset = PRM_PM_TESLA_PWRSTCTRL_OFFSET;\
	register u32 data = \
	RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(PM_TESLA_PWRSTCTRL_L2RetState_MASK);\
	newValue <<= PM_TESLA_PWRSTCTRL_L2RetState_OFFSET;\
	newValue  &= PM_TESLA_PWRSTCTRL_L2RetState_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}


/********************************************************************/


#define PRM_TESLA_PWRSTSTReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+PRM_PM_TESLA_PWRSTST_OFFSET))

/********************************************************************/


#define PRM_TESLA_PWRSTSTGet32(baseAddress)\
	(((RD_MEM_32_VOLATILE(((u32)(baseAddress))+PRM_PM_TESLA_PWRSTST_OFFSET))&\
	PM_TESLA_PWRSTST_PowerState_MASK) >>\
	PM_TESLA_PWRSTST_PowerState_OFFSET)

/********************************************************************/

#define PRM_CORE_PWRSTCTRLWrite32(baseAddress, value)\
{\
	const u32 offset = PRM_PM_CORE_PWRSTCTRL_OFFSET;\
	register u32 data = \
	RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(PRM_PM_CORE_PWRSTCTRL_PowerControl_MASK);\
	newValue <<= PRM_PM_CORE_PWRSTCTRL_PowerControl_OFFSET;\
	newValue &= PRM_PM_CORE_PWRSTCTRL_PowerControl_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}


#define PRM_TESLA_RSTCTRLReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+PRM_RM_TESLA_RSTCTRL_OFFSET))

#define PRM_TESLA_RSTCTRL_RST1_Write32(baseAddress, value)\
{\
	const u32 offset = PRM_RM_TESLA_RSTCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(RM_TESLA_RSTCTRL_RST1_MASK);\
	newValue <<= RM_TESLA_RSTCTRL_RST1_OFFSET;\
	newValue &= RM_TESLA_RSTCTRL_RST1_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define PRM_TESLA_RSTCTRL_RST2_Write32(baseAddress, value)\
{\
	const u32 offset = PRM_RM_TESLA_RSTCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(RM_TESLA_RSTCTRL_RST2_MASK);\
	newValue <<= RM_TESLA_RSTCTRL_RST2_OFFSET;\
	newValue &= RM_TESLA_RSTCTRL_RST2_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}


/********************************************************************/

#define PRM_TESLA_RSTSTReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+PRM_RM_TESLA_RSTST_OFFSET))

/********************************************************************/

#define PRM_TESLA_RSTST_Clear32(baseAddress)\
{\
	const u32 offset = PRM_RM_TESLA_RSTST_OFFSET;\
	register u32 newValue = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	newValue &= ~(RM_TESLA_RSTST_Clear_MASK);\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define PRM_TESLA_CONTEXTReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+PRM_RM_TESLA_TESLA_CONTEXT_OFFSET))

/********************************************************************/

#define PRM_TESLA_CONTEXT_Clear32(baseAddress)\
{\
	const u32 offset = PRM_RM_TESLA_TESLA_CONTEXT_OFFSET;\
	register u32 newValue = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	newValue &= ~(RM_TESLA_TESLA_CONTEXT_Clear_MASK);\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/


#define PRM_WKUP_IVA_ReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+PRM_PM_ALWON_SR_IVA_WKDEP_OFFSET))


/********************************************************************/

#define PRM_WKUP_IVA_MPU_Write32(baseAddress, value)\
{\
	const u32 offset = PRM_PM_ALWON_SR_IVA_WKDEP_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(PM_ALWON_SR_IVA_WKDEP_MPU_MASK);\
	newValue <<= PM_ALWON_SR_IVA_WKDEP_MPU_OFFSET;\
	newValue &= PM_ALWON_SR_IVA_WKDEP_MPU_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define PRM_WKUP_IVA_DUCATI_Write32(baseAddress, value)\
{\
	const u32 offset = PRM_PM_ALWON_SR_IVA_WKDEP_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(PM_ALWON_SR_IVA_WKDEP_DUCATI_MASK);\
	newValue <<= PM_ALWON_SR_IVA_WKDEP_DUCATI_OFFSET;\
	newValue &= PM_ALWON_SR_IVA_WKDEP_DUCATI_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define PRM_ALWON_CONTEXTReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+PRM_RM_ALWON_SR_IVA_CONTEXT_OFFSET))


/**********************************************************************/

#define CM_CLKSTCTRL_TESLAWriteRegister32(baseAddress, value)\
{\
	const u32 offset = CM1_CM_TESLA_CLKSTCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM_TESLA_CLKSTCTRL_Transition_MASK);\
	newValue <<= CM_TESLA_CLKSTCTRL_Transition_OFFSET;\
	newValue &= CM_TESLA_CLKSTCTRL_Transition_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}


/**********************************************************************/

#define CM_TESLA_TESLA_CLKCTRLWriteRegister32(baseAddress, value)\
{\
	const u32 offset = CM1_CM_TESLA_TESLA_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM_TESLA_TESLA_CLKCTRL_MODMODE_MASK);\
	newValue <<= CM_TESLA_TESLA_CLKCTRL_MODMODE_OFFSET;\
	newValue &= CM_TESLA_TESLA_CLKCTRL_MODMODE_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_TESLA_STBYST_Read32(baseAddress)\
	(((RD_MEM_32_VOLATILE(((u32)(baseAddress))+CM1_CM_TESLA_TESLA_CLKCTRL_OFFSET))&\
	CM_TESLA_TESLA_CLKCTRL_STBY_MASK) >>\
	CM_TESLA_TESLA_CLKCTRL_STBY_OFFSET)

/********************************************************************/


#define CM_TESLA_IDLEST_Read32(baseAddress)\
	(((RD_MEM_32_VOLATILE(((u32)(baseAddress))+CM1_CM_TESLA_TESLA_CLKCTRL_OFFSET))&\
	CM_TESLA_TESLA_CLKCTRL_IDLE_MASK) >>\
	CM_TESLA_TESLA_CLKCTRL_IDLE_OFFSET)


/********************************************************************/

#define CM_IVA_DVFS_PERFTESTLA_Read32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+CM2_CM_IVA_DVFS_PERF_TESLA_OFFSET))


/********************************************************************/

#define CM_IVA_DVFS_PERFTESTLA_Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_IVA_DVFS_PERF_TESLA_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM_IVA_DVFS_PERF_MASK);\
	newValue <<= CM_IVA_DVFS_PERF_OFFSET;\
	newValue &= CM_IVA_DVFS_PERF_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_IVA_DVFS_PERFIVAHD_Read32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+CM2_CM_IVA_DVFS_PERF_IVAHD_OFFSET))

/********************************************************************/

#define CM_IVA_DVFS_PERFIVAHD_Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_IVA_DVFS_PERF_IVAHD_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM_IVA_DVFS_PERF_MASK);\
	newValue <<= CM_IVA_DVFS_PERF_OFFSET;\
	newValue &= CM_IVA_DVFS_PERF_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_IVA_DVFS_PERFABE_Read32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+CM2_CM_IVA_DVFS_PERF_ABE_OFFSET))

/********************************************************************/

#define CM_IVA_DVFS_PERFABE_Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_IVA_DVFS_PERF_ABE_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM_IVA_DVFS_PERF_MASK);\
	newValue <<= CM_IVA_DVFS_PERF_OFFSET;\
	newValue &= CM_IVA_DVFS_PERF_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_IVA_DVFS_RESULT_Read32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+CM2_CM_IVA_DVFS_RESULT_OFFSET))

/********************************************************************/

#define CM_IVA_DVFS_CURRENT_Read32(baseAddress)\
	(RD_MEM_32_VOLATILE(((u32)(baseAddress))+CM2_CM_IVA_DVFS_CURRENT_OFFSET))


/**********************************************************************/

#define CM_CLKSTCTRL_ABEWriteRegister32(baseAddress, value)\
{\
	const u32 offset = CM1_CM1_ABE_CLKSTCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_ABE_CLKSTCTRL_Transition_MASK);\
	newValue <<= CM1_ABE_CLKSTCTRL_Transition_OFFSET;\
	newValue &= CM1_ABE_CLKSTCTRL_Transition_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_ABEEN_MCBSP1Write32(baseAddress, value)\
{\
	const u32 offset = CM1_CM1_ABE_MCBSP1_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_ABE_CLKCTRL_MASK);\
	newValue <<= CM1_ABE_CLKCTRL_OFFSET;\
	newValue &= CM1_ABE_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/


#define CM_ABEEN_MCBSP2Write32(baseAddress, value)\
{\
	const u32 offset = CM1_CM1_ABE_MCBSP2_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_ABE_CLKCTRL_MASK);\
	newValue <<= CM1_ABE_CLKCTRL_OFFSET;\
	newValue &= CM1_ABE_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/


#define CM_ABEEN_MCBSP3Write32(baseAddress, value)\
{\
	const u32 offset = CM1_CM1_ABE_MCBSP3_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_ABE_CLKCTRL_MASK);\
	newValue <<= CM1_ABE_CLKCTRL_OFFSET;\
	newValue &= CM1_ABE_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_ABEEN_TIMER5Write32(baseAddress, value)\
{\
	const u32 offset = CM1_CM1_ABE_TIMER5_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_ABE_CLKCTRL_MASK);\
	newValue <<= CM1_ABE_CLKCTRL_OFFSET;\
	newValue &= CM1_ABE_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_ABEEN_TIMER6Write32(baseAddress, value)\
{\
	const u32 offset = CM1_CM1_ABE_TIMER6_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_ABE_CLKCTRL_MASK);\
	newValue <<= CM1_ABE_CLKCTRL_OFFSET;\
	newValue &= CM1_ABE_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_ABEEN_TIMER7Write32(baseAddress, value)\
{\
	const u32 offset = CM1_CM1_ABE_TIMER7_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_ABE_CLKCTRL_MASK);\
	newValue <<= CM1_ABE_CLKCTRL_OFFSET;\
	newValue &= CM1_ABE_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_ABEEN_TIMER8Write32(baseAddress, value)\
{\
	const u32 offset = CM1_CM1_ABE_TIMER8_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_ABE_CLKCTRL_MASK);\
	newValue <<= CM1_ABE_CLKCTRL_OFFSET;\
	newValue &= CM1_ABE_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/**********************************************************************/

#define CM_CLKSTCTRL_L4PERWriteRegister32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_L4PER_CLKSTCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_L4PER_CLKSTCTRL_Transition_MASK);\
	newValue <<= CM1_L4PER_CLKSTCTRL_Transition_OFFSET;\
	newValue &= CM1_L4PER_CLKSTCTRL_Transition_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_L4PEREN_MCBSP4Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_L4PER_MCBSP4_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_L4PER_CLKCTRL_MASK);\
	newValue <<= CM1_L4PER_CLKCTRL_OFFSET;\
	newValue &= CM1_L4PER_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_L4PEREN_MCBSP5Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_L4PER_MCBSP5_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_L4PER_CLKCTRL_MASK);\
	newValue <<= CM1_L4PER_CLKCTRL_OFFSET;\
	newValue &= CM1_L4PER_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_L4PEREN_DMTIMER2Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_L4PER_DMTIMER2_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_L4PER_CLKCTRL_MASK);\
	newValue <<= CM1_L4PER_CLKCTRL_OFFSET;\
	newValue &= CM1_L4PER_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_L4PEREN_DMTIMER3Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_L4PER_DMTIMER3_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_L4PER_CLKCTRL_MASK);\
	newValue <<= CM1_L4PER_CLKCTRL_OFFSET;\
	newValue &= CM1_L4PER_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_L4PEREN_DMTIMER4Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_L4PER_DMTIMER4_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_L4PER_CLKCTRL_MASK);\
	newValue <<= CM1_L4PER_CLKCTRL_OFFSET;\
	newValue &= CM1_L4PER_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_L4PEREN_DMTIMER9Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_L4PER_DMTIMER9_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_L4PER_CLKCTRL_MASK);\
	newValue <<= CM1_L4PER_CLKCTRL_OFFSET;\
	newValue &= CM1_L4PER_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_L4PEREN_DMTIMER10Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_L4PER_DMTIMER10_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_L4PER_CLKCTRL_MASK);\
	newValue <<= CM1_L4PER_CLKCTRL_OFFSET;\
	newValue &= CM1_L4PER_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/

#define CM_L4PEREN_DMTIMER11Write32(baseAddress, value)\
{\
	const u32 offset = CM2_CM_L4PER_DMTIMER11_CLKCTRL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(((u32)(baseAddress))+offset);\
	register u32 newValue = ((u32)(value));\
	data &= ~(CM1_L4PER_CLKCTRL_MASK);\
	newValue <<= CM1_L4PER_CLKCTRL_OFFSET;\
	newValue &= CM1_L4PER_CLKCTRL_MASK ;\
	newValue |= data;\
	WR_MEM_32_VOLATILE((u32)(baseAddress)+offset, newValue);\
}

/********************************************************************/


#endif	/* USE_LEVEL_1_MACROS */



#endif /* _PRCM_REG_ACM_H */
/* EOF */

