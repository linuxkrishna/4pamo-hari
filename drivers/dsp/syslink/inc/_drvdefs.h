
/** ============================================================================
*  @file   _drvdefs.h
*
*  @path   $(NOTIFY)/gpp/src/inc
*
*  @desc   Defines internal data types and structures used by Linux Druver
*
*  @ver    1.00.00.01
*  ============================================================================
*  Copyright (c) Texas Instruments Incorporated 2002-2008
*
*  Use of this software is controlled by the terms and conditions found in the
*  license agreement under which this software has been supplied or provided.
*  ============================================================================
*/


#if !defined _DRVDEFS_H
#define _DRVDEFS_H

/*  ----------------------------------- IPC Headers                 */
#include <gpptypes.h>
#include <ipctypes.h>
#include <dbc.h>

/*  ----------------------------------- IPC */
#include <ipctypes.h>

/*  ----------------------------------- NOTIFY Headers*/
#include <notify.h>

#if defined __cplusplus
EXTERN "C" {
#endif /* defined (__cplusplus) */


/*  ============================================================================
*  @macro  CMD_NOTIFY_XXXX
*
*  @desc   Command ids for NOTIFY functions.
*  ============================================================================
*/
#define NOTIFY_BASE_CMD                      (0x100)
#define NOTIFY_DRV_CMD_DRIVERINIT            (NOTIFY_BASE_CMD + 1)
#define NOTIFY_DRV_CMD_DRIVEREXIT            (NOTIFY_BASE_CMD + 2)
#define NOTIFY_DRV_CMD_REGISTEREVENT         (NOTIFY_BASE_CMD + 3)
#define NOTIFY_DRV_CMD_UNREGISTEREVENT       (NOTIFY_BASE_CMD + 4)
#define NOTIFY_DRV_CMD_SENDEVENT             (NOTIFY_BASE_CMD + 5)
#define NOTIFY_DRV_CMD_DISABLE               (NOTIFY_BASE_CMD + 6)
#define NOTIFY_DRV_CMD_RESTORE               (NOTIFY_BASE_CMD + 7)
#define NOTIFY_DRV_CMD_DISABLEEVENT          (NOTIFY_BASE_CMD + 8)
#define NOTIFY_DRV_CMD_ENABLEEVENT           (NOTIFY_BASE_CMD + 9)


/** ============================================================================
*  @name   CMD_Args
*
*  @desc   Union defining arguments to be passed to ioctl calls. For the
*          explanation of individual field please see the corresponding APIs.

*  @field  apiStatus
*              Status returned by this API.
*          apiArgs
*              Union representing arguments for different APIs.
*  ============================================================================
*/
struct Notify_CmdArgs {
signed long int apiStatus ;
union {
struct {
void *handle ;
struct Notify_Config *config ;
char *driverName ;
} driverInitArgs ;

struct {
void *handle ;
} driverExitArgs ;

struct {
void *handle ;
unsigned long int          eventNo ;
unsigned long int     procId ;
FnNotifyCbck    fnNotifyCbck ;
void *cbckArg ;
} unregisterEventArgs ;

struct {
void *handle ;
unsigned long int          eventNo ;
unsigned long int     procId ;
FnNotifyCbck    fnNotifyCbck ;
void *cbckArg ;
} registerEventArgs ;

struct {
void *handle ;
unsigned long int          eventNo ;
unsigned long int    procId ;
unsigned long int          payload;
short int            waitClear;
} sendEventArgs ;

struct {
void *disableFlags ;
} disableArgs ;

struct {
void *restoreFlags ;
} restoreArgs ;

struct {
void *handle ;
unsigned long int          eventNo ;
unsigned long int     procId ;
} disableEventArgs ;

struct {
void *handle ;
unsigned long int          eventNo ;
unsigned long int     procId ;
} enableEventArgs ;
} apiArgs ;
};

#if defined __cplusplus
}
#endif /* defined __cplusplus */


#endif  /* !defined _DRVDEFS_H */
