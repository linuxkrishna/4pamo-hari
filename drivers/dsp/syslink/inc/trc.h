/** ============================================================================
 *  @file   trc.h
 *
 *  @path   $(DSPLINK)/gpp/src/gen/
 *
 *  @desc   Defines the interfaces and data structures for the
 *          sub-component TRC.
 *
 *  @ver    1.51.00.08
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2008
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TRC_H)
#define TRC_H


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

#include <notifyerr.h>


/** ============================================================================
 *  @const  MAXIMUM_COMPONENTS
 *
 *  @desc   maximum number of components supported.
 *  ============================================================================
 */
#define MAXIMUM_COMPONENTS         16u

/** ============================================================================
 *  @const  TRC_ENTER/TRC_LEVELn/TRC_LEAVE
 *
 *  @desc   Severity levels for debug printing.
 *  ============================================================================
 */
#define TRC_ENTER           0x01u       /*  Lowest level of severity */
#define TRC_LEVEL1          0x02u
#define TRC_LEVEL2          0x03u
#define TRC_LEVEL3          0x04u
#define TRC_LEVEL4          0x05u
#define TRC_LEVEL5          0x06u
#define TRC_LEVEL6          0x07u
#define TRC_LEVEL7          0x08u      /*  Highest level of severity */
#define TRC_LEAVE           TRC_ENTER


/** ============================================================================
 *  @name   ErrorInfo
 *
 *  @desc   Structure for storing error reason.
 *
 *  @field  IsSet
 *              Flag to indicate error is set.
 *  @field  ErrCode
 *              Error Code.
 *  @field  OsMajor
 *              OS  Version Major version number.
 *  @field  OsMinor
 *              OS  Version Minor version number.
 *  @field  OsBuild
 *              OS  Version Build number.
 *  @field  PddMajor
 *              PDD Version Major version number.
 *  @field  PddMinor
 *              PDD Version Minor version number.
 *  @field  PddBuild
 *              PDD Version Build number.
 *  @field  FileId
 *              ID of the file where failure occured.
 *  @field  LineNum
 *              Line number where failure occured.
 *  ============================================================================
 */
struct ErrorInfo {
    short int       IsSet    ;

    signed long int ErrCode  ;

    signed long int      OsMajor  ;
    signed long int      OsMinor  ;
    signed long int      OsBuild  ;

    signed long int      PddMajor ;
    signed long int      PddMinor ;
    signed long int      PddBuild ;

    signed long int      FileId   ;
    signed long int      LineNum  ;
} ;


/** ============================================================================
 *  @func   TRC_SetReason
 *
 *  @desc   This function logs failure if no previous failure has been logged.
 *
 *  @arg    status
 *              Error status to be logged.
 *  @arg    FileId
 *              File identifier.
 *  @arg    Line
 *              Line number where error occurred.
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
TRC_SetReason (signed long int status, signed long int FileId, signed long int Line) ;

#if defined (TRACE_ENABLE)

#if defined (TRACE_KERNEL)

/** ============================================================================
 *  @macro  TRC_ENABLE
 *
 *  @desc   Wrapper for function TRC_Enable ().
 *  ============================================================================
 */
#define TRC_ENABLE(map)             TRC_enable (map)

/** ============================================================================
 *  @macro  TRC_DISABLE
 *
 *  @desc   Wrapper for function TRC_Disable ().
 *  ============================================================================
 */
#define TRC_DISABLE(map)            TRC_disable (map)

/** ============================================================================
 *  @macro  TRC_SET_SEVERITY
 *
 *  @desc   Wrapper for function TRC_SetSeverity ().
 *  ============================================================================
 */
#define TRC_SET_SEVERITY(level)     TRC_setSeverity (level)

/** ============================================================================
 *  @macro  TRC_nPRINT
 *
 *  @desc   Uses corresponding TRC_nPrint function to print debug strings and
 *          optional arguments.
 *  ============================================================================
 */

#define TRC_0PRINT(a,b)                                           \
    TRC_0print (COMPONENT_ID, (a), (char *)(b))

#define TRC_1PRINT(a,b,c)                                         \
    TRC_1print (COMPONENT_ID, (a), (char *) (b), (unsigned long int) (c))

