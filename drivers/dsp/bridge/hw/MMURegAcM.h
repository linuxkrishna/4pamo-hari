/*
 * MMURegAcM.h
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


#ifndef _MMU_REG_ACM_H
#define _MMU_REG_ACM_H



#include "GlobalTypes.h"
#include "MMUAccInt.h"


/*****************************************************************************
* EXPORTED DEFINITIONS
******************************************************************************
*/

#if defined(USE_LEVEL_1_MACROS)


#define MMUMMU_SYSCONFIGReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE((baseAddress)+MMU_MMU_SYSCONFIG_OFFSET))


/********************************************************************/


#define MMUMMU_SYSCONFIGWriteRegister32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_SYSCONFIG_OFFSET;\
	register u32 newValue = (value);\
	WR_MEM_32_VOLATILE((baseAddress)+offset, newValue);\
}



#define MMUMMU_SYSCONFIGClockActivityGet32(var)\
	((u32)(((var) & MMU_MMU_SYSCONFIG_ClockActivity_MASK) >> MMU_MMU_SYSCONFIG_ClockActivity_OFFSET))




#define MMUMMU_SYSCONFIGAutoIdleSet32(var, value)\
	((((var) & ~(MMU_MMU_SYSCONFIG_AutoIdle_MASK)) |\
	(((value) << MMU_MMU_SYSCONFIG_AutoIdle_OFFSET) &\
	MMU_MMU_SYSCONFIG_AutoIdle_MASK)))

#define MMUMMU_IRQSTATUSReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE((baseAddress)+MMU_MMU_IRQSTATUS_OFFSET))


/********************************************************************/


#define MMUMMU_IRQSTATUSWriteRegister32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_IRQSTATUS_OFFSET;\
	register u32 newValue = (value);\
	WR_MEM_32_VOLATILE((baseAddress)+offset, newValue);\
}


#define MMUMMU_IRQENABLEReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE((baseAddress)+MMU_MMU_IRQENABLE_OFFSET))


/********************************************************************/


#define MMUMMU_IRQENABLEWriteRegister32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_IRQENABLE_OFFSET;\
	register u32 newValue = (value);\
	WR_MEM_32_VOLATILE((baseAddress)+offset, newValue);\
}


/********************************************************************/

#define MMUMMU_IRQENABLETableWalkFaultSet32(var, value)\
	((((var) & ~(MMU_MMU_IRQENABLE_TableWalkFault_MASK)) |\
	(((value) << MMU_MMU_IRQENABLE_TableWalkFault_OFFSET) &\
	MMU_MMU_IRQENABLE_TableWalkFault_MASK)))
/********************************************************************/


#define MMUMMU_IRQENABLETranslationFaultRead32(baseAddress)\
	((((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_IRQENABLE_OFFSET)))) &\
	MMU_MMU_IRQENABLE_TranslationFault_MASK) >>\
	MMU_MMU_IRQENABLE_TranslationFault_OFFSET))



#define MMUMMU_IRQENABLETranslationFaultSet32(var, value)\
	((((var) & ~(MMU_MMU_IRQENABLE_TranslationFault_MASK)) |\
	(((value) << MMU_MMU_IRQENABLE_TranslationFault_OFFSET) &\
	MMU_MMU_IRQENABLE_TranslationFault_MASK)))


/********************************************************************/


#define MMUMMU_IRQENABLETLBMissRead32(baseAddress)\
	((((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_IRQENABLE_OFFSET)))) &\
	MMU_MMU_IRQENABLE_TLBMiss_MASK) >>\
	MMU_MMU_IRQENABLE_TLBMiss_OFFSET))


/********************************************************************/


#define MMUMMU_IRQENABLETLBMissReadIsTrMissIntM32(baseAddress)\
	((MMUMMU_IRQENABLETLBMissTrMissIntM == (MMUMMU_IRQENABLETLBMissE)\
	(((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_IRQENABLE_OFFSET)))) &\
	MMU_MMU_IRQENABLE_TLBMiss_MASK) >>\
	MMU_MMU_IRQENABLE_TLBMiss_OFFSET)))


/********************************************************************/


#define MMUMMU_IRQENABLETLBMissReadIsTrMissGInt32(baseAddress)\
	((MMUMMU_IRQENABLETLBMissTrMissGInt == (MMUMMU_IRQENABLETLBMissE)\
	(((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_IRQENABLE_OFFSET)))) &\
	MMU_MMU_IRQENABLE_TLBMiss_MASK) >>\
	MMU_MMU_IRQENABLE_TLBMiss_OFFSET)))


/********************************************************************/


