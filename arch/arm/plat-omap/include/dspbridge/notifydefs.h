/** ============================================================================
 *  @file   notifydefs.h
 *
 *  @path   $(NOTIFY)/include
 *
 *  @desc   Defines data types and structures used by Notify module
 *
 *  @ver    1.00.00.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (NOTIFYDEFS_H)
#define NOTIFYDEFS_H


/*  ----------------------------------- IPC */
//#include <ipctypes.h>

/*  ----------------------------------- Notify */
#include <dspbridge/notifyerr.h>
#include <dspbridge/notifytypes.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  NOTIFY_MAX_DRIVERS
 *
 *  @desc   Maximum number of Notify drivers supported.
 *          NOTE: This can be modified by the user if data section size needs to
 *                be reduced, or if there is a need for more than the defined
 *                max drivers.
 *  ============================================================================
 */
#define NOTIFY_MAX_DRIVERS   16

/** ============================================================================
 *  @const  NOTIFY_MAX_NAMELEN
 *
 *  @desc   Maximum length of the name of Notify drivers, inclusive of NULL
 *          string terminator.
 *  ============================================================================
 */
#define NOTIFY_MAX_NAMELEN   32

#if 0
/** ============================================================================
 *  @name   Notify_Handle
 *
 *  @desc   This typedef defines the type for the handle to the Notify driver.
 *  ============================================================================
 */
typedef void * Notify_Handle ;


/** ============================================================================
 *  @name   FnNotifyCbck
 *
 *  @desc   Signature of the callback function to be registered with the NOTIFY
 *          component.
 *
 *  @arg    procId
 *              Processor ID from which the event is being received
 *  @arg    eventNo
 *              Event number for which the callback is being received
 *  @arg    arg
 *              Fixed argument registered with the IPS component along with
 *              the callback function.
 *  @arg    payload
 *              Run-time information provided to the upper layer by the Notify
 *              component. Depending on the Notify driver implementation, this
 *              may or may not pass on a user-specific value to the registered
 *              application
 *
 *  @ret    None.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
typedef void (*FnNotifyCbck) (IN     Processor_Id procId,
                              IN     u32       eventNo,
                              IN  void *       arg,
                              IN  u32       payload) ;


/** ============================================================================
 *  @name   Notify_Attrs
 *
 *  @desc   This structure defines attributes for initialization of the Notify
 *          module.
 *
 *  @field  maxDrivers
 *              Maximum number of drivers that can be registered with the Notify
 *              module.
 *
 *  @see    Notify_init ()
 *  ============================================================================
 */
typedef struct Notify_Attrs_tag {
    u32   maxDrivers ;
} Notify_Attrs ;

/** ============================================================================
 *  @name   Notify_Config
 *
 *  @desc   This structure defines the configuration structure for
 *          initialization of the Notify driver.
 *
 *  @field  driverAttrs
 *              Notify driver-specific attributes
 *
 *  @see    Notify_driverInit ()
 *  ============================================================================
 */
typedef struct Notify_Config_tag {
    void *   driverAttrs ;
} Notify_Config ;

#endif

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (NOTIFYDEFS_H) */
