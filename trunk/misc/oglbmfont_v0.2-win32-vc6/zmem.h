/* **************************************************************************
 * @version $Id: zmem.h, v 0.1 2006/04/03 12:03:00 legolas558 Exp $
 *
 * File:		zmem.h
 * Content:		heap leakage debugger
 * Notes:		-
 *
 * Copyright(c) 2006 by legolas558
 *
 * https://sourceforge.net/projects/zmem
 *
 * This software is released under a BSD license. See LICENSE.TXT
 * You must accept the license before using this software.
 *
 * **************************************************************************/

#ifndef	__ZMEM_H
#define	__ZMEM_H

#ifdef	ZMEM

#include <stdlib.h>

#define	_DBG_FILE	__FILE__
#define	_DBG_LINE	__LINE__

#define ZMEM_LOG_FILE	"zmem.log"

#include "../dbgarg.h"

/*#define _QUOTE(x) # x
#define QUOTE(x)	_QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "*/

void *dbg_zmalloc(size_t length, char *filename, int line);
void *dbg_zcalloc(size_t length, size_t n, char *filename, int line);
void *dbg_zrealloc(void *old_pointer, size_t length, char *filename, int line);
void dbg_zfree(void *pointer, char *filename, int line);

#define	zmalloc(l)		dbg_zmalloc(l,_DBG_FILE, _DBG_LINE)
#define	zcalloc(l,n)	dbg_zcalloc(l,n,_DBG_FILE, _DBG_LINE)
#define	zfree(p)		dbg_zfree(p,_DBG_FILE,_DBG_LINE)
#define	zrealloc(p,l)	dbg_zrealloc(p,l,_DBG_FILE,_DBG_LINE)

void	_cdecl zmem_finish(void);
void	zmem_init();

#else

#define	zmalloc		malloc
#define zrealloc	realloc
#define	zfree		free
#define	zcalloc		calloc

#define	zmem_init()

#endif


void	DSA_Init();
void _cdecl DSA_Finish(void);


#endif
