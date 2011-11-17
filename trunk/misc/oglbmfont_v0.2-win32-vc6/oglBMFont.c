/* **************************************************************************
* @version $Id: oglBMFont.c,v 0.2 2006/04/03 12:03:00 legolas558 Exp $
*
* File:		oglBMFont.c
* Content:		main source code for the bm_font object and related
*				methods/members
* Notes:		uses GLFW framework compiled into a static library
*				uses RWops object for I/O
*
* Copyright(c) 2006 by legolas558
*
* https://sourceforge.net/projects/oglbmfont
*
* This software is released under a BSD license. See LICENSE.TXT
* You must accept the license before using this software.
*
* **************************************************************************/

#include "oglBMfont.h"
#include <assert.h>
#include <stdio.h>

#include "glfw.h"

// main function used to load a BMF font
// note: must be called within a valid OpenGL context because it allocates
// a texture and creates display lists for the font
bool	bmf_load(bm_font *font, char *bmf_path) {
  FILE* bmf = NULL;
  char linebuf[4096];
  char *readresult;
  int		i;
  long	p;
  char	tga_path[BMF_MAX_PATH];
  BMchar	temp_char;
  BMkerninfo temp_kerning;

  bmf = fopen(bmf_path, "r");
  if (!bmf) return false;

  fgets(linebuf, 4096, bmf);  // skip the first info line

  // read header

  readresult = fgets(linebuf, 4096, bmf);
  if (sscanf(linebuf, "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d\n",
    &font->line_h, &font->base, &font->w, &font->h, &font->pages) != 5) {
      fclose(bmf);
      return false;
  }
  if (fscanf(bmf, "page id=0 file=%s\n",
    tga_path) != 1) {
      fclose(bmf);
      return false;
  }
  if (fscanf(bmf, "chars count=%d\n",
    &font->chars_count) != 1) {
      fclose(bmf);
      return false;
  }

  // allocate (and init to 0) the chars
  font->chars = (BMchar *) zcalloc(font->chars_count, sizeof(BMchar));
  i = 0;
NextChar:
  // read a character from the input file
  // note that the ordinal 'i' is also read and in a second pass temp_char is
  // swapped into the main array of chars
  readresult = fgets(linebuf, 4096, bmf);
  if (readresult && sscanf(linebuf, "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d\n",
    &temp_char.id,
    &temp_char.x,
    &temp_char.y,
    &temp_char.w,
    &temp_char.h,
    &temp_char.x_ofs,
    &temp_char.y_ofs,
    &temp_char.x_advance,
    &temp_char.page) == 9)
  {
    //assert( i==0 || font->chars[i-1].id < temp_char.id );       // assume sorted order for binary search
    font->chars[i] = temp_char;
    i++;
    goto NextChar;
  }

  if (sscanf(linebuf, "kernings count=%d\n",
    &font->kerninfo_count) != 1) {
      fclose(bmf);
      goto Fail;
  }

  // allocate (and init to 0) the kernings
  font->kerninfo = (BMkerninfo *) zcalloc(font->kerninfo_count, sizeof(BMkerninfo));
  i = 0;
NextKerning:
  // read a character from the input file
  // note that the ordinal 'i' is also read and in a second pass temp_char is
  // swapped into the main array of chars
  readresult = fgets(linebuf, 4096, bmf);
  if (readresult && sscanf(linebuf, "kerning first=%d second=%d amount=%d\n",
    &temp_kerning.first,
    &temp_kerning.second,
    &temp_kerning.kerning
    ) == 3)
  {
    font->kerninfo[i] = temp_kerning;
    i++;
    goto NextKerning;
  }

  fclose(bmf);										// close the source file

  /* OLD
  p = (long)strrchr(bmf_path, '.');
  if (!p) goto Fail;
  p-=(long)bmf_path;
  memcpy(tga_path, bmf_path, p+1);
  strcpy(tga_path+p+1, "tga");
  */

  // TODO compose the tga path relative to the given bmf_path
  // remove quotes
  memmove(tga_path, tga_path+1, strlen(tga_path)-1+1);
  tga_path[strlen(tga_path)-1] = 0;

  glGenTextures(1, &font->fontTex);					// create the main font texture
  glBindTexture(GL_TEXTURE_2D, font->fontTex);		// and set as current
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  if (!glfwLoadTexture2D(tga_path, GLFW_BUILD_MIPMAPS_BIT)) {				// try to load the font TGA file
    glDeleteTextures(1, &font->fontTex);
    zfree(font->kerninfo);
Fail:
    zfree(font->chars);
    return false;
  }

  font->scale = 1.f;
  return true;										// into an OpenGL display list
}

BMchar *find_glyph(bm_font *font, char c)
{
  int first=0, last=font->chars_count-1;
  int mid;
  unsigned glyphid;

  while (first < last)
  { 
    mid = (first + last) / 2;
    glyphid = font->chars[mid].id;
    if (glyphid == c) return &font->chars[mid];
    if (glyphid < c) { first = mid+1; }
    else { last = mid-1; }
  }
  glyphid = font->chars[first].id;
  if (glyphid == c) return &font->chars[first];

  return NULL;
}

BMkerninfo *find_kerning(bm_font *font, char a, char b)
{
  // binary search a

  const int count = font->kerninfo_count;
  int first=0, last=count-1;
  int mid;
  unsigned charid;

  while (first < last)
  { 
    mid = (first + last) / 2;
    charid = font->kerninfo[mid].first;
    if (charid == a) { first = last = mid; break; }
    if (charid < a) { first = mid+1; }
    else { last = mid-1; }
  }

  // brute force search b

  for (; first<count; first++)
  {
    if (font->kerninfo[first].first != a) return NULL;
    charid = font->kerninfo[first].second;
    if (charid == b) return &font->kerninfo[first];
  }

  return NULL;
}

// calculate the approx. width of a string of text
// note: no kerning info is currently evaluated
int		bmf__width(bm_font *font, char *text) {
  int	w, l, i;
  float scale = font->scale;
  w = 0;
  l = strlen(text);
  for (i=0;i < l; i++) {
    w += scale * find_glyph(font, text[i])->w;
  }
  return w+l;
}

// destroy the charset
void	bmf_destroy(bm_font *font) {
  glDeleteTextures(1, &font->fontTex);
  zfree(font->chars);
  zfree(font->kerninfo);
}

void	ogl___font_in(ogl_font *font);

void	bmf___font_in(ogl_font *font) {
  ogl___font_in(font);
  glBindTexture(GL_TEXTURE_2D, font->fontTex);
}

#include "oglfont.c"

