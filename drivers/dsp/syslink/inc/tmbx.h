//  ============================================================================
//  File    Mailbx.h
//
//  Path    $ (PROJROOT)\driver\mailbox
//
//  Desc    Defines the Mailbox Manager  API for the Notify HW drivers.
//
//  Rev     0.1.0
//
//  This computer program is copyright to Texas Instruments Incorporated.
//  The program may not be used without the written permission of
//  Texas Instruments Incorporated or against the terms and conditions
//  stipulated in the agreement under which this program has been supplied.
//
//  (c) Texas Instruments Incorporated 2008
//
//  ============================================================================





#ifndef __TMBX_H__
#define __TMBX_H__

//#include <e32cmn.h>
//#include <e32ver.h>
//#include <DNotifycmn.h>

//#include <e32def.h>
//#include <assp.h>
//#include <shared_resource_handler.h>
#include <gpptypes.h>
#include <notifydefs.h>
#include <linux/interrupt.h>
//include <notifytypes.h>
#include <notifyerr.h>

#define MAX_MBOX_MODULES 2
#define MAX_MBOX_ISRS 32
#define KErrNone 0
#define KErrNotSupported 1
#define KErrNotReady 2
#define KErrArgument 2
//#define KHwBaseMailBoxReg 3
//#define KErrTimedOut 4
//#define KMailBoxesIClk 5




/*TODO ,Replace it appropriate type later*/
typedef void (*ISR_Callback)(void *);
typedef ISR_Callback TIsr;
//#define DResource 
/****************************************/

struct MboxConfig {
    unsigned long int mBoxLinearAddr;
    unsigned long int mBoxModules;
    signed long int interruptLines[MAX_MBOX_MODULES];
    signed long int mailboxes[MAX_MBOX_MODULES];
};


struct MboxIsrs {
    signed long int isrNo[MAX_MBOX_MODULES]; // TODO: Remove this - seems to be unused.
    TIsr Isrs[MAX_MBOX_MODULES][MAX_MBOX_ISRS];
    void * IsrParams[MAX_MBOX_MODULES][MAX_MBOX_ISRS];
};



irqreturn_t Notify_Mailbox0User0_ISR(int temp, void *anArg, struct pt_regs *p);
//  ===========================================================================
//  name    Mailbx
//
//  desc    Class exporting APIs for the Mailboxes module
//
//  base    None
//  ===========================================================================


 
    //  ========================================================================
    //  func    Mailbx::BindInterrupt
    //
    //  desc    Bind an ISR to the HW interrupt line coming into the processor
    //  ========================================================================
      signed long int Mailbx_BindInterrupt(signed long int interruptNo, TIsr hwISR, void *isrArg);


    //  ========================================================================
    //  func    Mailbx::Debug
    //
    //  desc    Print the mailbox registers and other useful debug information
    //  ========================================================================
      void Mailbx_Debug(void);


    //  ========================================================================
    //  func    Mailbx::DeInit
    //
    //  desc    Uninitialize the Mailbox Manager module
    //  ========================================================================
      signed long int Mailbx_DeInit(void);


    //  ========================================================================
    //  func    Mailbx::GetMboxConfig
    //
    //  desc    Return the pointer to the Mailbox Manager's configuration object
    //  ========================================================================
      struct MboxConfig * Mailbx_GetMBoxConfig(void);


    //  ========================================================================
    //  func    Mailbx::Init
    //
    //  desc    Initialize the Mailbox Manager module
    //  ========================================================================
      signed long int Mailbx_Init(void);


    //  ========================================================================
    //  func    Mailbx::InterruptDisable
    //
    //  desc    Disable a particular IRQ bit on a Mailbox IRQ Enable Register
    //  ========================================================================
      signed long int Mailbx_InterruptDisable(unsigned long int mBoxModuleNo, unsigned long int aIrqBit);


    //  ========================================================================
    //  func    Mailbx::InterruptEnable
    //
    //  desc    Enable a particular IRQ bit on a Mailbox IRQ Enable Register
    //  ========================================================================
      signed long int Mailbx_InterruptEnable(unsigned long int mBoxModuleNo, unsigned long int aIrqBit);


    //  ========================================================================
    //  func    Mailbx::Read
    //
    //  desc    Read a message on a Mailbox FIFO queue
    //  ========================================================================
      signed long int Mailbx_Read(unsigned long int mBoxModuleNo, unsigned long int aMboxNo, unsigned long int *messages, unsigned long int *numMessages, short int readAll);


    //  ========================================================================
    //  func    Mailbx::Register
    //
    //  desc    Register a ISR callback associated with a particular IRQ bit on a 
    //          Mailbox IRQ Enable Register
    //  ========================================================================
      signed long int Mailbx_Register(unsigned long int mBoxModuleNo, unsigned long int aIrqBit, TIsr isrCallbackFn, void *isrCallbackArgs);


    //  ========================================================================
    //  func    Mailbx::Send
    //
    //  desc    Send a message on a Mailbox FIFO queue
    //  ========================================================================
      signed long int Mailbx_Send(unsigned long int mBoxModuleNo, unsigned long int aMboxNo, unsigned long int message);


    //  ========================================================================
    //  func    Mailbx::UnBindInterrupt
    //
    //  desc    Remove the ISR to the HW interrupt line coming into the processor
    //  ========================================================================
      signed long int Mailbx_UnBindInterrupt(signed long int interruptNo);


    //  ========================================================================
    //  func    Mailbx::Unregister
    //
    //  desc    Unregister a ISR callback associated with a particular IRQ bit on a 
    //          Mailbox IRQ Enable Register
    //  ========================================================================
      signed long int Mailbx_Unregister(unsigned long int mBoxModuleNo, unsigned long int aIrqBit);


    
//     MboxConfig Mailbx_hwConfig;
//     MboxIsrs Mailbx_swIsrs;
#ifdef DResource
     DResourceHandler* iResourceHandler;
#endif



#endif // __TMBX_H__
