/** ============================================================================
 *  @file   _notifydefs.h
 *
 *  @path   $(NOTIFY)/include
 *
 *  @desc   Defines internal data types and structures used by Notify module
 *
 *  @ver    1.00.00.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (_NOTIFYDEFS_H)
#define _NOTIFYDEFS_H


/*  ----------------------------------- IPC */
#include <ipctypes.h>
#include <gpptypes.h>
#include <notify_driverdefs.h>

/*  ----------------------------------- Notify */
#include <notifyerr.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   Notify_State
 *
 *  @desc   Defines the Notify state object, which contains all the module
 *          specific information.
 *
 *  @field  maxDrivers
 *              Maximum number of Notify drivers configured
 *  @field  drivers
 *              Array of Notify driver object.
 *  @field  disableFlags
 *              Array of flags for Notify disable.
 *              TBD: Currently does not support nesting of disable/restore calls
 *  ============================================================================
 */
 struct Notify_State {
    unsigned short int               maxDrivers ;
    struct Notify_DriverHandle  drivers [NOTIFY_MAX_DRIVERS] ;
    void *               disableFlags [NOTIFY_MAX_DRIVERS] ;
};


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (_NOTIFYDEFS_H) */

