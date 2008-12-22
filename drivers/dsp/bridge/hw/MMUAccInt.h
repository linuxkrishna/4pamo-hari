/*
 * MMUAccInt.h
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

#ifndef _MMU_ACC_INT_H
#define _MMU_ACC_INT_H


/* Register offset address definitions */

#define MMU_MMU_REVISION_OFFSET                                                 0x0
#define MMU_MMU_SYSCONFIG_OFFSET                                                0x10
#define MMU_MMU_SYSSTATUS_OFFSET                                                014
#define MMU_MMU_IRQSTATUS_OFFSET                                                0x18
#define MMU_MMU_IRQENABLE_OFFSET                                                0x1c
#define MMU_MMU_WALKING_ST_OFFSET                                               0x40
#define MMU_MMU_CNTL_OFFSET                                                     0x44
#define MMU_MMU_FAULT_AD_OFFSET                                                 0x48
#define MMU_MMU_TTB_OFFSET                                                      0x4c
#define MMU_MMU_LOCK_OFFSET                                                     0x50
#define MMU_MMU_LD_TLB_OFFSET                                                   0x54
#define MMU_MMU_CAM_OFFSET                                                      0x58
#define MMU_MMU_RAM_OFFSET                                                      0x5c
#define MMU_MMU_GFLUSH_OFFSET                                                   0x60
#define MMU_MMU_FLUSH_ENTRY_OFFSET                                              0x64
#define MMU_MMU_READ_CAM_OFFSET                                                 0x68
#define MMU_MMU_READ_RAM_OFFSET                                                 0x6c
#define MMU_MMU_EMU_FAULT_AD_OFFSET                                             0x70


/* Bitfield mask and offset declarations */

#define MMU_MMU_REVISION_Rev_MASK                                               0xff
#define MMU_MMU_REVISION_Rev_OFFSET                                             0

#define MMU_MMU_SYSCONFIG_ClockActivity_MASK                                    0x300
#define MMU_MMU_SYSCONFIG_ClockActivity_OFFSET                                  8

#define MMU_MMU_SYSCONFIG_IdleMode_MASK                                         0x18
#define MMU_MMU_SYSCONFIG_IdleMode_OFFSET                                       3

#define MMU_MMU_SYSCONFIG_SoftReset_MASK                                        0x2
#define MMU_MMU_SYSCONFIG_SoftReset_OFFSET                                      1

#define MMU_MMU_SYSCONFIG_AutoIdle_MASK                                         0x1
#define MMU_MMU_SYSCONFIG_AutoIdle_OFFSET                                       0

#define MMU_MMU_SYSSTATUS_ResetDone_MASK                                        0x1
#define MMU_MMU_SYSSTATUS_ResetDone_OFFSET                                      0

#define MMU_MMU_IRQSTATUS_MultiHitFault_MASK                                    0x10
#define MMU_MMU_IRQSTATUS_MultiHitFault_OFFSET                                  4

#define MMU_MMU_IRQSTATUS_TableWalkFault_MASK                                   0x8
#define MMU_MMU_IRQSTATUS_TableWalkFault_OFFSET                                 3

#define MMU_MMU_IRQSTATUS_EMUMiss_MASK                                          0x4
#define MMU_MMU_IRQSTATUS_EMUMiss_OFFSET                                        2

#define MMU_MMU_IRQSTATUS_TranslationFault_MASK                                 0x2
#define MMU_MMU_IRQSTATUS_TranslationFault_OFFSET                               1

#define MMU_MMU_IRQSTATUS_TLBMiss_MASK                                          0x1
#define MMU_MMU_IRQSTATUS_TLBMiss_OFFSET                                        0

#define MMU_MMU_IRQENABLE_MultiHitFault_MASK                                    0x10
#define MMU_MMU_IRQENABLE_MultiHitFault_OFFSET                                  4

#define MMU_MMU_IRQENABLE_TableWalkFault_MASK                                   0x8
#define MMU_MMU_IRQENABLE_TableWalkFault_OFFSET                                 3

#define MMU_MMU_IRQENABLE_EMUMiss_MASK                                          0x4
#define MMU_MMU_IRQENABLE_EMUMiss_OFFSET                                        2

#define MMU_MMU_IRQENABLE_TranslationFault_MASK                                 0x2
#define MMU_MMU_IRQENABLE_TranslationFault_OFFSET                               1

#define MMU_MMU_IRQENABLE_TLBMiss_MASK                                          0x1
#define MMU_MMU_IRQENABLE_TLBMiss_OFFSET                                        0

#define MMU_MMU_WALKING_ST_TWLRunning_MASK                                      0x1
#define MMU_MMU_WALKING_ST_TWLRunning_OFFSET                                    0

