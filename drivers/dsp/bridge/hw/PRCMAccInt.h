/*
 * PRCMAccInt.h
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


#ifndef _PRCM_ACC_INT_H
#define _PRCM_ACC_INT_H



/* Register offset address definitions */



/**********REGISTERS OFFSET*************************************/

/*PRM*/
/*base address = 0x4A30_6000*/

/*OCP_SOCKET_PRM*/
#define PRM_PRM_IRQSTATUS_TESLA_OFFSET           (u32)(0x030)
#define PRM_PRM_IRQENABLE_TESLA_OFFSET           (u32)(0x038)

/*TESLA_PRM*/
#define PRM_PM_TESLA_PWRSTCTRL_OFFSET            (u32)(0x400)
#define PRM_PM_TESLA_PWRSTST_OFFSET              (u32)(0x404)
#define PRM_RM_TESLA_RSTCTRL_OFFSET              (u32)(0x410)
#define PRM_RM_TESLA_RSTST_OFFSET                (u32)(0x414)
#define PRM_RM_TESLA_TESLA_CONTEXT_OFFSET        (u32)(0x424)

/*CORE_PRM*/
#define PRM_PM_CORE_PWRSTCTRL_OFFSET             (u32)(0x700)

/*ALWAYS_ON_PRM*/
#define PRM_PM_ALWON_SR_IVA_WKDEP_OFFSET         (u32)(0x630)
#define PRM_RM_ALWON_SR_IVA_CONTEXT_OFFSET       (u32)(0x634)


/*CM1*/
/*base address = 0x4A00_5000*/

/*CKGEN_CM1*/
#define CM1_CM_CLKMODE_DPLL_IVA_OFFSET           (u32)(0x1A0)
#define CM1_CM_IDLEST_DPLL_IVA_OFFSET            (u32)(0x1A4)
#define CM1_CM_AUTOIDLE_DPLL_IVA_OFFSET          (u32)(0x1A8)
#define CM1_CM_CLKSEL_DPLL_IVA_OFFSET            (u32)(0x1AC)
#define CM1_CM_DIV_M2_DPLL_IVA_OFFSET            (u32)(0x1B0)
#define CM1_CM_DIV_M4_DPLL_IVA_OFFSET            (u32)(0x1B8)
#define CM1_CM_DIV_M5_DPLL_IVA_OFFSET            (u32)(0x1BC)
#define CM1_CM_SSC_DELTAMSTEP_DPLL_IVA_OFFSET    (u32)(0x1C8)
#define CM1_CM_SSC_MODFREQDIV_DPLL_IVA_OFFSET    (u32)(0x1CC)
#define CM1_CM_BYPCLK_DPLL_IVA_OFFSET            (u32)(0x1DC)

/*TESLA_CM1*/
#define CM1_CM_TESLA_CLKSTCTRL_OFFSET            (u32)(0x400)
#define CM1_CM_TESLA_STATICDEP_OFFSET            (u32)(0x404)
#define CM1_CM_TESLA_DYNAMICDEP_OFFSET           (u32)(0x408)
#define CM1_CM_TESLA_TESLA_CLKCTRL_OFFSET        (u32)(0x420)

/*ABE_CM1*/
#define CM1_CM1_ABE_CLKSTCTRL_OFFSET             (u32)(0x500)
#define CM1_CM1_ABE_MCBSP1_CLKCTRL_OFFSET        (u32)(0x548)
#define CM1_CM1_ABE_MCBSP2_CLKCTRL_OFFSET        (u32)(0x550)
#define CM1_CM1_ABE_MCBSP3_CLKCTRL_OFFSET        (u32)(0x558)
#define CM1_CM1_ABE_TIMER5_CLKCTRL_OFFSET        (u32)(0x568)
#define CM1_CM1_ABE_TIMER6_CLKCTRL_OFFSET        (u32)(0x570)
#define CM1_CM1_ABE_TIMER7_CLKCTRL_OFFSET        (u32)(0x578)
#define CM1_CM1_ABE_TIMER8_CLKCTRL_OFFSET        (u32)(0x580)

/*CM2*/
/*base address = 0x4A00_8000*/

/*CKGEN_CM2*/
#define CM2_CM_IVA_DVFS_PERF_TESLA_OFFSET        (u32)(0x128)
#define CM2_CM_IVA_DVFS_PERF_IVAHD_OFFSET        (u32)(0x12C)
#define CM2_CM_IVA_DVFS_PERF_ABE_OFFSET          (u32)(0x130)
#define CM2_CM_IVA_DVFS_RESULT_OFFSET            (u32)(0x134)
#define CM2_CM_IVA_DVFS_CURRENT_OFFSET           (u32)(0x138)

/*L4PER_CM2*/
#define CM2_CM_L4PER_CLKSTCTRL_OFFSET            (u32)(0x1400)
#define CM2_CM_L4PER_DMTIMER10_CLKCTRL_OFFSET    (u32)(0x1428)
#define CM2_CM_L4PER_DMTIMER11_CLKCTRL_OFFSET    (u32)(0x1430)
#define CM2_CM_L4PER_DMTIMER2_CLKCTRL_OFFSET     (u32)(0x1438)
#define CM2_CM_L4PER_DMTIMER3_CLKCTRL_OFFSET     (u32)(0x1440)
#define CM2_CM_L4PER_DMTIMER4_CLKCTRL_OFFSET     (u32)(0x1448)
#define CM2_CM_L4PER_DMTIMER9_CLKCTRL_OFFSET     (u32)(0x1450)
#define CM2_CM_L4PER_MCBSP4_CLKCTRL_OFFSET       (u32)(0x14E0)
#define CM2_CM_L4PER_MCBSP5_CLKCTRL_OFFSET       (u32)(0x14E8)


