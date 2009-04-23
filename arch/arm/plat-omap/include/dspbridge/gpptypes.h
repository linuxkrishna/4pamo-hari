
/** ================================================================
 *  @file   gpptypes.h
 *
 *  @path   $(IPC)/gpp/inc/
 *
 *  @desc   Defines the type system for DSP/BIOS Link
 *
 *  @ver    1.00.00.01
 *  =================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms
 *	and conditions found in the
 *  license agreement under which this software
 *	has been supplied or provided.
 *  ==================================================================
 */

#include <dspbridge/global_var.h>
#if !defined GPPTYPES_H
#define GPPTYPES_H


#if defined __cplusplus
extern "C" {
#endif /* defined __cplusplus */


/** =============================================================
 *  @macro  USES
 *
 *  @desc   Empty macro to indicate header file dependency
 *  =============================================================
 */
#define USES(filename)


/** =================================================================
 *  @macro  Data types
 *
 *  @desc   Basic data types
 *  =================================================================
 */


/** ===============================================================
 *  @const  TRUE/FALSE
 *
 *  @desc   Boolean constants
 *  ================================================================
 */
#if !defined FALSE
#define FALSE   0u
#endif

#if !defined TRUE
#define TRUE    1u
#endif


/** ==================================================================
 *  @const  NULL
 *
 *  @desc   Definition is language specific
 *  ===================================================================
 */
#if !defined NULL

#if defined __cplusplus
#define NULL    0u
#else  /* defined __cplusplus */
#define NULL ((void *)0)
#endif /* defined __cplusplus */

#endif /* !defined NULL */


/** ================================================================
 *  @const  NULL_CHAR
 *
 *  @desc   String terminator.
 *  ==================================================================
 */
#define NULL_CHAR '\0'


/** ===================================================================
 *  @macro  REG8/REG16/REG32
 *
 *  @desc   Macros to access register fields.
 *  =====================================================================
 */
#define REG8(A)         (*(REG char *)(A))
#define REG16(A)        (*(REG unsigned short int *)(A))
#define REG32(A)        (*(REG unsigned long int *)(A))


/** ====================================================================
 *  @macro  DSP/BIOS Link specific types
 *
 *  @desc   These types are used across DSP/BIOS Link.
 *  =====================================================================
 */


/** ====================================================================
 *  @name   PoolId
 *
 *  @desc   This type is used for identifying the different pools used by
 *          DSPLINK.
 *  ======================================================================
 */



/** ====================================================================
 *  @macro  Calling convention
 *
 *  @desc   Definition of CDECL, DLLIMPORT, DLLEXPORT can be defined by
 *          OEM for his compiler
 *  ======================================================================
 */
#define STATIC          static
#define EXTERN          extern


#if defined __cplusplus
}
#endif /* defined __cplusplus */

#endif  /* !defined GPPTYPES_H */
