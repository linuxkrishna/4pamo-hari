//  ============================================================================
//  File    notifytypes.h
//
//  Path    $ (PROJROOT)\inc
//
//  Desc    Common definitions for Notify driver.
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

#if !defined(_NOTIFYTYPES_H_)
#define _NOTIFYTYPES_H_

#include <dspbridge/host_os.h>

#ifdef __cplusplus
extern "C" {
#endif


/*============================================================================*/
/*  Argument specification syntax                                             */
/*============================================================================*/
#ifndef IN
#define IN                              /* The argument is INPUT  only */
#endif

#ifndef OUT
#define OUT                             /* The argument is OUTPUT only */
#endif

#ifndef OPT
#define OPT                             /* The argument is OPTIONAL    */
#endif

#ifndef CONST
#define CONST   const
#endif


/*============================================================================*/
/*  NOTIFY Module definitions                                             */
/*============================================================================*/
#if 1
typedef u32    Notify_Status;


typedef u32    Processor_Id;
typedef u32 *  Notify_Handle;


typedef struct Notify_Attrs_tag {
    u32   maxDrivers;
} Notify_Attrs ;

typedef struct Notify_Config_tag {
    u32 *   driverAttrs ;
} Notify_Config ;

typedef void (*FnNotifyCbck) (IN     Processor_Id procId,
                              IN     u32       eventNo,
                              IN OPT u32 *       arg,
                              IN OPT u32       payload) ;
#endif

#ifdef __cplusplus
}
#endif

#endif  /* _NOTIFYTYPES_H_ */
