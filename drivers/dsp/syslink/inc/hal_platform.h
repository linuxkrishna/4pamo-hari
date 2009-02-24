/** ============================================================================
 *  @file   hal_platform.h
 *
 *  @path   $(NOTIFY)/gpp/src/Notify/hal/Linux/OMAP3530/
 *
 *  @desc   Contains platform and Operating System specific definitions.
 *
 *  @ver    1.00.00.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (HAL_PLATFORM_H)
#define HAL_PLATFORM_H


/*  ----------------------------------- OS Headers                  */
#include <linux/autoconf.h>
#include <linux/spinlock.h>
#include <asm/arch/io.h>
#include <asm/arch/hardware.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/*  ============================================================================
 *  @const  CORE_CM_BASE, MAILBOX_BASE
 *
 *  @desc   Mailbox interrupt related System configuration addresses
 *  ============================================================================
 */
#define CORE_CM_BASE           0x48004A00
#define MAILBOX_BASE           0x48094000

/*  ============================================================================
 *  @const  HAL_MAILBOX_BASE, HAL_CORECM_BASE
 *
 *  @desc   Mailbox interrupt related System configuration addresses as mapped
 *          in the Linux base port.
 *  ============================================================================
 */
#define HAL_CORECM_BASE               IO_ADDRESS(CORE_CM_BASE)
#define HAL_MAILBOX_BASE              IO_ADDRESS(MAILBOX_BASE)


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (HAL_PLATFORM_H) */
