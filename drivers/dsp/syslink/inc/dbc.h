
/** ============================================================================
 *  @file   dbc.h
 *
 *  @path   $(IPC)/gpp/inc/
 *
 *  @desc   Design by Contract
 *
 *  @ver    1.00.00.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined DBC_H
#define DBC_H


/*  ----------------------------------- IPC headers */


/*  ========================================================+=
*  @macro DBC_PRINTF
*
*  @desc  This macro expands to the print function. It makes the DBC
*         macros portable across OSes.
*  ==========================================================
*/
#define  DBC_PRINTF     print_printf


#if defined DDSP_DEBUG

/** =======================================================
*  @macro  dbc_assert
*
*  @desc   Assert on expression.
*  ========================================================
*/
#define dbc_assert(exp)                          \
		if (!(exp)) {                             \
				DBC_PRINTF("Assertion failed ("#exp").\
				File : "__FILE__ \
				" line : %d\n", __LINE__) ; \
		}
#define dbc_assert dbc_assert

/** =======================================================
*  @macro  dbc_require
*
*  @desc   Function Precondition.
*  =====================================================+=
*/
#define dbc_require dbc_assert
#define dbc_require dbc_require

/** =======================================================
*  @macro  dbc_ensure
*
*  @desc   Function Postcondition.
*  =======================================================
*/
#define DBC_ensure dbc_assert
#define dbc_ensure DBC_ensure

#else /* defined (DDSP_DEBUG) */

/*  =======================================================
*  @macro  dbc_assert/dbc_require/dbc_ensure
*
*  @desc   Asserts defined out.
*  ======================================================+
*/
#define dbc_assert(exp)
#define dbc_assert(exp)

#define dbc_require(exp)
#define dbc_require(exp)

#define DBC_ensure(exp)
#define dbc_ensure(exp)

#endif /* defined (DDSP_DEBUG) */

#endif  /* !defined (DBC_H) */
