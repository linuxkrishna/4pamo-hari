
/*  ==================================================
  File    hw_mbox.h

  Path    $ (PROJROOT)\driver\mailbox

  Desc    HW Mailbox API and types definitions.

  Rev     0.1.0

  This computer program is copyright to Texas Instruments Incorporated.
  The program may not be used without the written permission of
  Texas Instruments Incorporated or against the terms and conditions
  stipulated in the agreement under which this program has been supplied.

  (c) Texas Instruments Incorporated 2008

  ======================================================
*/
#ifndef __MBOX_H
#define __MBOX_H

/* =======================================================
* INCLUDE FILES (only if necessary)
* ========================================================
*/
#include <hw_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* =========================================================
* EXPORTED DEFINITIONS
* ==========================================================
*/
/* ---------------------------------------------------------
* DEFINITION:
*
* DESCRIPTION:  Bitmasks for Mailbox interrupt sources
*
* ----------------------------------------------------------
*/

#define HW_MBOX_INT_NEW_MSG    0x1
#define HW_MBOX_INT_NOT_FULL   0x2
#define HW_MBOX_INT_ALL        0x3

/* ---------------------------------------------------------
* DEFINITION:   HW_MBOX_MAX_NUM_MESSAGES
*
* DESCRIPTION:  Maximum number of messages that mailbox can hald at a time.
*
* ----------------------------------------------------------
*/

#define HW_MBOX_MAX_NUM_MESSAGES   4


/* width in bits of MBOX Id */
#define HW_MBOX_ID_WIDTH           2


/* =============================================================
* EXPORTED TYPES
* ==============================================================
*/

/* --------------------------------------------------------------
* TYPE:         enum hw_mbox_id_t
*
* DESCRIPTION:  Enumerated Type used to specify Mail Box Sub Module Id Number
*
* -----------------------------------------------------------------
*/
enum hw_mbox_id_t {
HW_MBOX_ID_0,
HW_MBOX_ID_1,
HW_MBOX_ID_2,
HW_MBOX_ID_3,
HW_MBOX_ID_4,
HW_MBOX_ID_5
};

/* -------------------------------------------------------------------
* TYPE:         enum hw_mbox_userid_t
*
* DESCRIPTION:  Enumerated Type used to specify Mail box User Id
*
* ---------------------------------------------------------------------
*/
enum hw_mbox_userid_t {
HW_MBOX_U0_ARM11,
HW_MBOX_U1_UMA,
HW_MBOX_U2_IVA,
HW_MBOX_U3_ARM11
};

#if defined(OMAP3430)
/* ----------------------------------------------------------------------
* TYPE:         mailbox_context
*
* DESCRIPTION:  Mailbox context settings
*
* ------------------------------------------------------------------------
*/
struct mailbox_context {
unsigned long sysconfig;
unsigned long irqEnable0;
unsigned long irqEnable1;
};
#endif/* defined(OMAP3430)*/

/* ======================================================================
* EXPORTED VARIABLES
* =======================================================================
*/

/* ======================================================================
* EXPORTED FUNCTIONS
* =======================================================================
*/

/* -----------------------------------------------------------------------
* FUNCTION      : hw_mbox_msg_read
*
* INPUTS:
*
*   Identifier  : base_address
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mail_box_id
*   Type        : const enum hw_mbox_id_t
*   Description : Mail Box Sub module Id to read
*
* OUTPUTS:
*
*   Identifier  : p_read_value
*   Type        : unsigned long *const
*   Description : Value read from MailBox
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*   RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*   RET_INVALID_ID      Invalid Id used
*   RET_EMPTY           Mailbox empty
*
* PURPOSE:
*     : this function reads a unsigned long from the sub module message
*     box Specified. if there are no messages in the mailbox
*    then and error is returned.
* ----------------------------------------------------------------
*/
extern long hw_mbox_msg_read(
	const unsigned long         base_address,
	const enum hw_mbox_id_t   mail_box_id,
	unsigned long *const        p_read_value
);

/* ---------------------------------------------------------------
* FUNCTION      : hw_mbox_msg_write
*
* INPUTS:
*
*   Identifier  : base_address
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mail_box_id
*   Type        : const enum hw_mbox_id_t
*   Description : Mail Box Sub module Id to write
*
*   Identifier  : write_value
*   Type        : const unsigned long
*   Description : Value to write to MailBox
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*   RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*   RET_INVALID_ID      Invalid Id used
*
* PURPOSE:: this function writes a unsigned long from the sub module message
*           box Specified.
*
* ---------------------------------------------------------------
*/
extern long hw_mbox_msg_write(
	const unsigned long         base_address,
	const enum hw_mbox_id_t   mail_box_id,
	const unsigned long         write_value
);

/* ------------------------------------------------------------------
* FUNCTION      : hw_mbox_is_full
*
* INPUTS:
*
*   Identifier  : base_address
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mail_box_id
*   Type        : const enum hw_mbox_id_t
*   Description : Mail Box Sub module Id to check
*
* OUTPUTS:
*
*   Identifier  : p_is_full
*   Type        : unsigned long *const
*   Description : false means mail box not Full
*                 true means mailbox full.
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*   RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*   RET_INVALID_ID      Invalid Id used
*
* PURPOSE:      : this function reads the full status register for mailbox.
*
* ----------------------------------------------------------------
*/
extern long hw_mbox_is_full(
	const unsigned long         base_address,
	const enum hw_mbox_id_t   mail_box_id,
	unsigned long *const        p_is_full
);

