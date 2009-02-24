/** ============================================================================
 *  @file   dpc.h
 *
 *  @path   $(DSPLINK)/gpp/src/osal/
 *
 *  @desc   Defines the interfaces and data structures for the
 *          sub-component DPC.
 *
 *  @ver    1.51.00.08
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (DPC_H)
#define DPC_H

/*  ----------------------------------- DSP/BIOS Link               */
#include <ipc.h>
#include <errbase.h>
/*  ----------------------------------- Trace & Debug               */
#include <_trace.h>


#if defined (__cplusplus)
extern "C" {
#endif

/** ============================================================================
 *  @name   struct DpcObject
 *
 *  @desc   Forward declaration for struct DpcObject. Actual definition is dependent
 *          on OS and is defined in the corresponding C file.
 *  ============================================================================
 */


/** ============================================================================
 *  @func   FnDpcProc
 *
 *  @desc   Function prototype for DPC function. The user defined functions
 *          that is to be invoked as a DPC should conform to this signature.
 *
 *  @arg    refData
 *             Argument to be passed to DPC call.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    DPC_Callback, DPC_Create
 *  ============================================================================
 */
typedef void (*FnDpcProc) (void * refData) ;


/** ============================================================================
 *  @name   struct DpcObject_tag
 *
 *  @desc   Montavista Linux specific definition of struct DpcObject.
 *          The type for this structure is defined in dpc.h as a forward
 *          declaration.
 *
 *  @field  signature
 *              Signature of the DPC object.
 *  @field  index
 *              Index of the DPC object.
 *  @field  paramData
 *              Parameter data to be passed to the user defined deferred
 *              function call.
 *  @field  userDPCFn
 *              user defined function to be called as a deferred function call.
 *  @field  pendCount
 *              Atomic variable indicating number of DPC pending for execution.
 *  @field  state
 *              Flag indicating state of DPC (disabled or enabled). FALSE means
 *              disabled and TRUE means enabled.
 *
 *  @see    DpcTaskletInfo
 *  ============================================================================
 */
struct DpcObject {
    unsigned long int    signature    ;
    unsigned long int    index        ;
    void *    paramData    ;
    FnDpcProc userDPCFn    ;
    atomic_t  pendCount    ;
    short int      state        ;
} ;






/** ============================================================================
 *  @func   DPC_init
 *
 *  @desc   Initializes the DPC module. It initializes the global area for
 *          holding all the DPC objects.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory error.
 *
 *  @enter  None
 *
 *  @leave  DPC must be initialized.
 *
 *  @see    DPC_exit
 *  ============================================================================
 */

signed long int
DPC_init (void) ;

/** ============================================================================
 *  @deprecated The deprecated function DPC_Initialize has been replaced
 *              with DPC_init.
 *  ============================================================================
 */
#define DPC_Initialize DPC_init


/** ============================================================================
 *  @func   DPC_exit
 *
 *  @desc   Releases all resources used by this sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory error.
 *
 *  @enter  DPC must be initialized.
 *
 *  @leave  All in-use DPC objects are released.
 *
 *  @see    DPC_init
 *  ============================================================================
 */

signed long int
DPC_exit (void) ;

/** ============================================================================
 *  @deprecated The deprecated function DPC_Finalize has been replaced
 *              with DPC_exit.
 *  ============================================================================
 */
#define DPC_Finalize DPC_exit


/** ============================================================================
 *  @func   DPC_Create
 *
 *  @desc   Creates a DPC object and returns it after populating relevant
 *          fields.
 *
 *  @arg    userDPCFn
 *             User specified function to be invoked as DPC.
 *  @arg    dpcArgs
 *             Arguments to be passed to the DPC.
 *  @arg    dpcObj
 *             OUT argument to store the DPC object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_ERESOURCE
 *              No more DPC objects are available for creation.
 *          DSP_EINVALIDARG
 *              Invalid arguments.
 *
 *  @enter  DPC must be initialized.
 *          userDPCFn must be a valid function.
 *          dpcObj must be a valid pointer.
 *
 *  @leave  *dpcObj points to an initialized DPC Object on successful
 *          completion or *dpcObj is NULL on failure.
 *
 *  @see    DPC_Delete, DPC_Schedule
 *  ============================================================================
 */

signed long int
DPC_Create (IN  FnDpcProc       userDPCFn,
            IN  void *           dpcArgs,
            OUT struct DpcObject **    dpcObj) ;


/** ============================================================================
 *  @func   DPC_Delete
 *
 *  @desc   Deletes the DPC object.
 *
 *  @arg    dpcObj
 *             The DPC object to be deleted.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EPOINTER
 *              Invalid dpcObj object.
 *          DSP_EINVALIDARG
 *              Incorrect dpcObj specified.
 *
 *  @enter  DPC must be initialized.
 *          dpcObj must be a valid DPC object.
 *
 *  @leave  Upon successful completion the dpcObj is reset.
 *
 *  @see    DPC_Create
 *  ============================================================================
 */

signed long int
DPC_Delete (IN struct DpcObject * dpcObj) ;


/** ============================================================================
 *  @func   DPC_Cancel
 *
 *  @desc   Cancels any pending DPCs associated to dpcObj.
 *
 *  @arg    dpcObj
 *             The DPC object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EPOINTER
 *              Invalid dpcObj object.
 *          DSP_EINVALIDARG
 *              Incorrect dpcObj specified.
 *
 *  @enter  DPC must be initialized.
 *          dpcObj must be a valid DPC object.
 *
 *  @leave  All pending calls to the DPC are cancelled.
 *
 *  @see    DPC_Schedule, DPC_Create
 *  ============================================================================
 */

signed long int
DPC_Cancel (IN struct DpcObject * dpcObj) ;


/** ============================================================================
 *  @func   DPC_Schedule
 *
 *  @desc   Schedules the user defined function associated with dpcObj to be
 *          invoked at a later point of time.
 *
 *  @arg    dpcObj
 *             The DPC object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EPOINTER
 *              Invalid dpcObj object.
 *          DSP_EINVALIDARG
 *              Incorrect dpcObj specified.
 *
 *  @enter  DPC must be initialized.
 *          dpcObj must be a valid DPC object.
 *
 *  @leave  None
 *
 *  @see    DPC_Create, DPC_Cancel, DPC_Callback
 *  ============================================================================
 */

signed long int
DPC_Schedule (IN struct DpcObject * dpcObj) ;


/** ============================================================================
 *  @func   DPC_Disable
 *
 *  @desc   Disables execution of DPCs.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */

void
DPC_Disable (void) ;


/** ============================================================================
 *  @func   DPC_Enable
 *
 *  @desc   Enables execution of DPCs.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */

void
DPC_Enable (void) ;


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   DPC_Debug
 *
 *  @desc   Prints the current status of DPC objects.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *  ============================================================================
 */

void
DPC_Debug (void) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (DPC_H) */

