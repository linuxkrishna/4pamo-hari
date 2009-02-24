/** ============================================================================
 *  @file   gen_utils.c
 *
 *  @path   $(DSPLINK)/gpp/src/gen/
 *
 *  @desc   Implementation of platform independent common library
 *          functions interface.
 *
 *  @ver    1.51.00.08
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- IPC headers */
#include <ipc.h>

/*  ----------------------------------- Trace & Debug */
#include <_trace.h>

/*  ----------------------------------- Generic Functions */
#include <gen_utils.h>
#include <list.h>
#include <errbase.h>
#include <gpptypes.h>



#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and subcomponent identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_GEN_UTILS

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#if defined (DDSP_DEBUG)
#define SET_FAILURE_REASON(status)   \
                            TRC_SetReason (status, FID_C_GEN_UTILS, __LINE__)
#else
#define SET_FAILURE_REASON(status)
#endif /* if defined (DDSP_DEBUG) */


/** ============================================================================
 *  @func   GEN_init
 *
 *  @desc   Initializes the GEN module's private state.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
GEN_init (void)
{
    signed long int status = DSP_SOK ;

    TRC_0ENTER ("GEN_init") ;

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON (status) ;
    }

    TRC_1LEAVE ("GEN_init", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_exit
 *
 *  @desc   Discontinues usage of the GEN module.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
GEN_exit (void)
{
    signed long int status = DSP_SOK ;

    TRC_0ENTER ("GEN_exit") ;

    TRC_1LEAVE ("GEN_exit", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_numToAscii
 *
 *  @desc   Converts a 1 or 2 digit number to a 2 digit string.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
GEN_numToAscii (IN unsigned long int number, OUT char * strNumber)
{
    signed long int  status = DSP_SOK ;
    char       tens   = '0'     ;

    TRC_2ENTER ("GEN_numToAscii", number, strNumber) ;

    DBC_Require (number <= 99) ;
    DBC_Require (strNumber != NULL) ;

    if ((number > 99) || (strNumber == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON (status) ;

    }
    else {
        tens = (char) (number / 10) ;
        number = number % 10 ;
        if (tens != 0) {
            strNumber [0] = (char) (tens + '0') ;
            strNumber [1] = (char) (number + '0') ;
            strNumber [2] = '\0' ;
        }
        else {
            strNumber [0] = (char) (number + '0') ;
            strNumber [1] = '\0' ;
        }
    }

    DBC_Ensure (strNumber != NULL) ;

    TRC_1LEAVE ("GEN_numToAscii", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_strncmp
 *
 *  @desc   Compares 2 ASCII strings.  Works the same way as stdio's strcmp.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
GEN_strncmp (IN  CONST char * string1,
             IN  CONST char * string2,
             IN  unsigned long int        size)
{
    signed long int     status         = DSP_SOK ;
    signed long int          charCompare    = 0 ;
    unsigned long int         string1Length  = 0 ;
    unsigned long int         string2Length  = 0 ;
    unsigned long int         charCount      = 0 ;

    TRC_3ENTER ("GEN_strncmp", string1, string2, size) ;

    DBC_Require (string1 != NULL) ;
    DBC_Require (string2 != NULL) ;

    if ((string1 == NULL) || (string2 == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON (status) ;
    }
    else {
        status = GEN_strlen (string1, &string1Length) ;
        DBC_Assert (DSP_SUCCEEDED (status)) ;

        status = GEN_strlen (string2, &string2Length) ;
        DBC_Assert (DSP_SUCCEEDED (status)) ;

        if (string1Length == string2Length) {
            /* iterate for comparing the strings - character by character
             * Note: The comparison takes care of termination of both strings
             *  - by explicitly checking for string termination of string2
             *  - the check for string1 termination is implicit, since when
             *    string1 terminates with a NULL ('\0') the charCompare value
             *    will be negative (if string2 also doesn't terminate then).
             */
            charCompare =  ((*(const unsigned char *) ((void *) string1))
                    - (*(const unsigned char *) ((void *) string2))) ;
            while (     (charCompare == 0)
                   &&   (*string2    != '\0')
                   &&   ((size == 0) || (charCount == size))) {
                charCompare =  ((*(const unsigned char *) ((void *) string1))
                        - (*(const unsigned char *)  ((void *) string2))) ;
                ++string1 ;
                ++string2 ;
                charCount++ ;
            }

            if (charCompare < 0) {
                charCompare = -1 ;
            }
            else if (charCompare > 0) {
                charCompare = 1 ;
            }
        }
        else {
            if (string1Length < string2Length) {
                charCompare = -1 ;
            }
            else {
                charCompare = 1 ;
            }
        }
    }

    TRC_1LEAVE ("GEN_strncmp", status) ;

    return charCompare ;
}


