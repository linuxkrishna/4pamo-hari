/*
 * MLBAccInt.h
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

#ifndef _MLB_ACC_INT_H
#define _MLB_ACC_INT_H


/* Register set MAILBOX_MESSAGE___REGSET_0_15 address offset, bank address increment and number of banks */

#define MLB_MAILBOX_MESSAGE___REGSET_0_15_OFFSET	(u32)(0x0040)
#define MLB_MAILBOX_MESSAGE___REGSET_0_15_STEP		(u32)(0x0004)
#define MLB_MAILBOX_MESSAGE___REGSET_0_15_BANKS	(u32)(16)
/* Register offset address definitions relative to register set MAILBOX_MESSAGE___REGSET_0_15 */
#define MLB_MAILBOX_MESSAGE___0_15_OFFSET		(u32)(0x0)
/* Register set MAILBOX_FIFOSTATUS___REGSET_0_15 address offset, bank address increment and number of banks */

#define MLB_MAILBOX_FIFOSTATUS___REGSET_0_15_OFFSET		(u32)(0x0080)
#define MLB_MAILBOX_FIFOSTATUS___REGSET_0_15_STEP		(u32)(0x0004)
#define MLB_MAILBOX_FIFOSTATUS___REGSET_0_15_BANKS		(u32)(16)
/* Register offset address definitions relative to register set MAILBOX_FIFOSTATUS___REGSET_0_15 */
#define MLB_MAILBOX_FIFOSTATUS___0_15_OFFSET		(u32)(0x0)
/* Register set MAILBOX_MSGSTATUS___REGSET_0_15 address offset, bank address increment and number of banks */
#define MLB_MAILBOX_MSGSTATUS___REGSET_0_15_OFFSET	(u32)(0x00c0)
#define MLB_MAILBOX_MSGSTATUS___REGSET_0_15_STEP		(u32)(0x0004)
#define MLB_MAILBOX_MSGSTATUS___REGSET_0_15_BANKS		(u32)(16)
/* Register offset address definitions relative to register set MAILBOX_MSGSTATUS___REGSET_0_15 */

#define MLB_MAILBOX_MSGSTATUS___0_15_OFFSET		(u32)(0x0)

/* Register set MAILBOX_IRQSTATUS___REGSET_0_3 address offset, bank address increment and number of banks */

#define MLB_MAILBOX_IRQSTATUS___REGSET_0_3_OFFSET		(u32)(0x0100)
#define MLB_MAILBOX_IRQSTATUS___REGSET_0_3_STEP		(u32)(0x0008)
#define MLB_MAILBOX_IRQSTATUS___REGSET_0_3_BANKS		(u32)(4)

/* Register offset address definitions relative to register set MAILBOX_IRQSTATUS___REGSET_0_3 */

#define MLB_MAILBOX_IRQSTATUS___0_3_OFFSET		(u32)(0x0)


/* Register set MAILBOX_IRQENABLE___REGSET_0_3 address offset, bank address increment and number of banks */

#define MLB_MAILBOX_IRQENABLE___REGSET_0_3_OFFSET		(u32)(0x0104)
#define MLB_MAILBOX_IRQENABLE___REGSET_0_3_STEP		(u32)(0x0008)
#define MLB_MAILBOX_IRQENABLE___REGSET_0_3_BANKS	(u32)(4)
/* Register offset address definitions relative to register set MAILBOX_IRQENABLE___REGSET_0_3 */
#define MLB_MAILBOX_IRQENABLE___0_3_OFFSET		(u32)(0x0)
/* Register offset address definitions */

#define MLB_MAILBOX_REVISION_OFFSET		(u32)(0x0)
#define MLB_MAILBOX_SYSCONFIG_OFFSET		(u32)(0x10)
#define MLB_MAILBOX_SYSSTATUS_OFFSET		(u32)(0x14)

/* Bitfield mask and offset declarations */

#define MLB_MAILBOX_REVISION_Rev_MASK		(u32)(0xff)
#define MLB_MAILBOX_REVISION_Rev_OFFSET		(u32)(0)

#define MLB_MAILBOX_SYSCONFIG_ClockActivity_MASK		(u32)(0x100)
#define MLB_MAILBOX_SYSCONFIG_ClockActivity_OFFSET	(u32)(8)

#define MLB_MAILBOX_SYSCONFIG_SIdleMode_MASK		(u32)(0x18)
#define MLB_MAILBOX_SYSCONFIG_SIdleMode_OFFSET		(u32)(3)

