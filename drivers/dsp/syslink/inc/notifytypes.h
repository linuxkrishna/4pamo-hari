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

#include <host_os.h>

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


#ifdef __cplusplus
}
#endif

#endif  /* _NOTIFYTYPES_H_ */
