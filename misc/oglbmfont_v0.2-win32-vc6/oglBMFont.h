/* **************************************************************************
* @version $Id: oglBMFont.h,v 0.2 2006/04/03 12:03:00 legolas558 Exp $
*
* File:		oglBMFont.h
* Content:		main definition header for the bm_font object and related
*				methods/members
* Notes:		uses RWops object for I/O
*
* Copyright(c) 2006 by legolas558
*
* https://sourceforge.net/projects/oglbmfont
*
* This software is released under a BSD license. See LICENSE.TXT
* You must accept the license before using this software.
*
* **************************************************************************/

#ifndef	__OGLBMF_H
#define	__OGLBMF_H

#include <windows.h>
#include <gl/gl.h>

#include "cppcompat.h"
#include "zmem.h"

#define	BMF_MAX_LINE	1024
#define	BMF_MAX_PATH	1024

typedef struct S_BMchar {
  GLuint id;
  GLuint x,y, w,h;

  GLint x_ofs,y_ofs;
  GLuint x_advance;
  GLuint page;		// unused
};
#define	BMchar	struct S_BMchar

typedef struct S_BMkerninfo {
  GLuint first, second;
  GLint kerning;
};
#define	BMkerninfo	struct S_BMkerninfo


typedef struct S_bm_font {
  float		scale;
  GLuint			base, line_h, w, h, pages,
    kerninfo_count, chars_count;
  GLuint			fontTex;
  BMkerninfo		*kerninfo;
  BMchar			*chars;
  float			mmat[16];
};
#define	bm_font	struct S_bm_font

#ifndef	_X
enum E_alignment {
  al_left = 0,
  al_center,
  al_right
};
#define	alignment	int

#ifndef	_X
#define	_X	0
#define	_Y	(_X+1)
#define	_W	(_Y+1)
#define	_H	(_W+1)
#define	DEF_RECT	(_H+1)
#endif

#endif


bool	bmf_load(bm_font *font, char *bmf_path);
int		bmf_width(bm_font *font, char *text);
void	bmf_destroy(bm_font *font);

#include "oglfont.h"

#endif	//__OGLBMF_H
