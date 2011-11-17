/* **************************************************************************
 * @version $Id: oglfont.h,v 0.1 2006/04/03 10:33:00 legolas558 Exp $
 *
 * File:		oglfont.h
 * Content:		OpenGL font functions header
 * Notes:		-
 *
 * Copyright(c) 2006 by legolas558
 *
 * https://sourceforge.net/projects/oglbmfont
 *
 * This software is released under a BSD license. See LICENSE.TXT
 * You must accept the license before using this software.
 *
 * **************************************************************************/

#ifndef	__OGLF_H
#define	__OGLF_H

#define	ogl_font			bm_font
#define	ogl_font_destroy	bmf_destroy
#define	ogl_font_load		bmf_load
#define	ogl__width			bmf__width

#define	ogl__font_in		bmf___font_in

void ogl_print(ogl_font *font, float x, float y, const char *text);
void ogl_printf_al(bm_font *font, alignment	align, GLuint *rect, const char *fmt, ...);
void ogl_printf(ogl_font *font, GLint x, GLint y, const char *fmt, ...);

#endif // __OGLF_H
