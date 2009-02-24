/*  ===========================================================================
    File    hal_mbox.c

    Path    $ (PROJROOT)\driver\mailbox

    Desc    HAL Mailbox messaging & configuration API definitions

    Rev     0.1.0

    This computer program is copyright to Texas Instruments Incorporated.
    The program may not be used without the written permission of
    Texas Instruments Incorporated or against the terms and conditions
    stipulated in the agreement under which this program has been supplied.

    (c) Texas Instruments Incorporated 2008
    ===========================================================================
    */


/* ============================================================================
 * STANDARD INCLUDE FILES
 * =============================================================================
 */

/* ============================================================================
 * PROJECT SPECIFIC INCLUDE FILES
 * =============================================================================
 */
#include <GlobalTypes.h>
#include <MBXRegAcM.h>
#include <MBXAccInt.h>
#include <hal_defs.h>
#include <hal_mbox.h>
#include<linux/module.h>
MODULE_LICENSE("GPL");

/* ============================================================================
 * GLOBAL VARIABLES DECLARATIONS
 * =============================================================================
 */

/* ============================================================================
 * LOCAL TYPES AND DEFINITIONS
 * =============================================================================
 */

/* ============================================================================
 * LOCAL VARIABLES DECLARATIONS
 * =============================================================================
 */

/* ============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 * =============================================================================
 */

/* ============================================================================
 * FUNCTIONS
 * =============================================================================
 */
#if defined(OMAP3430)
struct MAILBOX_CONTEXT mboxsetting = {0, 0, 0};
/*
 *  ======== HAL_MBOX_saveSettings ========
 *  purpose:
 *      Saves the mailbox context
 */
long HAL_MBOX_saveSettings(unsigned long    baseAddress)
{
	long status = RET_OK;

	mboxsetting.sysconfig = MLBMAILBOX_SYSCONFIGReadRegister32(baseAddress);
	/* Get current enable status */
	mboxsetting.irqEnable0 = MLBMAILBOX_IRQENABLE___0_3ReadRegister32
		(baseAddress, HAL_MBOX_U0_ARM11);
	mboxsetting.irqEnable1 = MLBMAILBOX_IRQENABLE___0_3ReadRegister32
		(baseAddress, HAL_MBOX_U1_UMA);

	return status;
}

/* ==================== Function Separator =============================*/

/*
 *  ======== HAL_MBOX_restoreSettings ========
 *  purpose:
 *      Restores the mailbox context
 */
long HAL_MBOX_restoreSettings(unsigned long    baseAddress)
{
	long status = RET_OK;

	/* Restore IRQ enable status */
	MLBMAILBOX_IRQENABLE___0_3WriteRegister32(baseAddress, HAL_MBOX_U0_ARM11,
			mboxsetting.irqEnable0);
	MLBMAILBOX_IRQENABLE___0_3WriteRegister32(baseAddress, HAL_MBOX_U1_UMA,
			mboxsetting.irqEnable1);

	/* Restore Sysconfig register */
	MLBMAILBOX_SYSCONFIGWriteRegister32(baseAddress, mboxsetting.sysconfig);

	return status;
}
#endif