#define MMUMMU_IRQENABLETLBMissGet32(var)\
	((u32)(((var) & MMU_MMU_IRQENABLE_TLBMiss_MASK) >> MMU_MMU_IRQENABLE_TLBMiss_OFFSET))


/********************************************************************/


#define MMUMMU_IRQENABLETLBMissIsTrMissIntM32(var)\
	((MMUMMU_IRQENABLETLBMissTrMissIntM == (MMUMMU_IRQENABLETLBMissE)(((var) & MMU_MMU_IRQENABLE_TLBMiss_MASK) >>\
	MMU_MMU_IRQENABLE_TLBMiss_OFFSET)))


/********************************************************************/


#define MMUMMU_IRQENABLETLBMissIsTrMissGInt32(var)\
	((MMUMMU_IRQENABLETLBMissTrMissGInt == (MMUMMU_IRQENABLETLBMissE)(((var) & MMU_MMU_IRQENABLE_TLBMiss_MASK) >>\
	MMU_MMU_IRQENABLE_TLBMiss_OFFSET)))


/********************************************************************/


#define MMUMMU_IRQENABLETLBMissWrite32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_IRQENABLE_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((baseAddress)+offset);\
	register u32 newValue = (value);\
	data &= ~(MMU_MMU_IRQENABLE_TLBMiss_MASK);\
	newValue <<= MMU_MMU_IRQENABLE_TLBMiss_OFFSET;\
	newValue &= MMU_MMU_IRQENABLE_TLBMiss_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(baseAddress+offset, newValue);\
}


/********************************************************************/


#define MMUMMU_IRQENABLETLBMissWriteTrMissIntM32(baseAddress)\
{\
	const u32 offset = MMU_MMU_IRQENABLE_OFFSET;\
	const u32 newValue = (u32)MMUMMU_IRQENABLETLBMissTrMissIntM <<\
	MMU_MMU_IRQENABLE_TLBMiss_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(baseAddress+offset);\
	data &= ~(MMU_MMU_IRQENABLE_TLBMiss_MASK);\
		data |= newValue;\
	WR_MEM_32_VOLATILE(baseAddress+offset, data);\
}


/********************************************************************/


#define MMUMMU_IRQENABLETLBMissWriteTrMissGInt32(baseAddress)\
{\
	const u32 offset = MMU_MMU_IRQENABLE_OFFSET;\
	const u32 newValue = (u32)MMUMMU_IRQENABLETLBMissTrMissGInt <<\
	MMU_MMU_IRQENABLE_TLBMiss_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(baseAddress+offset);\
	data &= ~(MMU_MMU_IRQENABLE_TLBMiss_MASK);\
	data |= newValue;\
	WR_MEM_32_VOLATILE(baseAddress+offset, data);\
}


/********************************************************************/


#define MMUMMU_IRQENABLETLBMissSet32(var, value)\
	((((var) & ~(MMU_MMU_IRQENABLE_TLBMiss_MASK)) |\
	(((value) << MMU_MMU_IRQENABLE_TLBMiss_OFFSET) &\
	MMU_MMU_IRQENABLE_TLBMiss_MASK)))


#define MMUMMU_WALKING_STTWLRunningRead32(baseAddress)\
	((((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_WALKING_ST_OFFSET)))) &\
	MMU_MMU_WALKING_ST_TWLRunning_MASK) >>\
	MMU_MMU_WALKING_ST_TWLRunning_OFFSET))



#define MMUMMU_CNTLTWLEnableRead32(baseAddress)\
	((((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_CNTL_OFFSET)))) &\
	MMU_MMU_CNTL_TWLEnable_MASK) >>\
	MMU_MMU_CNTL_TWLEnable_OFFSET))


#define MMUMMU_CNTLTWLEnableWrite32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_CNTL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((baseAddress)+offset);\
	register u32 newValue = (value);\
	data &= ~(MMU_MMU_CNTL_TWLEnable_MASK);\
	newValue <<= MMU_MMU_CNTL_TWLEnable_OFFSET;\
	newValue &= MMU_MMU_CNTL_TWLEnable_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(baseAddress+offset, newValue);\
}


#define MMUMMU_CNTLMMUEnableWrite32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_CNTL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((baseAddress)+offset);\
	register u32 newValue = (value);\
	data &= ~(MMU_MMU_CNTL_MMUEnable_MASK);\
	newValue <<= MMU_MMU_CNTL_MMUEnable_OFFSET;\
	newValue &= MMU_MMU_CNTL_MMUEnable_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(baseAddress+offset, newValue);\
}


#define MMUMMU_FAULT_ADReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE((baseAddress)+MMU_MMU_FAULT_AD_OFFSET))


/********************************************************************/


