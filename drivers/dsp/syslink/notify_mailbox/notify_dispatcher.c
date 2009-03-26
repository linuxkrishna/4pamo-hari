/*===========================================================================
File    Mailbx.c

Path    $(PROJROOT)\driver\mailbox

Desc    Implements the Mailbox Manager module.

Rev     0.1.0

This computer program is copyright to Texas Instruments Incorporated.
The program may not be used without the written permission of
Texas Instruments Incorporated or against the terms and conditions
stipulated in the agreement under which this program has been supplied.

(c) Texas Instruments Incorporated 2008

===========================================================================
*/

#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <gpptypes.h>
#include <GlobalTypes.h>
#include <tmbx.h>
#include <hal_mbox.h>
#include <hal_ocp.h>
#include <linux/delay.h>



#include "trc.h"
#include<dbc.h>
#include<_signature.h>
#include<clk.h>
#include<global_var.h>

/*TODO Replace when correct type found*/
#define DResource 0
MODULE_LICENSE("GPL");



/*************************************/



/**============================================================================
*@macro  COMPONENT_ID
*
*@desc   Component and Subcomponent Identifier.
*============================================================================
*/
#define  COMPONENT_ID       ID_KNL_NOTIFY_MBXMGR

/*_LIT(KLitNotifyRH, "NotifyRH") ;*/

struct MboxIsrs Mailbx_swIsrs;
EXPORT_SYMBOL(Mailbx_swIsrs);

struct MboxConfig Mailbx_hwConfig;
EXPORT_SYMBOL(Mailbx_hwConfig);


unsigned long int i_mBoxModuleNo;
unsigned long int i_aIrqBit;





#if DResource
/*DResourceHandler *Mailbx_iResourceHandler;*/
#endif

const unsigned long int KMboxMpuInterrupt = 26;

const unsigned long int KMpuMailboxes = 2;




irqreturn_t Notify_Mailbox0User0_ISR(int temp, void *anArg, struct pt_regs *p)
{

	REG unsigned long int pEventStatus = 0;
	signed long int mBoxIndex = Mailbx_hwConfig.mBoxModules - 1;
	signed long int i;

	for (i = 0; i <  Mailbx_hwConfig.mailboxes[mBoxIndex]; i++) {

		/*Read the Event Status */

		HW_MBOX_EventStatus(Mailbx_hwConfig.mBoxLinearAddr,
		(enum HW_MBOX_Id_t)(i), HW_MBOX_U0_ARM11,
		(unsigned long *) &pEventStatus);

		if (pEventStatus & HW_MBOX_INT_ALL) {




			if ((Mailbx_swIsrs.Isrs
			[mBoxIndex][i*HW_MBOX_ID_WIDTH]) > 0) {

				(*Mailbx_swIsrs.Isrs[mBoxIndex][i
				*HW_MBOX_ID_WIDTH])
				(Mailbx_swIsrs.IsrParams[mBoxIndex]
				[i*HW_MBOX_ID_WIDTH]);


				HW_MBOX_EventAck(Mailbx_hwConfig.
				mBoxLinearAddr,
				(enum HW_MBOX_Id_t)i,
				HW_MBOX_U0_ARM11, HW_MBOX_INT_NEW_MSG);

			}

		}
}

	return IRQ_HANDLED;
}
EXPORT_SYMBOL(Notify_Mailbox0User0_ISR);




/*  ===========================================================================
func    Mailbx_BindInterrupt

desc    Bind an ISR to the HW interrupt line coming into the processor
===========================================================================
*/

signed long int Mailbx_BindInterrupt(signed long int interruptNo,
		ISR_Callback hwISR, void *isrArg)
{
	signed long int status = KErrNone;
	short int validInterrupt = FALSE;
	unsigned long int i;


	TRC_3ENTER("Mailbx_BindInterrupt", interruptNo, hwISR, isrArg);


/*Validate the arguments*/
	for (i = 0; i < Mailbx_hwConfig.mBoxModules; i++) {
			if (interruptNo == Mailbx_hwConfig.interruptLines[i]) {
					validInterrupt = TRUE;
					break;
			}
	}

	if (validInterrupt != TRUE)
			status = KErrArgument;


	if ((status == KErrNone) && (hwISR == NULL))
			status = KErrArgument;




		status =
		request_irq(interruptNo, (void *)Notify_Mailbox0User0_ISR,
		IRQF_SHARED, "mbox", NotifyMbxDrv_NonShmISR);

		if (status)
			printk(KERN_ALERT "REQUEST_IRQ FAILED\n");




	return status;
}


