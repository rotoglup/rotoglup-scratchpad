/* **************************************************************************
 * @version $Id: oglfont.c,v 0.1 2006/04/03 10:33:00 legolas558 Exp $
 *
 * File:		oglfont.c
 * Content:		common OpenGL font functions
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

#include "oglfont.h"

void	ogl__font_out() {
	glPopAttrib();		
}

void	ogl___font_in(ogl_font *font) {
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGetFloatv(GL_MODELVIEW_MATRIX, font->mmat);
	glMatrixMode(GL_MODELVIEW);
}

void L_INLINE draw_bmchar(const ogl_font *font, const BMchar *bmchar, float *cursorX)
{
  float currentX = (*cursorX);
  float scale = font->scale;
  float tx0, tx1, ty0, ty1;
  int vx0, vx1, vy0, vy1;

  if (bmchar == NULL || !bmchar->w) {
    // if char has no width, treat it like a space
    (*cursorX) += (float)font->base;
    return;
  }

  // font->base is the y base of the font bottom

  tx0 = (float) bmchar->x / (float) font->w;
  tx1 = (float) (bmchar->x + bmchar->w) / (float) font->w;
  ty0 = (float) ( (font->h - (bmchar->y+bmchar->h)) / (float) font->h );      // bottom
  ty1 = (float) ( (font->h - (bmchar->y)) / (float) font->h );

  vx0 = currentX + (scale * bmchar->x_ofs);
  vx1 = vx0 + (scale * bmchar->w);
  vy1 = font->base - (scale * bmchar->y_ofs);
  vy0 = vy1 - (scale * bmchar->h);

  glBegin(GL_QUADS);
  glTexCoord2f(	tx0, ty1 );
  glVertex2i(	vx0, vy1 );

  glTexCoord2f(	tx1, ty1 );
  glVertex2i(	vx1, vy1 );

  glTexCoord2f(	tx1, ty0 );
  glVertex2i(	vx1, vy0 );

  glTexCoord2f(	tx0, ty0 );
  glVertex2i( vx0, vy0 );
  glEnd();

  *cursorX += scale * (float)(bmchar->x_advance);
}

void L_INLINE ogl__print_l(ogl_font *font, float x, float y, const char *text, int l)
{
  int i;
  char prev_char;
  float cursorX = 0.f;

  glTranslatef(x,y,0);
  
  for (i=0; i<l; i++)
  {
    BMchar *glyph = find_glyph(font, text[i]);
    int index = glyph - font->chars;

    if (i>0)
    {
      BMkerninfo* kerning = find_kerning(font, prev_char, text[i]);
      if (kerning)
      {
        cursorX += font->scale * kerning->kerning;
      }
    }

    draw_bmchar(font, glyph, &cursorX);

    prev_char = text[i];
  }
	glLoadMatrixf(font->mmat);
}

// 2d OpenGL printf implementation
GLvoid ogl_printf(ogl_font *font, GLint x, GLint y, const char *fmt, ...) {
	char		text[BMF_MAX_LINE], *line_start,*c;
	float		h;
	va_list		ap;				
	int			row;

	if (!strlen(fmt)) return;

	va_start(ap, fmt);			
	    vsprintf(text, fmt, ap);
	va_end(ap);

	line_start = text;
	row = 0;
	h = font->h/.63f;
	ogl__font_in(font);
NextL:
	c = strchr(line_start, '\n');
	if (c) {
		row++;
		ogl__print_l(font, (float)x, (float)(y-h*row), line_start, c-line_start);
		line_start = c+1;
		goto NextL;
	}

	ogl__print_l(font, (float)x, (float)(y-h*row), line_start, strlen(line_start));
	ogl__font_out();
}

void ogl_print(ogl_font *font, float x, float y, const char *text)  {
	ogl__font_in(font);
	ogl__print_l(font, x, y, text, strlen(text));
	ogl__font_out();
}

#define OGLF_CENTER_TEXT(rect, txtw, ofs)	((float)((rect[ofs+_W] - txtw - rect[ofs])/2 + rect[ofs]))

// printf function which takes in account the alignment
void	ogl_printf_al(bm_font *font, alignment	align, GLuint *rect, const char *fmt, ...) {
	char		text[BMF_MAX_LINE];
	va_list		ap;

	if (!strlen(fmt)) return;

	va_start(ap, fmt);
	    vsprintf(text, fmt, ap);
	va_end(ap);

	switch (align) {
		case al_left:
			ogl_print(font, (float)rect[_X], OGLF_CENTER_TEXT(rect, font->line_h, _Y), text);
			break;
		case al_right:
			ogl_print(font, ((float)rect[_W]-ogl__width(font, text)), OGLF_CENTER_TEXT(rect, font->line_h, _Y), text);
			break;
		case al_center:
			ogl_print(font, OGLF_CENTER_TEXT(rect, ogl__width(font, text), _X), OGLF_CENTER_TEXT(rect, font->line_h, _Y), text);
			break;
	}
}