#define MMU_MMU_CNTL_EmuTLBUpdate_MASK                                          0x8
#define MMU_MMU_CNTL_EmuTLBUpdate_OFFSET                                        3

#define MMU_MMU_CNTL_TWLEnable_MASK                                             0x4
#define MMU_MMU_CNTL_TWLEnable_OFFSET                                           2

#define MMU_MMU_CNTL_MMUEnable_MASK                                             0x2
#define MMU_MMU_CNTL_MMUEnable_OFFSET                                           1

#define MMU_MMU_FAULT_AD_FaultAddress_MASK	0xffffffff
#define MMU_MMU_FAULT_AD_FaultAddress_OFFSET                                    0

#define MMU_MMU_TTB_TTBAddress_MASK	0xffffff00
#define MMU_MMU_TTB_TTBAddress_OFFSET	8

#define MMU_MMU_LOCK_BaseValue_MASK	0xfc00
#define MMU_MMU_LOCK_BaseValue_OFFSET                                           10

#define MMU_MMU_LOCK_CurrentVictim_MASK                                         0x3f0
#define MMU_MMU_LOCK_CurrentVictim_OFFSET                                       4

#define MMU_MMU_LD_TLB_LdTLBItem_MASK                                           0x1
#define MMU_MMU_LD_TLB_LdTLBItem_OFFSET                                         0

#define MMU_MMU_CAM_VATag_MASK                                                  0xfffff000
#define MMU_MMU_CAM_VATag_OFFSET                                                12

#define MMU_MMU_CAM_P_MASK                                                      0x8
#define MMU_MMU_CAM_P_OFFSET                                                    3

#define MMU_MMU_CAM_V_MASK                                                      0x4
#define MMU_MMU_CAM_V_OFFSET                                                    2

#define MMU_MMU_CAM_PageSize_MASK                                               0x3
#define MMU_MMU_CAM_PageSize_OFFSET                                             0

#define MMU_MMU_RAM_PhysicalAddress_MASK                                        0xfffff000
#define MMU_MMU_RAM_PhysicalAddress_OFFSET                                      12

#define MMU_MMU_RAM_Endianness_MASK                                             0x200
#define MMU_MMU_RAM_Endianness_OFFSET                                           9

#define MMU_MMU_RAM_ElementSize_MASK                                            0x180
#define MMU_MMU_RAM_ElementSize_OFFSET                                          7

#define MMU_MMU_RAM_Mixed_MASK                                                  0x40
#define MMU_MMU_RAM_Mixed_OFFSET                                                6

#define MMU_MMU_GFLUSH_GlobalFlush_MASK                                         0x1
#define MMU_MMU_GFLUSH_GlobalFlush_OFFSET                                       0

#define MMU_MMU_FLUSH_ENTRY_FlushEntry_MASK                                     0x1
#define MMU_MMU_FLUSH_ENTRY_FlushEntry_OFFSET                                   0

#define MMU_MMU_READ_CAM_VATag_MASK                                             0xfffff000
#define MMU_MMU_READ_CAM_VATag_OFFSET                                           12

#define MMU_MMU_READ_CAM_P_MASK                                                 0x8
#define MMU_MMU_READ_CAM_P_OFFSET                                               3

#define MMU_MMU_READ_CAM_V_MASK                                                 0x4
#define MMU_MMU_READ_CAM_V_OFFSET                                               2

#define MMU_MMU_READ_CAM_PageSize_MASK                                          0x3
#define MMU_MMU_READ_CAM_PageSize_OFFSET                                        0

#define MMU_MMU_READ_RAM_PhysicalAddress_MASK                                   0xfffff000
#define MMU_MMU_READ_RAM_PhysicalAddress_OFFSET                                 12

#define MMU_MMU_READ_RAM_Endianness_MASK                                        0x200
#define MMU_MMU_READ_RAM_Endianness_OFFSET                                      9

#define MMU_MMU_READ_RAM_ElementSize_MASK                                       0x180
#define MMU_MMU_READ_RAM_ElementSize_OFFSET                                     7

#define MMU_MMU_READ_RAM_Mixed_MASK                                             0x40
#define MMU_MMU_READ_RAM_Mixed_OFFSET                                           6

#define MMU_MMU_EMU_FAULT_AD_EmuFaultAddress_MASK                               0xffffffff
#define MMU_MMU_EMU_FAULT_AD_EmuFaultAddress_OFFSET                             0


/*****************************************************************************
* EXPORTED TYPES
******************************************************************************
*/

/* The following type definitions represent the enumerated values for each bitfield */

enum {
    MMUMMU_SYSCONFIGIdleModeSfIdle = 0x0000,
    MMUMMU_SYSCONFIGIdleModeSnIdle = 0x0001,
    MMUMMU_SYSCONFIGIdleModeSsIdle = 0x0002,
    MMUMMU_SYSCONFIGIdleModeRes = 0x0003
}   MMUMMU_SYSCONFIGIdleModeE;

