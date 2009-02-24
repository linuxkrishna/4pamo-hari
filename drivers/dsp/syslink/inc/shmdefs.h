/** ============================================================================
 *  @file   shmdefs.h
 *
 *  @path   $(NOTIFY)/dsp/inc
 *
 *  @desc   Shared definitions for DSP/BIOS
 *
 *  @ver    1.00.00.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (SHMDEFS_H)
#define SHMDEFS_H

/*  ----------------------------------- IPC */
#include <platform.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  IPC_BUF_ALIGN
 *
 *  @desc   Value of Align parameter to alloc/create calls.
 *  ============================================================================
 */
#define IPC_BUF_ALIGN     128

/** ============================================================================
 *  @const  DSP_MAUSIZE
 *
 *  @desc   Size of the DSP MAU (in bytes).
 *  ============================================================================
 */
#define DSP_MAUSIZE           1

/** ============================================================================
 *  @const  CACHE_L2_LINESIZE
 *
 *  @desc   Line size of DSP L2 cache (in bytes).
 *  ============================================================================
 */
#define CACHE_L2_LINESIZE     128

/** ============================================================================
 *  @const  ADD_PADDING
 *
 *  @desc   Macro to add padding to a structure.
 *  ============================================================================
 */
#define ADD_PADDING(padVar, count)  unsigned short int padVar [count] ;

/*  ============================================================================
 *  @const  IPC_ALIGN
 *
 *  @desc   Macro to align a number to a specified value.
 *          x: The number to be aligned
 *          y: The value that the number should be aligned to.
 *  ============================================================================
 */
#define IPC_ALIGN(x, y) (unsigned long int)((unsigned long int)((x + y - 1) / y) * y)

/** ============================================================================
 *  @const  IPC_16BIT_PADDING
 *
 *  @desc   Padding required for alignment of a 16-bit value (for L2 cache)
 *          in 16-bit words.
 *  ============================================================================
 */
#define IPC_16BIT_PADDING  ((CACHE_L2_LINESIZE - sizeof (unsigned short int)) / 2)

/** ============================================================================
 *  @const  IPC_32BIT_PADDING
 *
 *  @desc   Padding required for alignment of a 32-bit value (for L2 cache)
 *          in 16-bit words.
 *  ============================================================================
 */
#define IPC_32BIT_PADDING  ((CACHE_L2_LINESIZE - sizeof (unsigned long int)) / 2)

/** ============================================================================
 *  @const  IPC_64BIT_PADDING
 *
 *  @desc   Padding required for alignment of a 64-bit value (for L2 cache)
 *          in 16-bit words.
 *  ============================================================================
 */
#define IPC_64BIT_PADDING  ((CACHE_L2_LINESIZE - (sizeof (unsigned long int) * 2)) / 2)

/** ============================================================================
 *  @const  IPC_BOOL_PADDING
 *
 *  @desc   Padding required for alignment of a short intean value (for L2 cache)
 *          in 16-bit words.
 *  ============================================================================
 */
#define IPC_BOOL_PADDING  ((CACHE_L2_LINESIZE - sizeof (short int)) / 2)

/** ============================================================================
 *  @const  IPC_PTR_PADDING
 *
 *  @desc   Padding required for alignment of a pointer value (for L2 cache)
 *          in 16-bit words.
 *  ============================================================================
 */
#define IPC_PTR_PADDING  ((CACHE_L2_LINESIZE - sizeof (void *)) / 2)

/** ============================================================================
 *  @const  NOTIFYSHMDRV_EVENT_ENTRY_PADDING
 *
 *  @desc   Padding length for NotifyShmDrv event entry.
 *  ============================================================================
 */
#define NOTIFYSHMDRV_EVENT_ENTRY_PADDING (  (CACHE_L2_LINESIZE     \
                                          - ((sizeof (unsigned long int)) * 3)) / 2)

/** ============================================================================
 *  @const  NOTIFYSHMDRV_CTRL_PADDING
 *
 *  @desc   Padding length for NotifyShmDrv control structure.
 *  ============================================================================
 */
#define NOTIFYSHMDRV_CTRL_PADDING (  (CACHE_L2_LINESIZE            \
                                    - (   (sizeof (void *) * 2)    \
                                       +  (sizeof (unsigned long int) * 2))) / 2)


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (SHMDEFS_H) */
