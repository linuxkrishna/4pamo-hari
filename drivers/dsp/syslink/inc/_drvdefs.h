
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
#include <dbc.h>


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
*          apiargs
*              Union representing arguments for different APIs.
*  ============================================================================
*/
struct notify_cmd_args {
signed long int apiStatus ;
union {
struct {
void *handle ;
struct notify_config *config ;
char *driver_name ;
} driverInitArgs ;

struct {
void *handle ;
} driverExitArgs ;

struct {
void *handle ;
unsigned long int          event_no ;
unsigned long int     proc_id ;
fn_notify_cbck    fn_notify_cbck ;
void *cbck_arg ;
} unregister_event_args ;

struct {
void *handle ;
unsigned long int          event_no ;
unsigned long int     proc_id ;
fn_notify_cbck    fn_notify_cbck ;
void *cbck_arg ;
} register_event_args ;

struct {
void *handle ;
unsigned long int          event_no ;
unsigned long int    proc_id ;
unsigned long int          payload;
short int            wait_clear;
} send_event_args ;

struct {
void *disable_flags ;
} disableArgs ;

struct {
void *restoreFlags ;
} restoreArgs ;

struct {
void *handle ;
unsigned long int          event_no ;
unsigned long int     proc_id ;
} disable_event_args ;

struct {
void *handle ;
unsigned long int          event_no ;
unsigned long int     proc_id ;
} enable_event_args ;
} apiargs ;
};

#if defined __cplusplus
}
#endif /* defined __cplusplus */


#endif  /* !defined _DRVDEFS_H */

