
/** ========================================================================
 *  @file   ipctypes.h
 *
 *  @path   $(NOTIFY)/include
 *
 *  @desc   Defines data types for IPC
 *
 *  @ver    1.00.00.01
 *  ======================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms
 *	and conditions found in the
 *  license agreement under which this software
 *	has been supplied or provided.
 *  ========================================================================
 */


#if !defined IPCTYPES_H
#define IPCTYPES_H


/*  ----------------------------------- Common */
/* TBD: Include common types header file. */


#if defined __cplusplus
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ===================================================================
 *  @macro  IN/OUT/OPTIONAL/CONST
 *
 *  @desc   Argument specification syntax
 *  =====================================================================
 */
#define IN                              /* The argument is INPUT  only */
#define OUT                             /* The argument is OUTPUT only */
#define OPT                             /* The argument is OPTIONAL    */
#define CONST   const


/** ===================================================================
 *  @name   Notify_Handle
 *
 *  @desc   This typedef defines the type for the handle to the Notify driver.
 *  ===================================================================
 */


/** ===================================================================
 *  @macro  Calling convention
 *
 *  @desc   Definition of CDECL, DLLIMPORT, DLLEXPORT can be defined by
 *          OEM for his compiler
 *  ==================================================================
 */
#if defined OS_WINCE
/*  ------------------------------------------- WINCE               */
#define CDECL           __cdecl
#define DLLIMPORT       __declspec(dllexport)
#define DLLEXPORT       __declspec(dllexport)
/*  ------------------------------------------- WINCE               */
#endif  /* defined (OS_WINCE) */


#if defined OS_NUCLEUS
/*  ------------------------------------------- NUCLEUS             */
#define CDECL
#define DLLIMPORT
#define DLLEXPORT
/*  ------------------------------------------- NUCLEUS             */
#endif  /* defined (OS_NUCLEUS) */

#if defined OS_LINUX
/*  ------------------------------------------- LINUX               */
#define CDECL
#define DLLIMPORT
#define DLLEXPORT
/*  ------------------------------------------- LINUX               */
#endif  /* defined (OS_LINUX) */


#if defined OS_PROS
/*  ------------------------------------------- PROS                */
#define CDECL
#define DLLIMPORT
#define DLLEXPORT
/*  ------------------------------------------- PROS                */
#endif  /* defined (OS_PROS) */


#if defined OS_DSPBIOS
/*  ------------------------------------------- PROS                */
#define CDECL
#define DLLIMPORT
#define DLLEXPORT
/*  ------------------------------------------- PROS                */
#endif  /* defined (OS_DSPBIOS) */

/* Derived calling conventions */
#define NORMAL_API      CDECL
#define IMPORT_API      DLLIMPORT
#define EXPORT_API      DLLEXPORT


#if defined __cplusplus
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (IPCTYPES_H) */