enum {
    MMUMMU_SYSCONFIGSoftResetalways_r = 0x0000,
    MMUMMU_SYSCONFIGSoftResetnofun_w = 0x0000,
    MMUMMU_SYSCONFIGSoftResetnever_r = 0x0001,
    MMUMMU_SYSCONFIGSoftResetrstMode_w = 0x0001
}   MMUMMU_SYSCONFIGSoftResetE;

enum {
    MMUMMU_SYSCONFIGAutoIdleclkfree = 0x0000,
    MMUMMU_SYSCONFIGAutoIdleautoClkGate = 0x0001
}   MMUMMU_SYSCONFIGAutoIdleE;

enum {
    MMUMMU_SYSSTATUSResetDonerstongoing = 0x0000,
    MMUMMU_SYSSTATUSResetDonerstcomp = 0x0001
}   MMUMMU_SYSSTATUSResetDoneE;

enum {
    MMUMMU_IRQSTATUSMultiHitFaultnMHF_r = 0x0000,
    MMUMMU_IRQSTATUSMultiHitFaultMHFstat_w = 0x0000,
    MMUMMU_IRQSTATUSMultiHitFaultMHF_r = 0x0001,
    MMUMMU_IRQSTATUSMultiHitFaultrMHFstat_w = 0x0001
}   MMUMMU_IRQSTATUSMultiHitFaultE;

enum {
    MMUMMU_IRQSTATUSTableWalkFaultnTWF_r = 0x0000,
    MMUMMU_IRQSTATUSTableWalkFaultTWFstat_w = 0x0000,
    MMUMMU_IRQSTATUSTableWalkFaultTWF_r = 0x0001,
    MMUMMU_IRQSTATUSTableWalkFaultrTWFstat_w = 0x0001
}   MMUMMU_IRQSTATUSTableWalkFaultE;

enum {
    MMUMMU_IRQSTATUSEMUMissnEMUM_r = 0x0000,
    MMUMMU_IRQSTATUSEMUMissEstat_w = 0x0000,
    MMUMMU_IRQSTATUSEMUMissEMUM_r = 0x0001,
    MMUMMU_IRQSTATUSEMUMissrEstat_w = 0x0001
}   MMUMMU_IRQSTATUSEMUMissE;

enum {
    MMUMMU_IRQSTATUSTranslationFaultnFault_r = 0x0000,
    MMUMMU_IRQSTATUSTranslationFaultFstat_w = 0x0000,
    MMUMMU_IRQSTATUSTranslationFaultFault_r = 0x0001,
    MMUMMU_IRQSTATUSTranslationFaultrFstat_w = 0x0001
}   MMUMMU_IRQSTATUSTranslationFaultE;

enum {
    MMUMMU_IRQSTATUSTLBMissnTLBM_r = 0x0000,
    MMUMMU_IRQSTATUSTLBMissMstat_w = 0x0000,
    MMUMMU_IRQSTATUSTLBMissTLBM_r = 0x0001,
    MMUMMU_IRQSTATUSTLBMissrMstat_w = 0x0001
}   MMUMMU_IRQSTATUSTLBMissE;

enum {
    MMUMMU_IRQENABLEMultiHitFaultMHFltMAsk = 0x0000,
    MMUMMU_IRQENABLEMultiHitFaultMHFltGInt = 0x0001
}   MMUMMU_IRQENABLEMultiHitFaultE;

enum {
    MMUMMU_IRQENABLETableWalkFaultTWLFltMAsk = 0x0000,
    MMUMMU_IRQENABLETableWalkFaultTWLFltGInt = 0x0001
}   MMUMMU_IRQENABLETableWalkFaultE;

enum {
    MMUMMU_IRQENABLEEMUMissEMUMFltMask = 0x0000,
    MMUMMU_IRQENABLEEMUMissEMUMFltGInt = 0x0001
}   MMUMMU_IRQENABLEEMUMissE;

enum {
    MMUMMU_IRQENABLETranslationFaultTranFltMask = 0x0000,
    MMUMMU_IRQENABLETranslationFaultTranFltGInt = 0x0001
}   MMUMMU_IRQENABLETranslationFaultE;

enum {
    MMUMMU_IRQENABLETLBMissTrMissIntM = 0x0000,
    MMUMMU_IRQENABLETLBMissTrMissGInt = 0x0001
}   MMUMMU_IRQENABLETLBMissE;

enum {
    MMUMMU_WALKING_STTWLRunningTWLComp = 0x0000,
    MMUMMU_WALKING_STTWLRunningTWLRun = 0x0001
}   MMUMMU_WALKING_STTWLRunningE;

