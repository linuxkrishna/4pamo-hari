/*
 * MMURegAcM.h
 *
 * Notify driver support functions for TI OMAP processors.
 *
 * Copyright (C) 2008-2009 Texas Instruments, Inc.
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

#include <GlobalTypes.h>

#include <EasiGlobal.h>

#include "MMUAccInt.h"

#if defined(USE_LEVEL_1_MACROS)


#define MMUMMU_SYSCONFIGReadRegister32(base_address)\
(_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_SYSCONFIGReadRegister32),\
RD_MEM_32_VOLATILE((base_address)+MMU_MMU_SYSCONFIG_OFFSET))


#define MMUMMU_SYSCONFIGIdleModeWrite32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_SYSCONFIG_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((base_address)+offset);\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_SYSCONFIGIdleModeWrite32);\
	data &= ~(MMU_MMU_SYSCONFIG_IdleMode_MASK);\
	newValue <<= MMU_MMU_SYSCONFIG_IdleMode_OFFSET;\
	newValue &= MMU_MMU_SYSCONFIG_IdleMode_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(base_address+offset, newValue);\
} while(0)


#define MMUMMU_SYSCONFIGAutoIdleWrite32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_SYSCONFIG_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((base_address)+offset);\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_SYSCONFIGAutoIdleWrite32);\
	data &= ~(MMU_MMU_SYSCONFIG_AutoIdle_MASK);\
	newValue <<= MMU_MMU_SYSCONFIG_AutoIdle_OFFSET;\
	newValue &= MMU_MMU_SYSCONFIG_AutoIdle_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(base_address+offset, newValue);\
}


#define MMUMMU_IRQSTATUSReadRegister32(base_address)\
	(_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_IRQSTATUSReadRegister32),\
	RD_MEM_32_VOLATILE((base_address)+MMU_MMU_IRQSTATUS_OFFSET))


#define MMUMMU_IRQSTATUSWriteRegister32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_IRQSTATUS_OFFSET;\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_IRQSTATUSWriteRegister32);\
	WR_MEM_32_VOLATILE((base_address)+offset, newValue);\
} while(0)


#define MMUMMU_IRQENABLEReadRegister32(base_address)\
	(_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_IRQENABLEReadRegister32),\
	RD_MEM_32_VOLATILE((base_address)+MMU_MMU_IRQENABLE_OFFSET))


#define MMUMMU_IRQENABLEWriteRegister32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_IRQENABLE_OFFSET;\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_IRQENABLEWriteRegister32);\
	WR_MEM_32_VOLATILE((base_address)+offset, newValue);\
} while(0)


#define MMUMMU_WALKING_STTWLRunningRead32(base_address)\
(_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_WALKING_STTWLRunningRead32),\
(((RD_MEM_32_VOLATILE(((base_address)+(MMU_MMU_WALKING_ST_OFFSET))))\
& MMU_MMU_WALKING_ST_TWLRunning_MASK) >>\
MMU_MMU_WALKING_ST_TWLRunning_OFFSET))


#define MMUMMU_CNTLTWLEnableRead32(base_address)\
(_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_CNTLTWLEnableRead32),\
(((RD_MEM_32_VOLATILE(((base_address)+(MMU_MMU_CNTL_OFFSET)))) &\
MMU_MMU_CNTL_TWLEnable_MASK) >>\
MMU_MMU_CNTL_TWLEnable_OFFSET))


#define MMUMMU_CNTLTWLEnableWrite32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_CNTL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((base_address)+offset);\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_CNTLTWLEnableWrite32);\
	data &= ~(MMU_MMU_CNTL_TWLEnable_MASK);\
	newValue <<= MMU_MMU_CNTL_TWLEnable_OFFSET;\
	newValue &= MMU_MMU_CNTL_TWLEnable_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(base_address+offset, newValue);\
} while(0)


#define MMUMMU_CNTLMMUEnableWrite32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_CNTL_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((base_address)+offset);\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_CNTLMMUEnableWrite32);\
	data &= ~(MMU_MMU_CNTL_MMUEnable_MASK);\
	newValue <<= MMU_MMU_CNTL_MMUEnable_OFFSET;\
	newValue &= MMU_MMU_CNTL_MMUEnable_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(base_address+offset, newValue);\
}while (0)


#define MMUMMU_FAULT_ADReadRegister32(base_address)\
(_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_FAULT_ADReadRegister32),\
RD_MEM_32_VOLATILE((base_address)+MMU_MMU_FAULT_AD_OFFSET))


#define MMUMMU_TTBWriteRegister32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_TTB_OFFSET;\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_TTBWriteRegister32);\
	WR_MEM_32_VOLATILE((base_address)+offset, newValue);\
} while(0)


#define mmu_lckread_reg_32(base_address)\
(_DEBUG_LEVEL_1_EASI(EASIL1_mmu_lckread_reg_32),\
RD_MEM_32_VOLATILE((base_address)+MMU_MMU_LOCK_OFFSET))


#define mmu_lck_write_reg32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_LOCK_OFFSET;\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_mmu_lck_write_reg32);\
	WR_MEM_32_VOLATILE((base_address)+offset, newValue);\
} while(0)


#define MMUMMU_LOCKBaseValueRead32(base_address)\
(_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_LOCKBaseValueRead32),\
(((RD_MEM_32_VOLATILE(((base_address)+(MMU_MMU_LOCK_OFFSET)))) &\
MMU_MMU_LOCK_BaseValue_MASK) >>\
MMU_MMU_LOCK_BaseValue_OFFSET))


#define MMUMMU_LOCKBaseValueWrite32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_LOCK_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((base_address)+offset);\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_LOCKBaseValueWrite32);\
	data &= ~(MMU_MMU_LOCK_BaseValue_MASK);\
	newValue <<= MMU_MMU_LOCK_BaseValue_OFFSET;\
	newValue &= MMU_MMU_LOCK_BaseValue_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(base_address+offset, newValue);\
} while (0)


#define MMUMMU_LOCKCurrentVictimRead32(base_address)\
(_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_LOCKCurrentVictimRead32),\
(((RD_MEM_32_VOLATILE(((base_address)+(MMU_MMU_LOCK_OFFSET)))) &\
MMU_MMU_LOCK_CurrentVictim_MASK) >>\
MMU_MMU_LOCK_CurrentVictim_OFFSET))


#define mmu_lck_crnt_vctmwite32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_LOCK_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((base_address)+offset);\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_mmu_lck_crnt_vctmwite32);\
	data &= ~(MMU_MMU_LOCK_CurrentVictim_MASK);\
	newValue <<= MMU_MMU_LOCK_CurrentVictim_OFFSET;\
	newValue &= MMU_MMU_LOCK_CurrentVictim_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(base_address+offset, newValue);\
} while(0)


#define MMUMMU_LOCKCurrentVictimSet32(var, value)\
(_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_LOCKCurrentVictimSet32),\
(((var) & ~(MMU_MMU_LOCK_CurrentVictim_MASK)) |\
(((value) << MMU_MMU_LOCK_CurrentVictim_OFFSET) &\
MMU_MMU_LOCK_CurrentVictim_MASK)))


#define MMUMMU_LD_TLBReadRegister32(base_address)\
(_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_LD_TLBReadRegister32),\
RD_MEM_32_VOLATILE((base_address)+MMU_MMU_LD_TLB_OFFSET))


#define mmu_ld_tlbwrt_reg32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_LD_TLB_OFFSET;\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_mmu_ld_tlbwrt_reg32);\
	WR_MEM_32_VOLATILE((base_address)+offset, newValue);\
} while(0)


#define MMUMMU_CAMWriteRegister32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_CAM_OFFSET;\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_CAMWriteRegister32);\
	WR_MEM_32_VOLATILE((base_address)+offset, newValue);\
} while(0)


#define MMUMMU_RAMWriteRegister32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_RAM_OFFSET;\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_RAMWriteRegister32);\
	WR_MEM_32_VOLATILE((base_address)+offset, newValue);\
} while(0)


#define MMUMMU_GFLUSHGlobalFlushWrite32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_GFLUSH_OFFSET;\
	register u32 data = RD_MEM_32_VOLATILE((base_address)+offset);\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_GFLUSHGlobalFlushWrite32);\
	data &= ~(MMU_MMU_GFLUSH_GlobalFlush_MASK);\
	newValue <<= MMU_MMU_GFLUSH_GlobalFlush_OFFSET;\
	newValue &= MMU_MMU_GFLUSH_GlobalFlush_MASK;\
	newValue |= data;\
	WR_MEM_32_VOLATILE(base_address+offset, newValue);\
}while(0)


#define MMUMMU_FLUSH_ENTRYWriteRegister32(base_address, value)\
do {\
	const u32 offset = MMU_MMU_FLUSH_ENTRY_OFFSET;\
	register u32 newValue = (value);\
	_DEBUG_LEVEL_1_EASI(EASIL1_MMUMMU_FLUSH_ENTRYWriteRegister32);\
	WR_MEM_32_VOLATILE((base_address)+offset, newValue);\
} while (0)

#define MMUMMU_CAMReadRegister32(base_address)\
(RD_MEM_32_VOLATILE((base_address)+MMU_MMU_READ_CAM_OFFSET))

#define MMUMMU_RAMReadRegister32(base_address)\
(RD_MEM_32_VOLATILE((base_address)+MMU_MMU_READ_RAM_OFFSET))

#endif	/* USE_LEVEL_1_MACROS */

#endif /* _MMU_REG_ACM_H */
