/** ============================================================================
 *  @file   _intobject.h
 *
 *  @path   $(DSPLINK)/gpp/inc/Linux/
 *
 *  @desc   Declares an object that encapsulates the interrupt information
 *          reqiured by Linux.
 *
 *  @ver    1.51.00.08
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (_INTOBJECT_H)
#define _INTOBJECT_H


/*  ----------------------------------- DSP/BIOS Link                   */
#include <gpptypes.h>


#if defined (__cplusplus)
extern "C" {
#endif


/** ============================================================================
 *  @name   InterruptObject
 *
 *  @desc   Object encapsulating the OS dependent interrupt information.
 *
 *  @field  intId
 *              Interrupt identifier
 *  ============================================================================
 */
 struct InterruptObject {
    signed long int  intId ;
} ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (_INTOBJECT_H) */