#define TRC_2PRINT(a,b,c,d)                                       \
    TRC_2print (COMPONENT_ID, (a), (char *) (b), (unsigned long int) (c),   \
                                              (unsigned long int) (d))

#define TRC_3PRINT(a,b,c,d,e)                                     \
    TRC_3print (COMPONENT_ID, (a), (char *) (b), (unsigned long int) (c),   \
                                              (unsigned long int) (d),       \
                                              (unsigned long int) (e))

#define TRC_4PRINT(a,b,c,d,e,f)                                   \
    TRC_4print (COMPONENT_ID, (a), (char *) (b), (unsigned long int) (c),   \
                                              (unsigned long int) (d),       \
                                              (unsigned long int) (e),       \
                                              (unsigned long int) (f))

#define TRC_5PRINT(a,b,c,d,e,f,g)                                 \
    TRC_5print (COMPONENT_ID, (a), (char *) (b), (unsigned long int) (c),   \
                                              (unsigned long int) (d),       \
                                              (unsigned long int) (e),       \
                                              (unsigned long int) (f),       \
                                              (unsigned long int) (g))

#define TRC_6PRINT(a,b,c,d,e,f,g,h)                               \
    TRC_6print (COMPONENT_ID, (a), (char *) (b), (unsigned long int) (c),   \
                                              (unsigned long int) (d),       \
                                              (unsigned long int) (e),       \
                                              (unsigned long int) (f),       \
                                              (unsigned long int) (g),       \
                                              (unsigned long int) (h))

/** ============================================================================
 *  @name   TrcObject
 *
 *  @desc   TRC Object that stores the severity and component and
 *          subcomponent maps on a global level.
 *
 *  @field  components
 *              component map
 *  @field  level
 *              severity level
 *  @field  subcomponents
 *              subcomponent map
 *  ============================================================================
 */
struct TrcObject {
    unsigned short int components ;
    unsigned short int level      ;
    unsigned short int subcomponents [MAXIMUM_COMPONENTS] ;
} ;

/** ============================================================================
 *  @func   TRC_enable
 *
 *  @desc   Enables debug prints on a component and subcomponent level.
 *
 *  @arg    componentMap
 *             The component & subcomponent map
 *
 *  @ret    DSP_SOK
 *              Operation successful
 *          DSP_EINVALIDARG
 *              Invalid argument to function call
 *          DSP_EFAIL
 *              Operation not successful
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    TRC_Disable, TRC_SetSeverity
 *  ============================================================================
 */

signed long int
TRC_enable (IN unsigned long int componentMap);


/** ============================================================================
 *  @func   TRC_disable
 *
 *  @desc   Disables debug prints on a component and subcomponent level.
 *
 *  @arg    componentMap
 *             The component & subcomponent map
 *
 *  @ret    DSP_SOK
 *              Operation successful
 *          DSP_EINVALIDARG
 *              Invalid argument to function call
 *          DSP_EFAIL
 *              Operation not successful
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    TRC_Enable, TRC_SetSeverity
 *  ============================================================================
 */

signed long int
TRC_disable (IN unsigned long int componentMap);


/** ============================================================================
 *  @func   TRC_setSeverity
 *
 *  @desc   set the severity of the required debug prints.
 *
 *  @arg    level
 *             The severity level of the debug prints required
 *
 *  @ret    DSP_SOK
 *              Operation successful
 *          DSP_EINVALIDARG
 *              Invalid argument to function call
 *          DSP_EFAIL
 *              Operation not successful
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    TRC_Enable, TRC_Disable
 *  ============================================================================
 */

signed long int
TRC_setSeverity (IN unsigned short int   level) ;

/** ============================================================================
 *  @func   TRC_0print
 *
 *  @desc   Prints a null terminated character string based on its severity,
 *          the subcomponent and component it is associated with.
 *
 *  @arg    componentMap
 *             The component & subcomponent to which this print belongs
 *  @arg    severity
 *             The severity associated with the print
 *  @arg    debugString
 *             The null terminated character string to be printed
 *
 *  @ret    None
 *
 *  @enter  The character string is valid
 *
 *  @leave  None
 *
 *  @see    TRC_1Print, TRC_2Print, TRC_3Print, TRC_4Print, TRC_5Print,
 *          TRC_6Print
 *  ============================================================================
 */