/*BITS MASKS & OFFSETS*/

/******************PM_TESLA_PWRSTCTRL***********/
#define PM_TESLA_PWRSTCTRL_PowerState_OFFSET            (u32)(0x0)
#define PM_TESLA_PWRSTCTRL_PowerState_MASK              (u32)(0x3)
#define PM_TESLA_PWRSTCTRL_LogicRetState_OFFSET         (u32)(0x2)
#define PM_TESLA_PWRSTCTRL_LogicRetState_MASK           (u32)(0x4)
#define PM_TESLA_PWRSTCTRL_L1RetState_OFFSET            (u32)(0x8)
#define PM_TESLA_PWRSTCTRL_L1RetState_MASK              (u32)(0x100)
#define PM_TESLA_PWRSTCTRL_L2RetState_OFFSET            (u32)(0x9)
#define PM_TESLA_PWRSTCTRL_L2RetState_MASK              (u32)(0x200)

/******************PRM_PM_TESLA_PWRSTST*********/
#define PM_TESLA_PWRSTST_PowerState_OFFSET              (u32)(0x0)
#define PM_TESLA_PWRSTST_PowerState_MASK                (u32)(0x3)

/******************RM_TESLA_RSTCTRL*************/
#define RM_TESLA_RSTCTRL_RST1_MASK                      (u32)(0x1)
#define RM_TESLA_RSTCTRL_RST1_OFFSET                    (u32)(0x0)
#define RM_TESLA_RSTCTRL_RST2_MASK                      (u32)(0x2)
#define RM_TESLA_RSTCTRL_RST2_OFFSET                    (u32)(0x1)

/******************RM_TESLA_RSTST***************/

#define RM_TESLA_RSTST_Clear_MASK                       (u32)(0x0F)

/******************RM_TESLA_TESLA_CONTEXT*******/

#define RM_TESLA_TESLA_CONTEXT_Clear_MASK               (u32)(0x701)

/******************PM_CORE_PWRSTCTRL************/

#define PRM_PM_CORE_PWRSTCTRL_PowerControl_OFFSET       (u32)(0x0)
#define PRM_PM_CORE_PWRSTCTRL_PowerControl_MASK         (u32)(0x3)

/******************PM_ALWON_SR_IVA_WKDEP********/

#define PM_ALWON_SR_IVA_WKDEP_MPU_OFFSET                (u32)(0x0)
#define PM_ALWON_SR_IVA_WKDEP_MPU_MASK                  (u32)(0x1)
#define PM_ALWON_SR_IVA_WKDEP_DUCATI_OFFSET             (u32)(0x1)
#define PM_ALWON_SR_IVA_WKDEP_DUCATI_MASK               (u32)(0x2)

/******************CM_TESLA_CLKSTCTRL***********/

#define CM_TESLA_CLKSTCTRL_Transition_OFFSET            (u32)(0x0)
#define CM_TESLA_CLKSTCTRL_Transition_MASK              (u32)(0x3)

/******************CM_TESLA_TESLA_CLKCTRL*******/
#define CM_TESLA_TESLA_CLKCTRL_STBY_MASK                (u32)(0x40000)
#define CM_TESLA_TESLA_CLKCTRL_STBY_OFFSET              (u32)(0x18)
#define CM_TESLA_TESLA_CLKCTRL_IDLE_MASK                (u32)(0x30000)
#define CM_TESLA_TESLA_CLKCTRL_IDLE_OFFSET              (u32)(0x16)
#define CM_TESLA_TESLA_CLKCTRL_MODMODE_OFFSET           (u32)(0x0)
#define CM_TESLA_TESLA_CLKCTRL_MODMODE_MASK             (u32)(0x03)

/******************CM1_ABE_CLKSTCTRL************/

#define CM1_ABE_CLKSTCTRL_Transition_OFFSET             (u32)(0x0)
#define CM1_ABE_CLKSTCTRL_Transition_MASK               (u32)(0x3)

/******************CM1_ABE_MCBSPX&TIMERX_CLKCTRL*/
#define CM1_ABE_CLKCTRL_OFFSET                          (u32)(0x0)
#define CM1_ABE_CLKCTRL_MASK                            (u32)(0x3)

/******************CM1_L4PER_CLKSTCTRL************/

#define CM1_L4PER_CLKSTCTRL_Transition_OFFSET             (u32)(0x0)
#define CM1_L4PER_CLKSTCTRL_Transition_MASK               (u32)(0x3)

/******************CM1_L4PER_MCBSPX&DMTIMERX_CLKCTRL*/
#define CM1_L4PER_CLKCTRL_OFFSET                          (u32)(0x0)
#define CM1_L4PER_CLKCTRL_MASK                            (u32)(0x3)

/******************CM_IVA_DVFS_PERF*************/
#define CM_IVA_DVFS_PERF_OFFSET                         (u32)(0x0)
#define CM_IVA_DVFS_PERF_MASK                           (u32)(0xFF)

/*****************************************************************************
* EXPORTED TYPES
******************************************************************************
*/

/* The following type definitions represent the 
* enumerated values for each bitfield 
*/

enum PRM_PM_TESLA_PWRSTCTRLE {
	PM_TESLA_PWRSTCTRLPowerStateOFF           = 0x0000,
	PM_TESLA_PWRSTCTRLPowerStateRET           = 0x0001,
	PM_TESLA_PWRSTCTRLPowerStateINACTIVE      = 0x0002,
	PM_TESLA_PWRSTCTRLPowerStateON            = 0x0003
};

#endif /* _PRCM_ACC_INT_H */
/* EOF */

