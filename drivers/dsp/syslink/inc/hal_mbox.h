//  ===========================================================================
//  File    hal_mbox.h
//
//  Path    $ (PROJROOT)\driver\mailbox
//
//  Desc    HAL Mailbox API and types definitions.
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
//  ===========================================================================

#ifndef __MBOX_H
#define __MBOX_H

/* ============================================================================
* INCLUDE FILES (only if necessary)
* =============================================================================
*/
#include <hal_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* ============================================================================
* EXPORTED DEFINITIONS
* =============================================================================
*/
/* ----------------------------------------------------------------------------
* DEFINITION:         
*
* DESCRIPTION:  Bitmasks for Mailbox interrupt sources
*
* -----------------------------------------------------------------------------
*/

#define HAL_MBOX_INT_NEW_MSG    0x1
#define HAL_MBOX_INT_NOT_FULL   0x2
#define HAL_MBOX_INT_ALL        0x3

/* ----------------------------------------------------------------------------
* DEFINITION:   HAL_MBOX_MAX_NUM_MESSAGES
*
* DESCRIPTION:  Maximum number of messages that mailbox can hald at a time.
*
* -----------------------------------------------------------------------------
*/

#define HAL_MBOX_MAX_NUM_MESSAGES   4


/* width in bits of MBOX Id */
#define HAL_MBOX_ID_WIDTH           2


/* ============================================================================
* EXPORTED TYPES
* =============================================================================
*/

/* ----------------------------------------------------------------------------
* TYPE:         enum HAL_MBOX_Id_t
*
* DESCRIPTION:  Enumerated Type used to specify Mail Box Sub Module Id Number
*
* -----------------------------------------------------------------------------
*/
 enum HAL_MBOX_Id_t
{
    HAL_MBOX_ID_0,
    HAL_MBOX_ID_1,
    HAL_MBOX_ID_2,
    HAL_MBOX_ID_3,
    HAL_MBOX_ID_4,
    HAL_MBOX_ID_5
};

/* ----------------------------------------------------------------------------
* TYPE:         enum HAL_MBOX_UserId_t
*
* DESCRIPTION:  Enumerated Type used to specify Mail box User Id
*
* -----------------------------------------------------------------------------
*/
enum HAL_MBOX_UserId_t
{
    HAL_MBOX_U0_ARM11,
    HAL_MBOX_U1_UMA,
    HAL_MBOX_U2_IVA,
    HAL_MBOX_U3_ARM11
};

#if defined(OMAP3430)
/* ----------------------------------------------------------------------------
* TYPE:         MAILBOX_CONTEXT
*
* DESCRIPTION:  Mailbox context settings
*
* -----------------------------------------------------------------------------
*/
struct MAILBOX_CONTEXT {
    unsigned long sysconfig;
    unsigned long irqEnable0;
    unsigned long irqEnable1;
};
#endif// defined(OMAP3430)

/* ============================================================================
* EXPORTED VARIABLES
* =============================================================================
*/

/* ============================================================================
* EXPORTED FUNCTIONS
* =============================================================================
*/

/* ----------------------------------------------------------------------------
* FUNCTION      : HAL_MBOX_MsgRead
*
* INPUTS:
*
*   Identifier  : baseAddress
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mailBoxId
*   Type        : const enum HAL_MBOX_Id_t
*   Description : Mail Box Sub module Id to read
*
* OUTPUTS:
*
*   Identifier  : pReadValue
*   Type        : unsigned long *const
*   Description : Value read from MailBox
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*                 RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*                 RET_INVALID_ID      Invalid Id used
*                 RET_EMPTY           Mailbox empty
*
* PURPOSE:      : this function reads a unsigned long from the sub module message
*                 box Specified. if there are no messages in the mailbox
*                 then and error is returned.
*
* -----------------------------------------------------------------------------
*/
extern long HAL_MBOX_MsgRead(
                      const unsigned long         baseAddress,
                      const enum HAL_MBOX_Id_t   mailBoxId,
                      unsigned long *const        pReadValue
                  );