enum {
    MMUMMU_CNTLEmuTLBUpdateEMUdis = 0x0000,
    MMUMMU_CNTLEmuTLBUpdateEMUen = 0x0001
}   MMUMMU_CNTLEmuTLBUpdateE;

enum {
    MMUMMU_CNTLTWLEnableTWLdis = 0x0000,
    MMUMMU_CNTLTWLEnableTWLen = 0x0001
}   MMUMMU_CNTLTWLEnableE;

enum {
    MMUMMU_CNTLMMUEnableMMUdis = 0x0000,
    MMUMMU_CNTLMMUEnableMMUen = 0x0001
}   MMUMMU_CNTLMMUEnableE;

enum {
    MMUMMU_LD_TLBLdTLBItemalways_r = 0x0000,
    MMUMMU_LD_TLBLdTLBItemnoeffect_w = 0x0000,
    MMUMMU_LD_TLBLdTLBItemnever_r = 0x0001,
    MMUMMU_LD_TLBLdTLBItemldTLB_w = 0x0001
}   MMUMMU_LD_TLBLdTLBItemE;

enum {
    MMUMMU_CAMPCanFlush = 0x0000,
    MMUMMU_CAMPNoFlush = 0x0001
}   MMUMMU_CAMPE;

enum {
    MMUMMU_CAMVInvalid = 0x0000,
    MMUMMU_CAMVValid = 0x0001
}   MMUMMU_CAMVE;

enum {
    MMUMMU_CAMPageSizeSection = 0x0000,
    MMUMMU_CAMPageSizeLarge = 0x0001,
    MMUMMU_CAMPageSizeSmall = 0x0002,
    MMUMMU_CAMPageSizeSuper = 0x0003
}   MMUMMU_CAMPageSizeE;

enum {
    MMUMMU_RAMEndiannessLittle = 0x0000,
    MMUMMU_RAMEndiannessBig = 0x0001
}   MMUMMU_RAMEndiannessE;

enum {
    MMUMMU_RAMElementSizeByte = 0x0000,
    MMUMMU_RAMElementSizeShort = 0x0001,
    MMUMMU_RAMElementSizeLong = 0x0002,
    MMUMMU_RAMElementSizeNone = 0x0003
}   MMUMMU_RAMElementSizeE;

enum {
    MMUMMU_RAMMixedTLBes = 0x0000,
    MMUMMU_RAMMixedCPUes = 0x0001
}   MMUMMU_RAMMixedE;

enum {
    MMUMMU_GFLUSHGlobalFlushrtn0_r = 0x0000,
    MMUMMU_GFLUSHGlobalFlushnft_w = 0x0000,
    MMUMMU_GFLUSHGlobalFlushnever_r = 0x0001,
    MMUMMU_GFLUSHGlobalFlushflush_w = 0x0001
}   MMUMMU_GFLUSHGlobalFlushE;

enum {
    MMUMMU_FLUSH_ENTRYFlushEntryalways_r = 0x0000,
    MMUMMU_FLUSH_ENTRYFlushEntrynofun_w = 0x0000,
    MMUMMU_FLUSH_ENTRYFlushEntrynever_r = 0x0001,
    MMUMMU_FLUSH_ENTRYFlushEntryflushTLB_w = 0x0001
}   MMUMMU_FLUSH_ENTRYFlushEntryE;

enum {
    MMUMMU_READ_CAMPCanFlush = 0x0000,
    MMUMMU_READ_CAMPNoFlush = 0x0001
}   MMUMMU_READ_CAMPE;

enum {
    MMUMMU_READ_CAMVInvalid = 0x0000,
    MMUMMU_READ_CAMVValid = 0x0001
}   MMUMMU_READ_CAMVE;

enum {
    MMUMMU_READ_CAMPageSizeSection = 0x0000,
    MMUMMU_READ_CAMPageSizeLarge = 0x0001,
    MMUMMU_READ_CAMPageSizeSmall = 0x0002,
    MMUMMU_READ_CAMPageSizeSuper = 0x0003
}   MMUMMU_READ_CAMPageSizeE;

enum {
    MMUMMU_READ_RAMEndiannessLittle = 0x0000,
    MMUMMU_READ_RAMEndiannessBig = 0x0001
}   MMUMMU_READ_RAMEndiannessE;

enum {
    MMUMMU_READ_RAMElementSizeByte = 0x0000,
    MMUMMU_READ_RAMElementSizeShort = 0x0001,
    MMUMMU_READ_RAMElementSizeLong = 0x0002,
    MMUMMU_READ_RAMElementSizeNone = 0x0003
}   MMUMMU_READ_RAMElementSizeE;

enum {
    MMUMMU_READ_RAMMixedTLBes = 0x0000,
    MMUMMU_READ_RAMMixedCPUes = 0x0001
}   MMUMMU_READ_RAMMixedE;


#endif /* _MMU_ACC_INT_H */
/* EOF */