/*  ===========================================================================
func    Mailbx_Debug

desc    Print the mailbox registers and other useful debug information
===========================================================================
*/
void Mailbx_Debug(void)
{

	unsigned long int pEventStatus;
	TRC_0ENTER("Mailbx_Debug");

	HW_MBOX_EventStatus(Mailbx_hwConfig.mBoxLinearAddr,
		(enum HW_MBOX_Id_t)1, HW_MBOX_U0_ARM11, &pEventStatus);

	TRC_1PRINT(TRC_LEVEL2, "  pEventStatus = [0x%x]", pEventStatus);

	TRC_0LEAVE("Mailbx_Debug");
}


/*===========================================================================
func    Mailbx_DeInit

desc    Uninitialize the Mailbox Manager module
===========================================================================
*/
signed long int Mailbx_DeInit(void)
{
	signed long int status = KErrNone;
	unsigned long int temp;
	signed long int i;


	TRC_0ENTER("Mailbx_DeInit");
/*Reset the Mailbox module */
	HW_OCP_SoftReset(Mailbx_hwConfig.mBoxLinearAddr);
	do {
			HW_OCP_SoftResetIsDone(Mailbx_hwConfig.mBoxLinearAddr ,
						&temp);
	} while (temp == 0);

/*Reset the Configuration for the Mailbox modules on MPU */


	for (i = 0; i < MAX_MBOX_MODULES; i++) {

	#if DResource
		if ((i < Mailbx_hwConfig.mBoxModules) &&
			(Mailbx_hwConfig.interruptLines[i] != (-1)))
			disable_irq((int)(Mailbx_hwConfig.interruptLines[i]));

	#endif
		Mailbx_hwConfig.interruptLines[i] = (-1);
		Mailbx_hwConfig.mailboxes[i] = (-1);
	}

	Mailbx_hwConfig.mBoxModules = 0;
	Mailbx_hwConfig.mBoxLinearAddr = (unsigned long int) (-1);


return status;
}


/*===========================================================================
func    Mailbx_Getstruct MboxConfig

desc    Return the pointer to the Mailbox Manager's configuration object
===========================================================================
*/
struct MboxConfig *Mailbx_GetMBoxConfig(void)
{
	return &(Mailbx_hwConfig);
}


/*===========================================================================
func    Mailbx_Init

desc    Initialize the Mailbox Manager module and the mailbox hardware
===========================================================================
*/
signed long int Mailbx_Init(void)
{
	signed long int status = KErrNone;
	signed long int i, j;


	TRC_0ENTER("Mailbx_Init");

/*Initialize the configuration parameters for the Mailbox modules on MPU */
	for (i = 0; i < MAX_MBOX_MODULES; i++) {
			Mailbx_hwConfig.interruptLines[i] = (-1);
			Mailbx_hwConfig.mailboxes[i] = (-1);
	}

	for (i = 0; i < MAX_MBOX_MODULES; i++) {
			Mailbx_swIsrs.isrNo[i] = (-1);
			for (j = 0; j < MAX_MBOX_ISRS; j++) {
					Mailbx_swIsrs.Isrs[i][j] = NULL;
					Mailbx_swIsrs.IsrParams[i][j] = NULL;
			}
	}

/*Setup the configuration parameters for the Mailbox modules on MPU */
	Mailbx_hwConfig.mBoxLinearAddr = (const unsigned long) LinearAddress;
	Mailbx_hwConfig.mBoxModules = 1;
	Mailbx_hwConfig.interruptLines[(Mailbx_hwConfig.mBoxModules-1)]
			= KMboxMpuInterrupt;

	Mailbx_hwConfig.mailboxes[(Mailbx_hwConfig.mBoxModules-1)]
			= KMpuMailboxes;


	return status;
}

