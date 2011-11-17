/* **************************************************************************
 * @version $Id: cppcompat.h,v 0.0 2006/03/31 02:26:00 legolas558 Exp $
 *
 * File:		cppcompat.h
 * Content:		include file that simulates some C++ definitions for C
 * Notes:		shared between my various projects
 *
 * Copyright(c) 2006 by legolas558
 *
 * This software is released under a BSD license. See LICENSE.TXT
 * You must accept the license before using this software.
 *
 * **************************************************************************/

#ifndef	_CPPCOMPAT_H
#define _CPPCOMPAT_H

#ifndef bool
#define	bool	unsigned int
#define	true	1
#define	false	0
#endif

// C style inline
#ifdef	_inline
#define	L_INLINE	_inline
#else // C++ style inline
#ifdef	inline
#define	L_INLINE	inline
#else // inline not available
#define	L_INLINE
#endif
#endif


#endif