/* -----------------------------------------------------------------
* FUNCTION      : hw_mbox_nomsg_get
*
* INPUTS:
*
*   Identifier  : base_address
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mail_box_id
*   Type        : const enum hw_mbox_id_t
*   Description : Mail Box Sub module Id to get num messages
*
* OUTPUTS:
*
*   Identifier  : p_num_msg
*   Type        : unsigned long *const
*   Description : Number of messages in mailbox
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*   RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*   RET_INVALID_ID      Inavlid ID input at parameter
*
* PURPOSE:
*    : this function gets number of messages in a specified mailbox.
*
* ----------------------------------------------------------------
*/
extern long hw_mbox_nomsg_get(
	const unsigned long         base_address,
	const enum hw_mbox_id_t   mail_box_id,
	unsigned long *const        p_num_msg
);

/* -----------------------------------------------------------------
* FUNCTION      : hw_mbox_event_enable
*
* INPUTS:
*
*   Identifier  : base_address
*   Type        : const unsigned long
*   RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*
*   Identifier  : mail_box_id
*   Type        : const enum hw_mbox_id_t
*   Description : Mail Box Sub module Id to enable
*
*   Identifier  : user_id
*   Type        : const enum hw_mbox_userid_t
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
*    RET_BAD_NULL_PARAM  A Pointer Paramater was set to NULL
*    RET_INVALID_ID      Invalid Id used
*
* PURPOSE:      : this function enables the specified IRQ.
*
* --------------------------------------------------------------------
*/
extern long hw_mbox_event_enable(
	const unsigned long             base_address,
	const enum hw_mbox_id_t       mail_box_id,
	const enum hw_mbox_userid_t   user_id,
	const unsigned long             events
);

/* ------------------------------------------------------------------
* FUNCTION      : hw_mbox_event_disable
*
* INPUTS:
*
*   Identifier  : base_address
*   Type        : const unsigned long
*  RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*
*   Identifier  : mail_box_id
*   Type        : const enum hw_mbox_id_t
*   Description : Mail Box Sub module Id to disable
*
*   Identifier  : user_id
*   Type        : const enum hw_mbox_userid_t
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
* ------------------------------------------------------------------
*/
extern long hw_mbox_event_disable(
	const unsigned long             base_address,
	const enum hw_mbox_id_t       mail_box_id,
	const enum hw_mbox_userid_t   user_id,
	const unsigned long             events
);

/* ------------------------------------------------------------------
* FUNCTION      : hw_mbox_event_status
*
* INPUTS:
*
*   Identifier  : base_address
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mail_box_id
*   Type        : const enum hw_mbox_id_t
*   Description : Mail Box Sub module Id to clear
*
*   Identifier  : user_id
*   Type        : const enum hw_mbox_userid_t
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
*   RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*   RET_INVALID_ID      Invalid Id used
*
* PURPOSE:      : this function gets the status of the specified IRQ.
*
* --------------------------------------------------------------
*/
extern long hw_mbox_event_status(
	const unsigned long             base_address,
	const enum hw_mbox_id_t       mail_box_id,
	const enum hw_mbox_userid_t   user_id,
	unsigned long *const            p_eventStatus
);

/* ----------------------------------------------------------------
* FUNCTION      : hw_mbox_event_ack
*
* INPUTS:
*
*   Identifier  : base_address
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*   Identifier  : mail_box_id
*   Type        : const enum hw_mbox_id_t
*   Description : Mail Box Sub module Id to set
*
*   Identifier  : user_id
*   Type        : const enum hw_mbox_userid_t
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
* --------------------------------------------------------------
*/
extern long hw_mbox_event_ack(
	const unsigned long              base_address,
	const enum hw_mbox_id_t        mail_box_id,
	const enum hw_mbox_userid_t    user_id,
	const unsigned long              event
);

#if defined(OMAP3430)
/* ---------------------------------------------------------------
* FUNCTION      : hw_mbox_save_settings
*
* INPUTS:
*
*   Identifier  : base_address
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*   RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*   RET_INVALID_ID      Invalid Id used
*    RET_EMPTY           Mailbox empty
*
* PURPOSE:      : this function saves the context of mailbox
*
* ----------------------------------------------------------------
*/
extern long hw_mbox_save_settings(unsigned long  baseAddres);

/* -----------------------------------------------------------------
* FUNCTION      : hw_mbox_restore_settings
*
* INPUTS:
*
*   Identifier  : base_address
*   Type        : const unsigned long
*   Description : Base Address of instance of Mailbox module
*
*
* RETURNS:
*
*   Type        : ReturnCode_t
*   Description : RET_OK              No errors occured
*   RET_BAD_NULL_PARAM  Address/pointer Paramater was set to 0/NULL
*   RET_INVALID_ID      Invalid Id used
*   RET_EMPTY           Mailbox empty
*
* PURPOSE:      : this function restores the context of mailbox
*
* -------------------------------------------------------------------
*/
extern long hw_mbox_restore_settings(unsigned long  baseAddres);
#endif/* defined(OMAP3430)*/

#ifdef __cplusplus
}
#endif

#endif  /* __MBOX_H */