#define MMUMMU_FAULT_ADFaultAddressRead32(baseAddress)\
	((((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_FAULT_AD_OFFSET)))) &\
	MMU_MMU_FAULT_AD_FaultAddress_MASK) >>\
	MMU_MMU_FAULT_AD_FaultAddress_OFFSET))


/********************************************************************/


#define MMUMMU_FAULT_ADFaultAddressGet32(var)\
	((u32)(((var) & MMU_MMU_FAULT_AD_FaultAddress_MASK) >> MMU_MMU_FAULT_AD_FaultAddress_OFFSET))


/********************************************************************/


#define MMUMMU_TTBReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE((baseAddress)+MMU_MMU_TTB_OFFSET))


/********************************************************************/


#define MMUMMU_TTBWriteRegister32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_TTB_OFFSET;\
	register u32 newValue = (value);\
	WR_MEM_32_VOLATILE((baseAddress)+offset, newValue);\
}


/********************************************************************/


#define MMUMMU_TTBTTBAddressRead32(baseAddress)\
	((((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_TTB_OFFSET)))) &\
	MMU_MMU_TTB_TTBAddress_MASK) >>\
	MMU_MMU_TTB_TTBAddress_OFFSET))


/********************************************************************/


#define MMUMMU_TTBTTBAddressGet32(var)\
	((u32)(((var) & MMU_MMU_TTB_TTBAddress_MASK) >> MMU_MMU_TTB_TTBAddress_OFFSET))


/********************************************************************/


#define MMUMMU_TTBTTBAddressWrite32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_TTB_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((baseAddress)+offset);\
	register u32 newValue = (value);\
	data &= ~(MMU_MMU_TTB_TTBAddress_MASK);\
	newValue <<= MMU_MMU_TTB_TTBAddress_OFFSET;\
	newValue &= MMU_MMU_TTB_TTBAddress_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(baseAddress+offset, newValue);\
}


/********************************************************************/


#define MMUMMU_TTBTTBAddressSet32(var, value)\
	((((var) & ~(MMU_MMU_TTB_TTBAddress_MASK)) |\
	(((value) << MMU_MMU_TTB_TTBAddress_OFFSET) &\
	MMU_MMU_TTB_TTBAddress_MASK)))


/********************************************************************/


#define MMUMMU_LOCKReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE((baseAddress)+MMU_MMU_LOCK_OFFSET))


/********************************************************************/


#define MMUMMU_LOCKWriteRegister32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_LOCK_OFFSET;\
	register u32 newValue = (value);\
	WR_MEM_32_VOLATILE((baseAddress)+offset, newValue);\
}


/********************************************************************/


#define MMUMMU_LOCKBaseValueRead32(baseAddress)\
	((((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_LOCK_OFFSET)))) &\
	MMU_MMU_LOCK_BaseValue_MASK) >>\
	MMU_MMU_LOCK_BaseValue_OFFSET))


/********************************************************************/


#define MMUMMU_LOCKBaseValueGet32(var)\
	((u32)(((var) & MMU_MMU_LOCK_BaseValue_MASK) >> MMU_MMU_LOCK_BaseValue_OFFSET))


/********************************************************************/


#define MMUMMU_LOCKBaseValueWrite32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_LOCK_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((baseAddress)+offset);\
	register u32 newValue = (value);\
	data &= ~(MMU_MMU_LOCK_BaseValue_MASK);\
	newValue <<= MMU_MMU_LOCK_BaseValue_OFFSET;\
	newValue &= MMU_MMU_LOCK_BaseValue_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(baseAddress+offset, newValue);\
}


/********************************************************************/


#define MMUMMU_LOCKBaseValueSet32(var, value)\
	((((var) & ~(MMU_MMU_LOCK_BaseValue_MASK)) |\
	(((value) << MMU_MMU_LOCK_BaseValue_OFFSET) &\
	MMU_MMU_LOCK_BaseValue_MASK)))


/********************************************************************/


#define MMUMMU_LOCKCurrentVictimRead32(baseAddress)\
	((((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_LOCK_OFFSET)))) &\
	MMU_MMU_LOCK_CurrentVictim_MASK) >>\
	MMU_MMU_LOCK_CurrentVictim_OFFSET))


/********************************************************************/


#define MMUMMU_LOCKCurrentVictimGet32(var)\
	((u32)(((var) & MMU_MMU_LOCK_CurrentVictim_MASK) >> MMU_MMU_LOCK_CurrentVictim_OFFSET))


/********************************************************************/


#define MMUMMU_LOCKCurrentVictimWrite32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_LOCK_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((baseAddress)+offset);\
	register u32 newValue = (value);\
	data &= ~(MMU_MMU_LOCK_CurrentVictim_MASK);\
	newValue <<= MMU_MMU_LOCK_CurrentVictim_OFFSET;\
	newValue &= MMU_MMU_LOCK_CurrentVictim_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(baseAddress+offset, newValue);\
}