/** ============================================================================
 *  @func   GEN_strncpy
 *
 *  @desc   Safe strcpy function.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
GEN_strncpy (OUT char * destination,
             IN  char * source,
             IN  signed long int   maxNum)
{
    signed long int     status = DSP_SOK     ;
    char *        dest   = NULL        ;
    CONST char *  src    = NULL        ;

    TRC_3ENTER ("GEN_strncpy", destination, source, maxNum) ;

    DBC_Require (destination != NULL) ;
    DBC_Require (source != NULL) ;
    DBC_Require (maxNum >= 0) ;

    if ((destination == NULL) || (source == NULL) || (maxNum < 0)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON (status) ;
    }
    else {
        dest   = destination ;
        src    = source      ;

        DBC_Assert (src != NULL) ;
        DBC_Assert (dest != NULL) ;

        while ((maxNum != 0) && (*src != '\0')) {
            *dest++ = *src++ ;
            maxNum-- ;
        }

        if (maxNum != 0) {
            /* pad out with zeroes */
            while (--maxNum) {
                *dest++ = '\0' ;
            }
        }
        else {
            *dest = '\0' ;
        }
    }

    TRC_1LEAVE ("GEN_strncpy", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_strlen
 *
 *  @desc   Determines the length of a null terminated ASCI string.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
GEN_strlen (IN CONST char * strSrc, OUT unsigned long int * length)
{
    signed long int     status = DSP_SOK ;
    CONST char *  str    = NULL    ;

    TRC_2ENTER ("GEN_strlen", strSrc, length) ;

    DBC_Require (strSrc != NULL) ;
    DBC_Require (length != NULL) ;

    if ((strSrc == NULL) || (length == NULL)) {
        status = DSP_EINVALIDARG ;
    }
    else {
        str = strSrc  ;
        DBC_Assert (str != NULL) ;

        while (*str++)
        {
            /* Added for MISRAC compliance */
        }

        *length = (unsigned long int) (str - strSrc - 1) ;
    }

    TRC_1LEAVE ("GEN_strlen", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_wcharToAnsi
 *
 *  @desc   Converts a wide char string to an ansi string.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
GEN_wcharToAnsi (OUT char *   destination,
                 IN  short *  source,
                 IN  signed long int     numChars)
{
    signed long int  status = DSP_SOK     ;
    char *     temp   = NULL        ;

    TRC_3ENTER ("GEN_wcharToAnsi", destination, source, numChars) ;

    DBC_Require (destination != NULL) ;
    DBC_Require (source != NULL) ;
    DBC_Require (numChars >= 0) ;

    if ((source == NULL) || (destination == NULL) || (numChars == 0)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON (status) ;
    }
    else  {
        temp = destination ;
        DBC_Assert (temp != NULL) ;
        while ((*source != '\0') && (numChars > 0)) {
            *temp++= (char) *source++ ;
            numChars-- ;
        }
        *temp = '\0' ;
    }

    TRC_1LEAVE ("GEN_wcharToAnsi", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_ansiToWchar
 *
 *  @desc   Converts an ANSI string to a wide char string.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
GEN_ansiToWchar (OUT short *  destination,
                 IN  char *   source,
                 IN  signed long int     numChars)
{
    signed long int status = DSP_SOK ;

    TRC_3ENTER ("GEN_ansiToWchar", destination, source, numChars) ;

    DBC_Require (destination != NULL) ;
    DBC_Require (source != NULL) ;
    DBC_Require (numChars >= 0) ;

    if ((source == NULL) || (destination == NULL) || (numChars == 0)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON (status) ;
    }
    else {
        while ((*source != '\0') && (numChars > 0)) {
            *destination++= (short) *source++ ;
            numChars-- ;
        }
        *destination = (short) '\0' ;
    }

    TRC_1LEAVE ("GEN_ansiToWchar", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_wstrlen
 *
 *  @desc   Determines the length of a null terminated wide character string.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
GEN_wstrlen (IN short * strSrc, OUT unsigned long int * length)
{
    signed long int      status = DSP_SOK ;
    CONST short *  str    = NULL ;

    TRC_2ENTER ("GEN_wstrlen", strSrc, length) ;

    DBC_Require (strSrc != NULL) ;
    DBC_Require (length != NULL );

    if ((strSrc == NULL) || (length == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON (status) ;
    }
    else {
        str = strSrc  ;
        DBC_Assert (str != NULL) ;
        while (*str++)
        {
        }
        *length = str - strSrc - 1 ;
    }

    TRC_1LEAVE ("GEN_wstrlen", status) ;

    return status ;
}


/** ============================================================================
 *  @func   GEN_strncat
 *
 *  @desc   Safe strcat function.
 *
 *  @modif  None
 *  ============================================================================
 */

signed long int
GEN_strncat (OUT char * destination,
             IN  char * source,
             IN  signed long int   maxNum)
{
    signed long int     status = DSP_SOK     ;
    char *        dest   = NULL        ;
    CONST char *  src    = NULL        ;

    TRC_3ENTER ("GEN_strncat", destination, source, maxNum) ;

    DBC_Require (destination != NULL) ;
    DBC_Require (source != NULL) ;
    DBC_Require (maxNum >= 0) ;

    if ((destination == NULL) || (source == NULL) || (maxNum < 0)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON (status) ;
    }
    else {
        dest   = destination ;
        src    = source      ;
        DBC_Assert (dest != NULL) ;
        DBC_Assert (src != NULL) ;

        while ((maxNum != 0) && (*dest != '\0')) {
            dest++ ;
            maxNum-- ;
        }
        while ((maxNum != 0) && (*src != '\0')) {
            *dest++ = *src++ ;
            maxNum-- ;
        }

        if (maxNum != 0) {
            /* pad out with zeroes */
            while (--maxNum) {
                *dest++ = '\0' ;
            }
        }
        else {
            *dest = '\0' ;
        }
    }

    TRC_1LEAVE ("GEN_strncat", status) ;

    return status ;
}

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
