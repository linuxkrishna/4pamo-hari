/*===========================================================================
  File    hal_ocp.c

  Path    $(PROJROOT)\driver\mailbox

  Desc    API definitions to setup OCP Socket system registers

  Rev     0.1.0

  This computer program is copyright to Texas Instruments Incorporated.
  The program may not be used without the written permission of
  Texas Instruments Incorporated or against the terms and conditions
  stipulated in the agreement under which this program has been supplied.

 (c) Texas Instruments Incorporated 2008

  ===========================================================================
*/


/*============================================================================
*STANDARD INCLUDE FILES
*=============================================================================
*/

/*============================================================================
*PROJECT SPECIFIC INCLUDE FILES
*=============================================================================
*/
#include <GlobalTypes.h>
#include <hal_ocp.h>
#include <hal_defs.h>
#include <MBXRegAcM.h>
#include <MBXAccInt.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");


/*============================================================================
*GLOBAL VARIABLES DECLARATIONS
*=============================================================================
*/

/*============================================================================
*LOCAL TYPES AND DEFINITIONS
*=============================================================================
*/


/*============================================================================
*LOCAL VARIABLES DECLARATIONS
*=============================================================================
*/

/*============================================================================
*LOCAL FUNCTIONS PROTOTYPES
*=============================================================================
*/

/*============================================================================
*EXPORTED FUNCTIONS
*=============================================================================
*/

long HW_OCP_SoftReset(const unsigned long baseAddress)
{
    long status = RET_OK;
    MLBMAILBOX_SYSCONFIGSoftResetWrite32(baseAddress, HAL_SET);
    return status;
}

long HW_OCP_SoftResetIsDone(const unsigned long baseAddress,
					unsigned long *resetIsDone)
{
    long status = RET_OK;

    *resetIsDone = MLBMAILBOX_SYSSTATUSResetDoneRead32(baseAddress);

    return status;
}

long HW_OCP_IdleModeSet(const unsigned long baseAddress,
			enum HAL_OCPIdleMode_t idleMode)
{
    long status = RET_OK;

    MLBMAILBOX_SYSCONFIGSIdleModeWrite32(baseAddress, idleMode);

    return status;
}


long HW_OCP_IdleModeGet(const unsigned long baseAddress,
			enum HAL_OCPIdleMode_t *idleMode)
{
    long status = RET_OK;

    *idleMode = (enum HAL_OCPIdleMode_t)
			MLBMAILBOX_SYSCONFIGSIdleModeRead32(baseAddress);

    return status;
}

long HW_OCP_AutoIdleSet(const unsigned long baseAddress,
				enum HAL_SetClear_t autoIdle)
{
    long status = RET_OK;

    MLBMAILBOX_SYSCONFIGAutoIdleWrite32(baseAddress, autoIdle);

    return status;
}

long HW_OCP_AutoIdleGet(const unsigned long baseAddress,
				enum HAL_SetClear_t *autoIdle)
{
    long status = RET_OK;

    *autoIdle = (enum HAL_SetClear_t)
			MLBMAILBOX_SYSCONFIGAutoIdleRead32(baseAddress);

    return status;
}

/*============================================================================
*LOCAL FUNCTIONS
*=============================================================================
*/