long HAL_MBOX_MsgRead(
		const unsigned long    baseAddress,
		const enum HAL_MBOX_Id_t   mailBoxId,
		unsigned long *const   pReadValue
		)
{
	long status = RET_OK;
	/* Check input parameters */
	CHECK_INPUT_PARAM(baseAddress, 0,  RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_PARAM(pReadValue,  NULL, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(mailBoxId,
			HAL_MBOX_ID_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);

	/* Read 32-bit message in mail box */
	*pReadValue = MLBMAILBOX_MESSAGE___0_15ReadRegister32(baseAddress, (unsigned long)mailBoxId);

	return status;
}

/* ==================== Function Separator =============================*/

long HAL_MBOX_MsgWrite
(
 const unsigned long   baseAddress,
 const enum HAL_MBOX_Id_t  mailBoxId,
 const unsigned long   writeValue
 )
{

	long status = RET_OK;
	/* Check input parameters */
	CHECK_INPUT_PARAM(baseAddress, 0, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(mailBoxId,
			HAL_MBOX_ID_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);

	/* Write 32-bit value to mailbox */
	MLBMAILBOX_MESSAGE___0_15WriteRegister32(baseAddress, (unsigned long)mailBoxId, (unsigned long)writeValue);

	return status;
}

/* ==================== Function Separator =============================*/

long HAL_MBOX_IsFull(
		const unsigned long    baseAddress,
		const enum HAL_MBOX_Id_t   mailBoxId,
		unsigned long  *const     pIsFull
		)
{
	long status = RET_OK;
	unsigned long fullStatus;
	/* Check input parameters */
	CHECK_INPUT_PARAM(baseAddress, 0, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_PARAM(pIsFull,  NULL, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(mailBoxId,
			HAL_MBOX_ID_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);

	/* read the is full status parameter for Mailbox */
	fullStatus = MLBMAILBOX_FIFOSTATUS___0_15FifoFullMBmRead32(baseAddress, (unsigned long)mailBoxId);

	/* fill in return parameter */
	*pIsFull = (fullStatus & 0xFF);

	return status;
}

/* ==================== Function Separator =============================*/

long HAL_MBOX_NumMsgGet(
		const   unsigned long   baseAddress,
		const   enum HAL_MBOX_Id_t  mailBoxId,
		unsigned long *const    pNumMsg
		)
{
	long status = RET_OK;
	/* Check input parameters */
	CHECK_INPUT_PARAM(baseAddress, 0, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_PARAM(pNumMsg,  NULL, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);

	CHECK_INPUT_RANGE_MIN0(mailBoxId,
			HAL_MBOX_ID_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);

	/* Get number of messages available for MailBox */
	*pNumMsg = MLBMAILBOX_MSGSTATUS___0_15NbOfMsgMBmRead32(baseAddress, (unsigned long)mailBoxId);

	return status;
}

/* ==================== Function Separator =============================*/

long HAL_MBOX_EventEnable(
		const unsigned long             baseAddress,
		const enum HAL_MBOX_Id_t       mailBoxId,
		const enum HAL_MBOX_UserId_t   userId,
		const unsigned long             events
		)
{
	long status = RET_OK;
	unsigned long      irqEnableReg;
	/* Check input parameters */
	CHECK_INPUT_PARAM(baseAddress, 0, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(
			mailBoxId,
			HAL_MBOX_ID_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(
			enableIrq,
			HAL_MBOX_INT_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(
			userId,
			HAL_MBOX_USER_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);

	/* Get current enable status */
	irqEnableReg = MLBMAILBOX_IRQENABLE___0_3ReadRegister32(baseAddress, (unsigned long)userId);

	/* update enable value */
	irqEnableReg |= ((unsigned long)(events)) << (((unsigned long)(mailBoxId))*HAL_MBOX_ID_WIDTH);

	/* write new enable status */
	MLBMAILBOX_IRQENABLE___0_3WriteRegister32(baseAddress, (unsigned long)userId, (unsigned long)irqEnableReg);

	return status;
}


/* ==================== Function Separator =============================*/

long HAL_MBOX_EventDisable(
		const unsigned long             baseAddress,
		const enum HAL_MBOX_Id_t       mailBoxId,
		const enum HAL_MBOX_UserId_t   userId,
		const unsigned long             events
		)
{
	long status = RET_OK;
	unsigned long      irqDisableReg;
	/* Check input parameters */
	CHECK_INPUT_PARAM(baseAddress, 0, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(
			mailBoxId,
			HAL_MBOX_ID_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(
			disableIrq,
			HAL_MBOX_INT_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(
			userId,
			HAL_MBOX_USER_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);

	/* Get current enable status */
	irqDisableReg = MLBMAILBOX_IRQENABLE___0_3ReadRegister32(baseAddress, (unsigned long)userId);

	/* update enable value */
	irqDisableReg &= ~(((unsigned long)(events)) << (((unsigned long)(mailBoxId))*HAL_MBOX_ID_WIDTH));

	/* write new enable status */
	MLBMAILBOX_IRQENABLE___0_3WriteRegister32(baseAddress, (unsigned long)userId, (unsigned long)irqDisableReg);

	return status;
}


/* ==================== Function Separator =============================*/

long HAL_MBOX_EventStatus(
		const unsigned long              baseAddress,
		const enum HAL_MBOX_Id_t        mailBoxId,
		const enum HAL_MBOX_UserId_t    userId,
		unsigned long *const             pEventStatus
		)
{
	long status = RET_OK;
	unsigned long      irqStatusReg;
	/* Check input parameters */
	CHECK_INPUT_PARAM(baseAddress, 0, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_PARAM(pIrqStatus, NULL, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);

	CHECK_INPUT_RANGE_MIN0(
			mailBoxId,
			HAL_MBOX_ID_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(
			userId,
			HAL_MBOX_USER_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);

	/* Get Irq Status for specified mailbox/User Id */
	irqStatusReg = MLBMAILBOX_IRQSTATUS___0_3ReadRegister32(baseAddress, (unsigned long)userId);

	/* update status value */
	*pEventStatus = (unsigned long)((((unsigned long)(irqStatusReg)) >> (((unsigned long)(mailBoxId))*HAL_MBOX_ID_WIDTH)) &
			((unsigned long)(HAL_MBOX_INT_ALL)));

	return status;
}

/* ==================== Function Separator =============================*/

long HAL_MBOX_EventAck(
		const unsigned long          baseAddress,
		const enum HAL_MBOX_Id_t        mailBoxId,
		const enum HAL_MBOX_UserId_t    userId,
		const unsigned long              event
		)
{
	long status = RET_OK;
	unsigned long      irqStatusReg;
	/* Check input parameters */
	CHECK_INPUT_PARAM(baseAddress, 0, RET_BAD_NULL_PARAM, RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);

	CHECK_INPUT_RANGE_MIN0(
			irqStatus,
			HAL_MBOX_INT_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(
			mailBoxId,
			HAL_MBOX_ID_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);
	CHECK_INPUT_RANGE_MIN0(
			userId,
			HAL_MBOX_USER_MAX,
			RET_INVALID_ID,
			RES_MBOX_BASE + RES_INVALID_INPUT_PARAM);


	/* calculate status to write */
	irqStatusReg = ((unsigned long)event) << (((unsigned long)(mailBoxId))*HAL_MBOX_ID_WIDTH);

	/* clear Irq Status for specified mailbox/User Id */
	MLBMAILBOX_IRQSTATUS___0_3WriteRegister32(baseAddress, (unsigned long)userId, (unsigned long)irqStatusReg);

	return status;
}
