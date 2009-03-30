
/** ============================================================================
 *  @file   platform.h
 *
 *  @path   $(NOTIFY)/gpp/inc/Linux/OMAP/
 *
 *  @desc   Defines platform specific attributes.
 *
 *  @ver    1.00.00.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined PLATFORM_H
#define PLATFORM_H


#if defined __cplusplus
extern "C" {
#endif /* defined  __cplusplus */


/** ============================================================================
 *  @const  MAX_PROCESSORS
 *
 *  @desc   Maximum number of processors supported for this platform.
 *  ============================================================================
 */
#define MAX_PROCESSORS        2

/** ============================================================================
 *  @const  MAX_DSPS
 *
 *  @desc   Maximum number of DSPs supported for this platform.
 *  ============================================================================
 */
#define MAX_DSPS              (MAX_PROCESSORS - 1)

/** ============================================================================
 *  @const  ID_GPP
 *
 *  @desc   Processor Identifier for the GPP for this platform.
 *  ============================================================================
 */
#define ID_GPP                1


#if defined __cplusplus
}
#endif /* defined __cplusplus */


#endif /* if !defined PLATFORM_H */

