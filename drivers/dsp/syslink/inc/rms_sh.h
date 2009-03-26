
/*
 * rms_sh.h
 *
 * DSP-BIOS Bridge driver support functions for TI OMAP processors.
 *
 * Copyright (C) 2008 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


/*
 *  ======== rms_sh.h ========
 *
 *  DSP/BIOS Bridge Resource Manager Server shared definitions (used on both
 *  GPP and DSP sides).
 *
 *! Revision History
 *! ================
 *! 24-Mar-2003 vp  Merged updates required for CCS2.2 transition.
 *! 24-Feb-2003 kc  Rearranged order of node types to temporarily support
 *!		    legacy message node code
 *! 23-Nov-2002 gp  Converted tabs -> spaces, to fix formatting.
 *! 13-Feb-2002 jeh Added sysstacksize element to RMS_MoreTaskArgs.
 *! 11-Dec-2000 sg  Added 'misc' element to RMS_MoreTaskArgs.
 *! 04-Dec-2000 ag  Added RMS_BUFDESC command code.
 *!                 C/R code value changed to allow ORing of system/user codes.
 *! 10-Oct-2000 sg  Added 'align' field to RMS_StrmDef.
 *! 09-Oct-2000 sg  Moved pre-defined message codes here from rmsdefs.h.
 *! 02-Oct-2000 sg  Changed ticks to msec.
 *! 24-Aug-2000 sg  Moved definitions that will be exposed to app developers
 *!  		    to a separate file, rmsdefs.h.
 *! 10-Aug-2000 sg  Added RMS_COMMANDBUFSIZE and RMS_RESPONSEBUFSIZE; added
 *!		    pre-defined command/response codes; more comments.
 *! 09-Aug-2000 sg  Added RMS_ETASK.
 *! 08-Aug-2000 jeh Define u32 for GPP, rename DSP_MSG to RMS_DSPMSG.
 *!                 Added RMS_MsgArgs, RMS_MoreTaskArgs.
 *! 25-Jul-2000 sg: Changed SIO to STRM.
 *! 30-Jun-2000 sg: Initial.
 */

#ifndef RMS_SH_
#define RMS_SH_

#include <rmstypes.h>

/* Node Types: */
#define RMS_TASK                1	/* Task node */
#define RMS_DAIS                2	/* xDAIS socket node */
#define RMS_MSG                 3	/* Message node */

/* Memory Types: */
#define RMS_CODE                0	/* Program space */
#define RMS_DATA                1	/* Data space */
#define RMS_IO                	2	/* I/O space */

/* RM Server Command and Response Buffer Sizes: */
#define RMS_COMMANDBUFSIZE     256	/* Size of command buffer */
#define RMS_RESPONSEBUFSIZE    16	/* Size of response buffer */

/* Pre-Defined Command/Response Codes: */
#define RMS_EXIT                0x80000000   /* GPP->Node: shutdown */
#define RMS_EXITACK             0x40000000   /* Node->GPP: ack shutdown */
#define RMS_BUFDESC             0x20000000   /* Arg1 SM buf, Arg2 is SM size */
#define RMS_KILLTASK            0x10000000  /* GPP->Node: Kill Task */
#define RMS_USER                0x0	/* Start of user-defined msg codes */
#define RMS_MAXUSERCODES        0xfff	/* Maximum user defined C/R Codes */


/* RM Server RPC Command Structure: */
	struct RMS_Command {
		u32 fxn;	/* Server function address */
		u32 arg1;	/* First argument */
		u32 arg2;	/* Second argument */
		u32 data;	/* Function-specific data array */
	} ;

/*
 *  The RMS_StrmDef structure defines the parameters for both input and output
 *  streams, and is passed to a node's create function.
 */
	struct RMS_StrmDef {
		u32 bufsize;	/* Buffer size (in DSP words) */
		u32 nbufs;	/* Max number of bufs in stream */
		u32 segid;	/* Segment to allocate buffers */
		u32 align;	/* Alignment for allocated buffers */
		u32 timeout;	/* Timeout (msec) for blocking calls */
		char name[1];	/* Device Name (terminated by '\0') */
	} ;

/* Message node create args structure: */
	struct RMS_MsgArgs {
		u32 maxMessages;	/* Max # simultaneous msgs to node */
		u32 segid;	/* Mem segment for NODE_allocMsgBuf */
		u32 notifyType;	/* Type of message notification */
		u32 argLength;	/* Length (in DSP chars) of arg data */
		u32 argData;	/* Arg data for node */
	} ;

/* Partial task create args structure */
	struct RMS_MoreTaskArgs {
		u32 priority;	/* Task's runtime priority level */
		u32 stackSize;	/* Task's stack size */
		u32 sysstackSize;	/* Task's system stack size (55x) */
		u32 stackSeg;	/* Memory segment for task's stack */
		u32 heapAddr;   /* base address of the node memory heap in
				      * external memory (DSP virtual address) */
		u32 heapSize;   /* size in MAUs of the node memory heap in
				      * external memory */
		u32 misc;	/* Misc field.  Not used for 'normal'
				 * task nodes; for xDAIS socket nodes
				 * specifies the IALG_Fxn pointer.
				 */
		/* # input STRM definition structures */
		u32 numInputStreams;
	} ;

#endif				/* RMS_SH_ */

