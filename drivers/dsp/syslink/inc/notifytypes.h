/*
 * notifytypes.h
 *
 * Notify driver support for OMAP Processors.
 *
 * Copyright (C) 2008-2009 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


#if !defined  _NOTIFYTYPES_H_
#define _NOTIFYTYPES_H_

#include <host_os.h>
#ifndef IN
#define IN                              /* The argument is INPUT  only */
#endif

#ifndef OUT
#define OUT                             /* The argument is OUTPUT only */
#endif

#ifndef OPT
#define OPT                             /* The argument is OPTIONAL    */
#endif

#ifndef CONST
#define CONST   const
#endif

#endif  /* _NOTIFYTYPES_H_ */
