
/*
 * memdefs.h
 *
 * Notify driver support functions for TI OMAP processors.
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


#ifndef MEMDEFS_
#define MEMDEFS_

/* Memory Pool Attributes: */
	enum mem_pool_attrs {
		MEM_PAGED = 0,
		MEM_NONPAGED = 1,
		MEM_LARGEVIRTMEM = 2
	} ;

/*
 *  MEM_VIRTUALSEGID is used by Node & Strm to access virtual address space in
 *  the correct client process context.
 */
#define MEM_SETVIRTUALSEGID     0x10000000
#define MEM_GETVIRTUALSEGID     0x20000000
#define MEM_MASKVIRTUALSEGID    (MEM_SETVIRTUALSEGID | MEM_GETVIRTUALSEGID)

#define TO_VIRTUAL_UNCACHED(x) x
#define INTREG_TO_VIRTUAL_UNCACHED(x) x

#endif				/* MEMDEFS_ */