/********************************************************************/


#define MMUMMU_LOCKCurrentVictimSet32(var, value)\
	((((var) & ~(MMU_MMU_LOCK_CurrentVictim_MASK)) |\
	(((value) << MMU_MMU_LOCK_CurrentVictim_OFFSET) &\
	MMU_MMU_LOCK_CurrentVictim_MASK)))


/********************************************************************/


#define MMUMMU_LD_TLBReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE((baseAddress)+MMU_MMU_LD_TLB_OFFSET))


/********************************************************************/


#define MMUMMU_LD_TLBWriteRegister32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_LD_TLB_OFFSET;\
	register u32 newValue = (value);\
	WR_MEM_32_VOLATILE((baseAddress)+offset, newValue);\
}


/********************************************************************/


#define MMUMMU_LD_TLBLdTLBItemRead32(baseAddress)\
	((((RD_MEM_32_VOLATILE(((baseAddress)+(MMU_MMU_LD_TLB_OFFSET)))) &\
	MMU_MMU_LD_TLB_LdTLBItem_MASK) >>\
	MMU_MMU_LD_TLB_LdTLBItem_OFFSET))




#define MMUMMU_CAMReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE((baseAddress)+MMU_MMU_CAM_OFFSET))


/********************************************************************/


#define MMUMMU_CAMWriteRegister32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_CAM_OFFSET;\
	register u32 newValue = (value);\
	WR_MEM_32_VOLATILE((baseAddress)+offset, newValue);\
}
/********************************************************************/


#define MMUMMU_RAMReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE((baseAddress)+MMU_MMU_RAM_OFFSET))


/********************************************************************/


#define MMUMMU_RAMWriteRegister32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_RAM_OFFSET;\
	register u32 newValue = (value);\
	WR_MEM_32_VOLATILE((baseAddress)+offset, newValue);\
}


/********************************************************************/

#define MMUMMU_GFLUSHGlobalFlushWrite32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_GFLUSH_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((baseAddress)+offset);\
	register u32 newValue = (value);\
	data &= ~(MMU_MMU_GFLUSH_GlobalFlush_MASK);\
	newValue <<= MMU_MMU_GFLUSH_GlobalFlush_OFFSET;\
	newValue &= MMU_MMU_GFLUSH_GlobalFlush_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(baseAddress+offset, newValue);\
}


/********************************************************************/


#define MMUMMU_GFLUSHGlobalFlushWritenft_w32(baseAddress)\
{\
		const u32 offset = MMU_MMU_GFLUSH_OFFSET;\
	const u32 newValue = (u32)MMUMMU_GFLUSHGlobalFlushnft_w <<\
	MMU_MMU_GFLUSH_GlobalFlush_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(baseAddress+offset);\
	data &= ~(MMU_MMU_GFLUSH_GlobalFlush_MASK);\
	data |= newValue;\
	WR_MEM_32_VOLATILE(baseAddress+offset, data);\
}


/********************************************************************/


#define MMUMMU_GFLUSHGlobalFlushWriteflush_w32(baseAddress)\
{\
	const u32 offset = MMU_MMU_GFLUSH_OFFSET;\
	const u32 newValue = (u32)MMUMMU_GFLUSHGlobalFlushflush_w <<\
	MMU_MMU_GFLUSH_GlobalFlush_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE(baseAddress+offset);\
	data &= ~(MMU_MMU_GFLUSH_GlobalFlush_MASK);\
	data |= newValue;\
	WR_MEM_32_VOLATILE(baseAddress+offset, data);\
}


/********************************************************************/


#define MMUMMU_GFLUSHGlobalFlushSet32(var, value)\
	((((var) & ~(MMU_MMU_GFLUSH_GlobalFlush_MASK)) |\
	(((value) << MMU_MMU_GFLUSH_GlobalFlush_OFFSET) &\
	MMU_MMU_GFLUSH_GlobalFlush_MASK)))


/********************************************************************/


#define MMUMMU_FLUSH_ENTRYReadRegister32(baseAddress)\
	(RD_MEM_32_VOLATILE((baseAddress)+MMU_MMU_FLUSH_ENTRY_OFFSET))


/********************************************************************/



#define MMUMMU_FLUSH_ENTRYWriteRegister32(baseAddress, value)\
{\
	const u32 offset = MMU_MMU_FLUSH_ENTRY_OFFSET;\
	register u32 newValue = (value);\
	WR_MEM_32_VOLATILE((baseAddress)+offset, newValue);\
}



#endif	/* USE_LEVEL_1_MACROS */



#endif /* _MMU_REG_ACM_H */
/* EOF */