void
TRC_0print (IN  unsigned long int   componentMap,
            IN  unsigned short int   severity,
            IN  char *  debugString) ;


/** ============================================================================
 *  @func   TRC_1print
 *
 *  @desc   Prints a null terminated character string and an integer argument
 *          based on its severity, the subcomponent and component it is
 *          associated  with.
 *
 *  @arg    componentMap
 *             The component & subcomponent to which this print belongs
 *  @arg    severity
 *             The severity associated with the print
 *  @arg    debugString
 *             The null terminated character string to be printed
 *  @arg    argument1
 *             The integer argument to be printed
 *
 *  @ret    None
 *
 *  @enter  The character string is valid
 *
 *  @leave  None
 *
 *  @see    TRC_0Print, TRC_2Print, TRC_3Print, TRC_4Print, TRC_5Print,
 *          TRC_6Print
 *  ============================================================================
 */

void
TRC_1print (IN  unsigned long int   componentMap,
            IN  unsigned short int   severity,
            IN  char *  debugString,
            IN  unsigned long int   argument1) ;


/** ============================================================================
 *  @func   TRC_2print
 *
 *  @desc   Prints a null terminated character string and two integer arguments
 *          based on its severity, the subcomponent and component it is
 *          associated  with.
 *
 *  @arg    componentMap
 *             The component & subcomponent to which this print belongs
 *  @arg    severity
 *             The severity associated with the print
 *  @arg    debugString
 *             The null terminated character string to be printed
 *  @arg    argument1
 *             The first integer argument to be printed
 *  @arg    argument2
 *             The second integer argument to be printed
 *
 *  @ret    None
 *
 *  @enter  The character string is valid
 *
 *  @leave  None
 *
 *  @see    TRC_0Print, TRC_1Print, TRC_3Print, TRC_4Print, TRC_5Print,
 *          TRC_6Print
 *  ============================================================================
 */

void
TRC_2print (IN  unsigned long int   componentMap,
            IN  unsigned short int   severity,
            IN  char *  debugString,
            IN  unsigned long int   argument1,
            IN  unsigned long int   argument2) ;


/** ============================================================================
 *  @func   TRC_3print
 *
 *  @desc   Prints a null terminated character string and three integer
 *          arguments based on its severity, the subcomponent and component it
 *          is associated  with.
 *
 *  @arg    componentMap
 *             The component & subcomponent to which this print belongs
 *  @arg    severity
 *             The severity associated with the print
 *  @arg    debugString
 *             The null terminated character string to be printed
 *  @arg    argument1
 *             The first integer argument to be printed
 *  @arg    argument2
 *             The second integer argument to be printed
 *  @arg    argument3
 *             The third integer argument to be printed
 *
 *  @ret    None
 *
 *  @enter  The character string is valid
 *
 *  @leave  None
 *
 *  @see    TRC_0Print, TRC_1Print, TRC_2Print, TRC_4Print, TRC_5Print,
 *          TRC_6Print
 *  ============================================================================
 */

void
TRC_3print (IN  unsigned long int   componentMap,
            IN  unsigned short int   severity,
            IN  char *  debugString,
            IN  unsigned long int   argument1,
            IN  unsigned long int   argument2,
            IN  unsigned long int   argument3) ;


/** ============================================================================
 *  @func   TRC_4print
 *
 *  @desc   Prints a null terminated character string and four integer
 *          arguments based on its severity, the subcomponent and component it
 *          is associated  with.
 *
 *  @arg    componentMap
 *             The component & subcomponent to which this print belongs
 *  @arg    severity
 *             The severity associated with the print
 *  @arg    debugString
 *             The null terminated character string to be printed
 *  @arg    argument1
 *             The first integer argument to be printed
 *  @arg    argument2
 *             The second integer argument to be printed
 *  @arg    argument3
 *             The third integer argument to be printed
 *  @arg    argument4
 *             The fourth integer argument to be printed
 *
 *  @ret    None
 *
 *  @enter  The character string is valid
 *
 *  @leave  None
 *
 *  @see    TRC_0Print, TRC_1Print, TRC_2Print, TRC_3Print, TRC_5Print,
 *          TRC_6Print
 *  ============================================================================
 */