/*===========================================================================
func    Mailbx_InterruptDisable

desc    Disable a particular IRQ bit on a Mailbox IRQ Enable Register
===========================================================================
*/
signed long int Mailbx_InterruptDisable(unsigned long int mBoxModuleNo,
					unsigned long int aIrqBit)
{

		signed long int status = KErrNone;
		TRC_2ENTER("Mailbx_InterruptDisable", mBoxModuleNo, aIrqBit);

		/*Validate the parameters */
		if (mBoxModuleNo > Mailbx_hwConfig.mBoxModules)
				status = KErrArgument;


		if ((status == KErrNone) && (aIrqBit >= (HW_MBOX_ID_WIDTH *
		Mailbx_hwConfig.mailboxes[mBoxModuleNo-1])))
				status = KErrArgument;


		/*Interrupts on transmission not supported currently */
		if ((status == KErrNone) && (aIrqBit % HW_MBOX_ID_WIDTH))
				status = KErrNotSupported;


		if (status == KErrNone) {
			HW_MBOX_EventDisable(Mailbx_hwConfig.mBoxLinearAddr ,
			(enum HW_MBOX_Id_t)(aIrqBit / HW_MBOX_ID_WIDTH),
			HW_MBOX_U0_ARM11, HW_MBOX_INT_NEW_MSG);
		}

		return status;
}


/*===========================================================================
func    Mailbx_InterruptEnable

desc    Enable a particular IRQ bit on a Mailbox IRQ Enable Register
===========================================================================
*/
signed long int Mailbx_InterruptEnable(unsigned long int mBoxModuleNo,
					unsigned long int aIrqBit)
{

		signed long int status = KErrNone;
		TRC_2ENTER("Mailbx_InterruptEnable", mBoxModuleNo, aIrqBit);

		/*Validate the parameters */
		if (mBoxModuleNo > Mailbx_hwConfig.mBoxModules)
				status = KErrArgument;


		if ((status == KErrNone) &&
				(aIrqBit >= (HW_MBOX_ID_WIDTH *
				Mailbx_hwConfig.mailboxes[mBoxModuleNo-1])))
				status = KErrArgument;


		if ((status == KErrNone) &&
		(Mailbx_swIsrs.Isrs[mBoxModuleNo-1][aIrqBit] == NULL))
			status = KErrNotReady;



		/*Interrupts on transmission not supported currently */
		if ((status == KErrNone) && (aIrqBit % HW_MBOX_ID_WIDTH))
				status = KErrNotSupported;


	if (status == KErrNone) {
		HW_MBOX_EventEnable(Mailbx_hwConfig.mBoxLinearAddr,
		(enum HW_MBOX_Id_t)(aIrqBit / HW_MBOX_ID_WIDTH),
		HW_MBOX_U0_ARM11, HW_MBOX_INT_NEW_MSG);
	}


		return status;
}


/*  ===========================================================================
func    Mailbx_Read

desc    Read a message on a Mailbox FIFO queue
===========================================================================
*/
signed long int Mailbx_Read(unsigned long int mBoxModuleNo,
		unsigned long int aMboxNo, unsigned long int *messages,
		unsigned long int *numMessages, short int readAll)
{
		signed long int status = KErrNone;


		if (mBoxModuleNo > Mailbx_hwConfig.mBoxModules)
				status = KErrArgument;


		if ((status == KErrNone) &&
		(aMboxNo >= Mailbx_hwConfig.mailboxes[mBoxModuleNo-1]))
				status = KErrArgument;


		if ((status == KErrNone) &&
			(messages == NULL || numMessages == NULL))
				status = KErrArgument;


		if (status == KErrNone) {
				/*Read a single message */
			HW_MBOX_NumMsgGet(Mailbx_hwConfig.mBoxLinearAddr ,
			(enum HW_MBOX_Id_t)aMboxNo, numMessages);

		if (*numMessages > 0) {
			HW_MBOX_MsgRead(Mailbx_hwConfig.mBoxLinearAddr,
			(enum HW_MBOX_Id_t)aMboxNo, messages);
		} else
			status = KErrNotReady;


		}


		return (signed long int)status;
}


/*  ===========================================================================
func    Mailbx_Register

desc    Register a ISR callback associated with a particular IRQ bit on a
Mailbox IRQ Enable Register and Also Reigisters a Interrupt Handler
===========================================================================
*/
signed long int Mailbx_Register(unsigned long int mBoxModuleNo,
		unsigned long int aIrqBit, ISR_Callback isrCallbackFn,
		void *isrCallbackArgs)

