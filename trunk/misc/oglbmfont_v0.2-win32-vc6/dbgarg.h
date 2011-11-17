/* **************************************************************************
 * @version $Id: dbgarg.h,v 0.0 2006/03/31 02:26:00 legolas558 Exp $
 *
 * File:		dbgarg.h
 * Content:		include file to handle debug parameters
 * Notes:		shared between my various projects
 *
 * Copyright(c) 2006 by legolas558
 *
 * This software is released under a BSD license. See LICENSE.TXT
 * You must accept the license before using this software.
 *
 * **************************************************************************/

#ifndef	__DBGARG_H
#define	__DBGARG_H

#define	DBG_GET		,__FILE__,__LINE__
#define	DBG_OUT		,dbg_fn,dbg_line
#define	DBG_IN		,char *dbg_fn, int dbg_line
#define	dbg_zmalloc	raw_zmalloc

#endif
