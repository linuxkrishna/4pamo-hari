/** ============================================================================
 *  @file   gen_utils.h
 *
 *  @path   $(DSPLINK)/gpp/src/gen/
 *
 *  @desc   Platform independent common library function interface.
 *
 *  @ver    1.51.00.08
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (GEN_H)
#define GEN_H


/*  ----------------------------------- DSP/BIOS Link               */
#include <ipc.h>
#include <notifyerr.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   GEN_init
 *
 *  @desc   Initializes the GEN module's private state.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  None
 *
 *  @leave  Subcomponent is initialized.
 *
 *  @see    GEN_Finalize
 *  ============================================================================
 */

signed long int
GEN_init (void) ;


/** ============================================================================
 *  @func   GEN_exit
 *
 *  @desc   Discontinues usage of the GEN module.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *
 *  @enter  Subcomponent must be initialized.
 *
 *  @leave  None
 *
 *  @see    GEN_Initialize
 *  ============================================================================
 */

signed long int
GEN_exit (void) ;


/** ============================================================================
 *  @func   GEN_numToAscii
 *
 *  @desc   Converts a 1 or 2 digit number to a 2 digit string.
 *
 *  @arg    number
 *              Number to convert.
 *  @arg    strNumber
 *              Buffer to store converted string.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The number to convert must be between 0 and 99, both numbers
 *          included.
 *          The buffer to store output string must be valid.
 *
 *  @leave  The buffer to store output string is valid.
 *
 *  @see    None
 *  ============================================================================
 */

signed long int
GEN_numToAscii (IN unsigned long int number, OUT char * strNumber) ;


/** ============================================================================
 *  @func   GEN_strncmp
 *
 *  @desc   Compares 2 ASCII strings.  Works the same way as stdio's strcmp.
 *
 *  @arg    string1
 *              First string for comparison.
 *  @arg    string2
 *              Second string for comparison.
 *  @arg    cmpResult
 *              Result of comparision (zero = equal, non-zero otherwise).
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The buffer to store first string must be valid.
 *          The buffer to store second string must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */

signed long int
GEN_strncmp (IN  CONST char * string1,
             IN  CONST char * string2,
             IN  unsigned long int        size) ;

#define GEN_strcmp(string1,string2) GEN_Strncmp(string1,string2,0)

/** ============================================================================
 *  @func   GEN_strncpy
 *
 *  @desc   Safe strcpy function.
 *
 *  @arg    destination
 *              destination buffer.
 *  @arg    source
 *              Source buffer.
 *  @arg    maxNum
 *              Number of characters to copy.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The destination buffer must be valid.
 *          The source buffer must be valid.
 *          The number of characters to copy must be more than zero.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */

signed long int
GEN_strncpy (IN  char * destination,
             IN  char * source,
             IN  signed long int   maxNum) ;


/** ============================================================================
 *  @func   GEN_strlen
 *
 *  @desc   Determines the length of a null terminated ASCI string.
 *
 *  @arg    strSrc
 *              Pointer to string.
 *  @arg    length
 *              Out parameter to hold the length of string.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The pointer to the string buffer must be valid.
 *          The pointer to the length field must be valid.
 *
 *  @leave  The pointer to the length field is valid.
 *
 *  @see    None
 *  ============================================================================
 */

signed long int
GEN_strlen (IN CONST char * strSrc, OUT unsigned long int * length) ;


/** ============================================================================
 *  @func   GEN_wcharToAnsi
 *
 *  @desc   Converts a wide char string to an ansi string.
 *
 *  @arg    destination
 *              Destination buffer.
 *  @arg    source
 *              Source buffer.
 *  @arg    numChars
 *              Number of characters (Wide chars) to be converted/copied.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The destination buffer must be valid.
 *          The source buffer must be valid.
 *          The number of characters to be converted/copied must be greater
 *          than 0.
 *
 *  @leave  The destination buffer is valid.
 *
 *  @see    None
 *  ============================================================================
 */

signed long int
GEN_wcharToAnsi (OUT char  * destination,
                 IN  short * source,
                 IN  signed long int    numChars) ;


/** ============================================================================
 *  @func   GEN_ansiToWchar
 *
 *  @desc   Converts an ANSI string to a wide char string.
 *
 *  @arg    destination
 *              Destination buffer.
 *  @arg    source
 *              Source buffer.
 *  @arg    numChars
 *              Number of characters (Ansi) to be converted/copied.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The destination buffer must be valid.
 *          The source buffer must be valid.
 *          The number of characters to be converted/copied must be greater
 *          than 0.
 *
 *  @leave  The destination buffer is valid.
 *
 *  @see    None
 *  ============================================================================
 */

signed long int
GEN_ansiToWchar (OUT short * destination,
                 IN  char *  source,
                 IN  signed long int    numChars) ;


/** ============================================================================
 *  @func   GEN_wstrlen
 *
 *  @desc   Determines the length of a null terminated wide character string.
 *
 *  @arg    strSrc
 *              pointer to string.
 *  @arg    length
 *              Length of string.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  Subcomponent must be initialized.
 *          The pointer to the string buffer must be valid.
 *          The pointer to length of buffer must be valid.
 *
 *  @leave  The pointer to length of buffer is valid.
 *
 *  @see    None
 *  ============================================================================
 */

signed long int
GEN_wstrlen (IN short * strSrc, IN unsigned long int * length) ;


/** ============================================================================
 *  @func   GEN_strncat
 *
 *  @desc   Safe strcat function.
 *
 *  @arg    destination
 *              destination buffer.
 *  @arg    source
 *              Source buffer.
 *  @arg    maxNum
 *              Maximum length of the destination buffer after concatenation.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
 *          DSP_EINVALIDARG
 *              Failure due to invalid argument.
 *
 *  @enter  The destination buffer must be valid.
 *          The source buffer must be valid.
 *          The number of characters to copy must be more than zero.
 *
 *  @leave  None.
 *
 *  @see    None
 *  ============================================================================
 */

signed long int
GEN_strncat (OUT char * destination,
             IN  char * source,
             IN  signed long int   maxNum) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (GEN_H) */