#define MLB_MAILBOX_SYSCONFIG_SoftReset_MASK		(u32)(0x2)
#define MLB_MAILBOX_SYSCONFIG_SoftReset_OFFSET	(u32)(1)

#define MLB_MAILBOX_SYSCONFIG_AutoIdle_MASK		(u32)(0x1)
#define MLB_MAILBOX_SYSCONFIG_AutoIdle_OFFSET	(u32)(0)

#define MLB_MAILBOX_SYSSTATUS_ResetDone_MASK	(u32)(0x1)
#define MLB_MAILBOX_SYSSTATUS_ResetDone_OFFSET		(u32)(0)

#define MLB_MAILBOX_MESSAGE___0_15_MessageValueMBm_MASK	(u32)(0xffffffff)
#define MLB_MAILBOX_MESSAGE___0_15_MessageValueMBm_OFFSET	(u32)(0)

#define MLB_MAILBOX_FIFOSTATUS___0_15_FifoFullMBm_MASK		(u32)(0x1)
#define MLB_MAILBOX_FIFOSTATUS___0_15_FifoFullMBm_OFFSET	(u32)(0)

#define MLB_MAILBOX_MSGSTATUS___0_15_NbOfMsgMBm_MASK		(u32)(0x7f)
#define MLB_MAILBOX_MSGSTATUS___0_15_NbOfMsgMBm_OFFSET	(u32)(0)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB15_MASK	(u32)(0x80000000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB15_OFFSET		(u32)(31)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB15_MASK	(u32)(0x40000000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB15_OFFSET	(u32)(30)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB14_MASK	(u32)(0x20000000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB14_OFFSET		(u32)(29)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB14_MASK		(u32)(0x10000000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB14_OFFSET	(u32)(28)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB13_MASK	(u32)(0x8000000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB13_OFFSET		(u32)(27)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB13_MASK	(u32)(0x4000000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB13_OFFSET	(u32)(26)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB12_MASK	(u32)(0x2000000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB12_OFFSET	(u32)(25)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB12_MASK	(u32)(0x1000000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB12_OFFSET	(u32)(24)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB11_MASK	(u32)(0x800000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB11_OFFSET	(u32)(23)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB11_MASK	(u32)(0x400000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB11_OFFSET	(u32)(22)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB10_MASK	(u32)(0x200000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB10_OFFSET	(u32)(21)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB10_MASK	(u32)(0x100000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB10_OFFSET	(u32)(20)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB9_MASK	(u32)(0x80000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB9_OFFSET	(u32)(19)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB9_MASK	(u32)(0x40000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB9_OFFSET	(u32)(18)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB8_MASK	(u32)(0x20000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB8_OFFSET	(u32)(17)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB8_MASK	(u32)(0x10000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB8_OFFSET	(u32)(16)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB7_MASK	(u32)(0x8000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB7_OFFSET	(u32)(15)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB7_MASK	(u32)(0x4000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB7_OFFSET	(u32)(14)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB6_MASK	(u32)(0x2000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB6_OFFSET	(u32)(13)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB6_MASK	(u32)(0x1000)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB6_OFFSET	(u32)(12)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB5_MASK	(u32)(0x800)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB5_OFFSET	(u32)(11)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB5_MASK	(u32)(0x400)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB5_OFFSET	(u32)(10)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB4_MASK	(u32)(0x200)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB4_OFFSET	(u32)(9)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB4_MASK	(u32)(0x100)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB4_OFFSET	(u32)(8)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB3_MASK	(u32)(0x80)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB3_OFFSET	(u32)(7)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB3_MASK	(u32)(0x40)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB3_OFFSET	(u32)(6)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB2_MASK	(u32)(0x20)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB2_OFFSET	(u32)(5)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB2_MASK	(u32)(0x10)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB2_OFFSET	(u32)(4)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB1_MASK	(u32)(0x8)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB1_OFFSET	(u32)(3)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB1_MASK	(u32)(0x4)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB1_OFFSET	(u32)(2)

#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB0_MASK	(u32)(0x2)
#define MLB_MAILBOX_IRQSTATUS___0_3_NotFullStatusUuMB0_OFFSET	(u32)(1)