/* ----------------------------------------------------------------------------
* FUNCTION      : HAL_MBOX_MsgWrite
*
* INPUTS:
*
*   Identifier  : baseAddress
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mailBoxId
*   Type        : const enum HAL_MBOX_Id_t
*   Description : Mail Box Sub module Id to write
*
*   Identifier  : writeValue
*   Type        : const unsigned long
*   Description : Value to write to MailBox
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*                 RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*                 RET_INVALID_ID      Invalid Id used
*
* PURPOSE:      : this function writes a unsigned long from the sub module message
*                 box Specified.
*
* -----------------------------------------------------------------------------
*/
extern long HAL_MBOX_MsgWrite(
                      const unsigned long         baseAddress,
                      const enum HAL_MBOX_Id_t   mailBoxId,
                      const unsigned long         writeValue
                  );

/* ----------------------------------------------------------------------------
* FUNCTION      : HAL_MBOX_IsFull
*
* INPUTS:
*
*   Identifier  : baseAddress
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mailBoxId
*   Type        : const enum HAL_MBOX_Id_t
*   Description : Mail Box Sub module Id to check
*
* OUTPUTS:
*
*   Identifier  : pIsFull
*   Type        : unsigned long *const
*   Description : false means mail box not Full
*                 true means mailbox full.
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*                 RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*                 RET_INVALID_ID      Invalid Id used
*
* PURPOSE:      : this function reads the full status register for mailbox.
*
* -----------------------------------------------------------------------------
*/
extern long HAL_MBOX_IsFull(
                      const unsigned long         baseAddress,
                      const enum HAL_MBOX_Id_t   mailBoxId,
                      unsigned long *const        pIsFull
                  );

/* ----------------------------------------------------------------------------
* FUNCTION      : HAL_MBOX_NumMsgGet
*
* INPUTS:
*
*   Identifier  : baseAddress
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mailBoxId
*   Type        : const enum HAL_MBOX_Id_t
*   Description : Mail Box Sub module Id to get num messages
*
* OUTPUTS:
*
*   Identifier  : pNumMsg
*   Type        : unsigned long *const
*   Description : Number of messages in mailbox
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*                 RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*                 RET_INVALID_ID      Inavlid ID input at parameter
*
* PURPOSE:      : this function gets number of messages in a specified mailbox.
*
* -----------------------------------------------------------------------------
*/
extern long HAL_MBOX_NumMsgGet(
                      const unsigned long         baseAddress,
                      const enum HAL_MBOX_Id_t   mailBoxId,
                      unsigned long *const        pNumMsg
                  );

/* ----------------------------------------------------------------------------
* FUNCTION      : HAL_MBOX_EventEnable
*
* INPUTS:
*
*   Identifier  : baseAddress
*   Type        : const unsigned long
*                 RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*
*   Identifier  : mailBoxId
*   Type        : const enum HAL_MBOX_Id_t
*   Description : Mail Box Sub module Id to enable
*
*   Identifier  : userId
*   Type        : const enum HAL_MBOX_UserId_t
*   Description : Mail box User Id to enable
*
*   Identifier  : enableIrq
*   Type        : const unsigned long
*   Description : Irq value to enable
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*                 RET_BAD_NULL_PARAM  A Pointer Paramater was set to NULL
*                 RET_INVALID_ID      Invalid Id used
*
* PURPOSE:      : this function enables the specified IRQ.
*
* -----------------------------------------------------------------------------
*/
extern long HAL_MBOX_EventEnable(
                      const unsigned long             baseAddress,
                      const enum HAL_MBOX_Id_t       mailBoxId,
                      const enum HAL_MBOX_UserId_t   userId,
                      const unsigned long             events
                  );