void
TRC_4print (IN  unsigned long int   componentMap,
            IN  unsigned short int   severity,
            IN  char *  debugString,
            IN  unsigned long int   argument1,
            IN  unsigned long int   argument2,
            IN  unsigned long int   argument3,
            IN  unsigned long int   argument4) ;


/** ============================================================================
 *  @func   TRC_5print
 *
 *  @desc   Prints a null terminated character string and five integer
 *          arguments based on its severity, the subcomponent and component it
 *          is associated  with.
 *
 *  @arg    componentMap
 *             The component & subcomponent to which this print belongs
 *  @arg    severity
 *             The severity associated with the print
 *  @arg    debugString
 *             The null terminated character string to be printed
 *  @arg    argument1
 *             The first integer argument to be printed
 *  @arg    argument2
 *             The second integer argument to be printed
 *  @arg    argument3
 *             The third integer argument to be printed
 *  @arg    argument4
 *             The fourth integer argument to be printed
 *  @arg    argument5
 *             The fifth integer argument to be printed
 *
 *  @ret    None
 *
 *  @enter  The character string is valid
 *
 *  @leave  None
 *
 *  @see    TRC_0Print, TRC_1Print, TRC_2Print, TRC_3Print, TRC_4Print,
 *          TRC_6Print
 *  ============================================================================
 */

void
TRC_5print (IN  unsigned long int   componentMap,
            IN  unsigned short int   severity,
            IN  char *  debugString,
            IN  unsigned long int   argument1,
            IN  unsigned long int   argument2,
            IN  unsigned long int   argument3,
            IN  unsigned long int   argument4,
            IN  unsigned long int   argument5) ;


/** ============================================================================
 *  @func   TRC_6print
 *
 *  @desc   Prints a null terminated character string and six integer
 *          arguments based on its severity, the subcomponent and component it
 *          is associated  with.
 *
 *  @arg    componentMap
 *             The component & subcomponent to which this print belongs
 *  @arg    severity
 *             The severity associated with the print
 *  @arg    debugString
 *             The null terminated character string to be printed
 *  @arg    argument1
 *             The first integer argument to be printed
 *  @arg    argument2
 *             The second integer argument to be printed
 *  @arg    argument3
 *             The third integer argument to be printed
 *  @arg    argument4
 *             The fourth integer argument to be printed
 *  @arg    argument5
 *             The fifth integer argument to be printed
 *  @arg    argument6
 *             The sixth integer argument to be printed
 *
 *  @ret    None
 *
 *  @enter  The character string is valid
 *
 *  @leave  None
 *
 *  @see    TRC_0Print, TRC_1Print, TRC_2Print, TRC_3Print, TRC_4Print,
 *          TRC_5Print.
 *  ============================================================================
 */

void
TRC_6print (IN  unsigned long int   componentMap,
            IN  unsigned short int   severity,
            IN  char *  debugString,
            IN  unsigned long int   argument1,
            IN  unsigned long int   argument2,
            IN  unsigned long int   argument3,
            IN  unsigned long int   argument4,
            IN  unsigned long int   argument5,
            IN  unsigned long int   argument6) ;

#else /* defined (TRACE_KERNEL) */

#define TRC_ENABLE(map)
#define TRC_DISABLE(map)
#define TRC_SET_SEVERITY(level)

#define TRC_0PRINT(a,b)                 \
    PRINT_Printf (b)

#define TRC_1PRINT(a,b,c)               \
    PRINT_Printf ((b), (int)(c))

#define TRC_2PRINT(a,b,c,d)             \
    PRINT_Printf ((b), (int)(c),        \
                       (int)(d))

#define TRC_3PRINT(a,b,c,d,e)           \
    PRINT_Printf ((b),(int)(c),         \
                      (int)(d),         \
                       (int)(e))

#define TRC_4PRINT(a,b,c,d,e,f)         \
    PRINT_Printf ((b), (int) (c),       \
                       (int) (d),       \
                       (int) (e),       \
                       (int) (f))

#define TRC_5PRINT(a,b,c,d,e,f,g)       \
    PRINT_Printf ((b), (int) (c),       \
                       (int) (d),       \
                       (int) (e),       \
                       (int) (f),       \
                       (int) (g))

