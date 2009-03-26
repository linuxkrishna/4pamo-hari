
/** ============================================================================
 *  @file   ipcdefs.h
 *
 *  @path   $(IPC)/gpp/inc/
 *
 *  @desc   Defines data types and structures used by IPC modules.
 *
 *  @ver    1.00.00.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined IPCDEFS_H
#define IPCDEFS_H


/*  ----------------------------------- IPC headers */
#include <gpptypes.h>
#include <ipctypes.h>


#if defined __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @name   Endianism
 *
 *  @desc   Enumeration of data endianism.
 *
 *  @field  Endianism_Default
 *              Default endianism - no conversion required.
 *  @field  Endianism_Big
 *              Big endian.
 *  @field  Endianism_Little
 *              Little endian.
 *  ============================================================================
 */
enum Endianism {
    Endianism_Default = 1u,
    Endianism_Big     = 2u,
    Endianism_Little  = 3u
};


#if defined __cplusplus
}
#endif


#endif /* !defined (IPCDEFS_H) */
