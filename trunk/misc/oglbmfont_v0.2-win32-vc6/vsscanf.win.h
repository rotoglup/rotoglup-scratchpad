/* **************************************************************************
 * @version $Id: cppcompat.h,v 0.0 2006/04/01 01:41:00 legolas558 Exp $
 *
 * File:		vsscanf.win.h
 * Content:		include file for the vsscanf VC6 implementation
 * Notes:		shared between my various projects
 *
 *
 * **************************************************************************/

#ifndef	__VSSCANF_WIN_H
#define	__VSSCANF_WIN_H

#ifndef		vsscanf

#pragma comment( lib, "vsscanf.lib" )

#include <stdarg.h>

int __cdecl vsscanf (
    const char* str,
    const unsigned char *format,
    va_list arglist
    );

#endif

#endif