#define TRC_6PRINT(a,b,c,d,e,f,g,h)     \
    PRINT_Printf ((b), (int) (c),       \
                       (int) (d),       \
                       (int) (e),       \
                       (int) (f),       \
                       (int) (g),       \
                       (int) (h))
#endif /* defined(TRACE_KERNEL) */

#define TRC_0ENTER(str)                     \
    TRC_0PRINT (TRC_ENTER,                  \
                "Entered " str " ()\n")

#define TRC_1ENTER(str,a)                   \
    TRC_1PRINT (TRC_ENTER,                  \
                "Entered " str " ()\n"    \
                "\t"#a"\t[0x%x]\n",         \
                a)

#define TRC_2ENTER(str,a,b)                 \
    TRC_2PRINT (TRC_ENTER,                  \
                "Entered " str " ()\n"    \
                "\t"#a"\t[0x%x]\n"          \
                "\t"#b"\t[0x%x]\n",         \
                a,b)

#define TRC_3ENTER(str,a,b,c)               \
    TRC_3PRINT (TRC_ENTER,                  \
                "Entered " str " ()\n"    \
                "\t"#a"\t[0x%x]\n"          \
                "\t"#b"\t[0x%x]\n"          \
                "\t"#c"\t[0x%x]\n",         \
                a,b,c)

#define TRC_4ENTER(str,a,b,c,d)             \
    TRC_4PRINT (TRC_ENTER,                  \
                "Entered " str " ()\n"    \
                "\t"#a"\t[0x%x]\n"          \
                "\t"#b"\t[0x%x]\n"          \
                "\t"#c"\t[0x%x]\n"          \
                "\t"#d"\t[0x%x]\n",         \
                a,b,c,d)

#define TRC_5ENTER(str,a,b,c,d,e)           \
    TRC_5PRINT (TRC_ENTER,                  \
                "Entered " str " ()\n"    \
                "\t"#a"\t[0x%x]\n"          \
                "\t"#b"\t[0x%x]\n"          \
                "\t"#c"\t[0x%x]\n"          \
                "\t"#d"\t[0x%x]\n"          \
                "\t"#e"\t[0x%x]\n",         \
                a,b,c,d,e)

#define TRC_6ENTER(str,a,b,c,d,e,f)         \
    TRC_6PRINT (TRC_ENTER,                  \
                "Entered " str " ()\n"    \
                "\t"#a"\t[0x%x]\n"          \
                "\t"#b"\t[0x%x]\n"          \
                "\t"#c"\t[0x%x]\n"          \
                "\t"#d"\t[0x%x]\n"          \
                "\t"#e"\t[0x%x]\n"          \
                "\t"#f"\t[0x%x]\n",         \
                a,b,c,d,e,f)

#define TRC_0LEAVE(str)                     \
    TRC_0PRINT (TRC_LEAVE,                  \
                "Leaving " str " ()\n")

#define TRC_1LEAVE(str,status)                                  \
    TRC_1PRINT (TRC_LEAVE,                                      \
                "Leaving " str " () \t"#status" [0x%x]\n",    \
                status)


#else  /* defined (TRACE_ENABLE) */

#define TRC_ENABLE(map)
#define TRC_DISABLE(map)
#define TRC_SET_SEVERITY(level)

#define TRC_0PRINT(a,b)
#define TRC_1PRINT(a,b,c)
#define TRC_2PRINT(a,b,c,d)
#define TRC_3PRINT(a,b,c,d,e)
#define TRC_4PRINT(a,b,c,d,e,f)
#define TRC_5PRINT(a,b,c,d,e,f,g)
#define TRC_6PRINT(a,b,c,d,e,f,g,h)

#define TRC_0ENTER(str)
#define TRC_1ENTER(str,a)
#define TRC_2ENTER(str,a,b)
#define TRC_3ENTER(str,a,b,c)
#define TRC_4ENTER(str,a,b,c,d)
#define TRC_5ENTER(str,a,b,c,d,e)
#define TRC_6ENTER(str,a,b,c,d,e,f)

#define TRC_0LEAVE(str)
#define TRC_1LEAVE(str,status)


#endif  /* defined (TRACE_ENABLE) */

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

#endif /* !defined (TRC_H) */