/* ----------------------------------------------------------------------------
* FUNCTION      : HAL_MBOX_EventDisable
*
* INPUTS:
*
*   Identifier  : baseAddress
*   Type        : const unsigned long
*                 RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*
*   Identifier  : mailBoxId
*   Type        : const enum HAL_MBOX_Id_t
*   Description : Mail Box Sub module Id to disable
*
*   Identifier  : userId
*   Type        : const enum HAL_MBOX_UserId_t
*   Description : Mail box User Id to disable
*
*   Identifier  : enableIrq
*   Type        : const unsigned long
*   Description : Irq value to disable
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*                 RET_BAD_NULL_PARAM  A Pointer Paramater was set to NULL
*                 RET_INVALID_ID      Invalid Id used
*
* PURPOSE:      : this function disables the specified IRQ.
*
* -----------------------------------------------------------------------------
*/
extern long HAL_MBOX_EventDisable(
                      const unsigned long             baseAddress,
                      const enum HAL_MBOX_Id_t       mailBoxId,
                      const enum HAL_MBOX_UserId_t   userId,
                      const unsigned long             events
                  );

/* ----------------------------------------------------------------------------
* FUNCTION      : HAL_MBOX_EventStatus
*
* INPUTS:
*
*   Identifier  : baseAddress
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mailBoxId
*   Type        : const enum HAL_MBOX_Id_t
*   Description : Mail Box Sub module Id to clear
*
*   Identifier  : userId
*   Type        : const enum HAL_MBOX_UserId_t
*   Description : Mail box User Id to clear
*
* OUTPUTS:
*
*   Identifier  : pIrqStatus
*   Type        : pMBOX_Int_t *const
*   Description : The value in IRQ status register
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*                 RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*                 RET_INVALID_ID      Invalid Id used
*
* PURPOSE:      : this function gets the status of the specified IRQ.
*
* -----------------------------------------------------------------------------
*/
extern long HAL_MBOX_EventStatus(
                      const unsigned long             baseAddress,
                      const enum HAL_MBOX_Id_t       mailBoxId,
                      const enum HAL_MBOX_UserId_t   userId,
                      unsigned long *const            pEventStatus
                  );

/* ----------------------------------------------------------------------------
* FUNCTION      : HAL_MBOX_EventAck
*
* INPUTS:
*
*   Identifier  : baseAddress
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mailBoxId
*   Type        : const enum HAL_MBOX_Id_t
*   Description : Mail Box Sub module Id to set
*
*   Identifier  : userId
*   Type        : const enum HAL_MBOX_UserId_t
*   Description : Mail box User Id to set
*
*   Identifier  : irqStatus
*   Type        : const unsigned long
*   Description : The value to write IRQ status
*
* OUTPUTS:
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*                 RET_BAD_NULL_PARAM  Address Paramater was set to 0
*                 RET_INVALID_ID      Invalid Id used
*
* PURPOSE:      : this function sets the status of the specified IRQ.
*
* -----------------------------------------------------------------------------
*/
extern long HAL_MBOX_EventAck(
                      const unsigned long              baseAddress,
                      const enum HAL_MBOX_Id_t        mailBoxId,
                      const enum HAL_MBOX_UserId_t    userId,
                      const unsigned long              event
                  );

#if defined(OMAP3430)
/* ----------------------------------------------------------------------------
* FUNCTION      : HAL_MBOX_saveSettings
*
* INPUTS:
*
*   Identifier  : baseAddress
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*  
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*                 RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*                 RET_INVALID_ID      Invalid Id used
*                 RET_EMPTY           Mailbox empty
*
* PURPOSE:      : this function saves the context of mailbox
*
* -----------------------------------------------------------------------------
*/
extern long HAL_MBOX_saveSettings(unsigned long    baseAddres );

/* ----------------------------------------------------------------------------
* FUNCTION      : HAL_MBOX_restoreSettings
*
* INPUTS:
*
*   Identifier  : baseAddress
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*  
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*                 RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*                 RET_INVALID_ID      Invalid Id used
*                 RET_EMPTY           Mailbox empty
*
* PURPOSE:      : this function restores the context of mailbox
*
* -----------------------------------------------------------------------------
*/
extern long HAL_MBOX_restoreSettings(unsigned long    baseAddres );
#endif// defined(OMAP3430)

#ifdef __cplusplus
}
#endif

#endif  /* __MBOX_H */