{
		signed long int status = KErrNone;

		i_mBoxModuleNo =  mBoxModuleNo;
		i_aIrqBit = aIrqBit;

		TRC_4ENTER("Mailbx_Register", mBoxModuleNo, aIrqBit,
						isrCallbackFn, isrCallbackArgs);

/*Validate the parameters */
		if (mBoxModuleNo > Mailbx_hwConfig.mBoxModules)
				status = KErrArgument;


		if ((status == KErrNone) &&
				(aIrqBit >= (HW_MBOX_ID_WIDTH *
				Mailbx_hwConfig.mailboxes[mBoxModuleNo-1])))
				status = KErrArgument;


		if ((status == KErrNone) && (isrCallbackFn == NULL))
				status = KErrArgument;



		if ((status == KErrNone) && (aIrqBit % HW_MBOX_ID_WIDTH))
				status = KErrNotSupported;


		if (status == KErrNone) {

			HW_MBOX_EventDisable(Mailbx_hwConfig.mBoxLinearAddr ,
			(enum HW_MBOX_Id_t)(aIrqBit / HW_MBOX_ID_WIDTH),
			HW_MBOX_U0_ARM11, HW_MBOX_INT_NEW_MSG);



			Mailbx_swIsrs.Isrs[mBoxModuleNo-1][aIrqBit] =
			isrCallbackFn;

			Mailbx_swIsrs.IsrParams[mBoxModuleNo-1][aIrqBit]
			= isrCallbackArgs;

	}


		return status;
}


/*===========================================================================
func    Mailbx_Send

desc  Send a message on a Mailbox FIFO queue
===========================================================================
*/
signed long int Mailbx_Send(unsigned long int mBoxModuleNo,
		unsigned long int aMboxNo, unsigned long int message)
{
	signed long int status = KErrNone;
	/*unsigned long int mbxFull;*/

	TRC_3ENTER("Mailbx_Send", mBoxModuleNo, aMboxNo, message);


/*Validate the arguments */
	if (mBoxModuleNo > Mailbx_hwConfig.mBoxModules)
			status = KErrArgument;


	if ((status == KErrNone) &&
	(aMboxNo >= Mailbx_hwConfig.mailboxes[mBoxModuleNo-1]))
		status = KErrArgument;


/*Send the message */

if (1) {

	HW_MBOX_MsgWrite(Mailbx_hwConfig.mBoxLinearAddr,
	(enum HW_MBOX_Id_t)aMboxNo, message);

}


return status;
}


/*  ===========================================================================
func    Mailbx_UnBindInterrupt

desc    Remove the ISR to the HW interrupt line coming into the processor
===========================================================================
*/
signed long int Mailbx_UnBindInterrupt(signed long int interruptNo)
{
	signed long int status = KErrNone;
	unsigned long int i;
	short int validInterrupt = FALSE;

	TRC_1ENTER("Mailbx_UnBindInterrupt", interruptNo);

	/*Validate the arguments*/

	for (i = 0; i < Mailbx_hwConfig.mBoxModules; i++) {
			if (interruptNo == Mailbx_hwConfig.interruptLines[i]) {
					validInterrupt = TRUE;
					break;
			}
	}


	if (validInterrupt != TRUE)
			status = KErrArgument;


	/*Unbind the HW Interrupt */
	if (status == KErrNone)
			disable_irq(interruptNo);



	return status;
}


/*===========================================================================
func    Mailbx_Unregister

desc    Unregister a ISR callback associated with a particular IRQ bit on a
Mailbox IRQ Enable Register
===========================================================================
*/
signed long int Mailbx_Unregister(unsigned long int mBoxModuleNo,
		unsigned long int aIrqBit)
{
		signed long int status = KErrNone;

		TRC_2ENTER("Mailbx_Unregister", mBoxModuleNo, aIrqBit);

		/*Validate the arguments */
		if (mBoxModuleNo > Mailbx_hwConfig.mBoxModules)
				status = KErrArgument;


		if ((status == KErrNone) &&
				(aIrqBit >= (HW_MBOX_ID_WIDTH *
				Mailbx_hwConfig.mailboxes[mBoxModuleNo-1])))
				status = KErrArgument;


		/*Interrupts on transmission not supported currently */
		if ((status == KErrNone) && (aIrqBit % HW_MBOX_ID_WIDTH))
				status = KErrNotSupported;


		if (status == KErrNone) {
				/*Remove the ISR plugin */
			Mailbx_swIsrs.Isrs[mBoxModuleNo-1][aIrqBit] = NULL;
			Mailbx_swIsrs.IsrParams[mBoxModuleNo-1][aIrqBit] = NULL;
		}



		return status;
}