#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB0_MASK	(u32)(0x1)
#define MLB_MAILBOX_IRQSTATUS___0_3_NewMsgStatusUuMB0_OFFSET	(u32)(0)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB15_MASK	(u32)(0x80000000)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB15_OFFSET	(u32)(31)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB15_MASK	(u32)(0x40000000)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB15_OFFSET	(u32)(30)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB14_MASK	(u32)(0x20000000)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB14_OFFSET	(u32)(29)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB14_MASK	(u32)(0x10000000)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB14_OFFSET	(u32)(28)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB13_MASK	(u32)(0x8000000)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB13_OFFSET	(u32)(27)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB13_MASK	(u32)(0x4000000)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB13_OFFSET	(u32)(26)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB12_MASK	(u32)(0x2000000)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB12_OFFSET	(u32)(25)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB12_MASK	(u32)(0x1000000)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB12_OFFSET	(u32)(24)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB11_MASK	(u32)(0x800000)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB11_OFFSET	(u32)(23)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB11_MASK	(u32)(0x400000)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB11_OFFSET	(u32)(22)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB10_MASK	(u32)(0x200000)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB10_OFFSET	(u32)(21)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB10_MASK	(u32)(0x100000)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB10_OFFSET	(u32)(20)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB9_MASK	(u32)(0x80000)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB9_OFFSET	(u32)(19)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB9_MASK	(u32)(0x40000)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB9_OFFSET	(u32)(18)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB8_MASK	(u32)(0x20000)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB8_OFFSET	(u32)(17)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB8_MASK	(u32)(0x10000)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB8_OFFSET	(u32)(16)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB7_MASK	(u32)(0x8000)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB7_OFFSET	(u32)(15)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB7_MASK	(u32)(0x4000)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB7_OFFSET	(u32)(14)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB6_MASK	(u32)(0x2000)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB6_OFFSET	(u32)(13)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB6_MASK	(u32)(0x1000)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB6_OFFSET	(u32)(12)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB5_MASK	(u32)(0x800)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB5_OFFSET	(u32)(11)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB5_MASK	(u32)(0x400)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB5_OFFSET	(u32)(10)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB4_MASK	(u32)(0x200)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB4_OFFSET	(u32)(9)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB4_MASK	(u32)(0x100)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB4_OFFSET	(u32)(8)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB3_MASK	(u32)(0x80)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB3_OFFSET	(u32)(7)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB3_MASK	(u32)(0x40)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB3_OFFSET		(u32)(6)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB2_MASK	(u32)(0x20)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB2_OFFSET	(u32)(5)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB2_MASK	(u32)(0x10)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB2_OFFSET	(u32)(4)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB1_MASK	(u32)(0x8)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB1_OFFSET	(u32)(3)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB1_MASK	(u32)(0x4)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB1_OFFSET	(u32)(2)

#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB0_MASK	(u32)(0x2)
#define MLB_MAILBOX_IRQENABLE___0_3_NotFullEnableUuMB0_OFFSET	(u32)(1)

#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB0_MASK	(u32)(0x1)
#define MLB_MAILBOX_IRQENABLE___0_3_NewMsgEnableUuMB0_OFFSET	(u32)(0)


/*****************************************************************************
* EXPORTED TYPES
******************************************************************************
*/

/* The following type definitions represent the enumerated values for each bitfield */

enum {
	MLBMAILBOX_SYSCONFIGSIdleModeb00 = 0x0000,
	MLBMAILBOX_SYSCONFIGSIdleModeb01 = 0x0001,
	MLBMAILBOX_SYSCONFIGSIdleModeb10 = 0x0002,
	MLBMAILBOX_SYSCONFIGSIdleModeb11 = 0x0003
} MLBMAILBOX_SYSCONFIGSIdleModeE;

enum {
	MLBMAILBOX_SYSCONFIGSoftResetb0 = 0x0000,
	MLBMAILBOX_SYSCONFIGSoftResetb1 = 0x0001
} MLBMAILBOX_SYSCONFIGSoftResetE;

enum {
	MLBMAILBOX_SYSCONFIGAutoIdleb0 = 0x0000,
	MLBMAILBOX_SYSCONFIGAutoIdleb1 = 0x0001
} MLBMAILBOX_SYSCONFIGAutoIdleE;

enum {
	MLBMAILBOX_SYSSTATUSResetDonerstongoing = 0x0000,
	MLBMAILBOX_SYSSTATUSResetDonerstcomp = 0x0001
} MLBMAILBOX_SYSSTATUSResetDoneE;


#endif /* _MLB_ACC_INT_H */
/* EOF */

