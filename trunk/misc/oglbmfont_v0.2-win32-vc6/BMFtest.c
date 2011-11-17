/* **************************************************************************
 * @version $Id: BMFtest.c,v 0.2 2006/04/03 10:33:00 legolas558 Exp $
 *
 * File:		BMFtest.c
 * Content:		main program source for testing my OpenGL BMfont
 *				implementation
 * Notes:		uses GLFW framework compiled into a static library
 *
 * Copyright(c) 2006 by legolas558
 *
 * https://sourceforge.net/projects/oglbmfont
 *
 * This software is released under a BSD license. See LICENSE.TXT
 * You must accept the license before using this software.
 *
 * **************************************************************************/

#include "BMFtest.h"

#define	SCREEN_W	800			//screen width
#define	SCREEN_H	600			//screen height

void	ogl_setup_2d() {
	// set 2d mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// invert y axis since OpenGL uses it updown
	glOrtho(0, SCREEN_W, 0, SCREEN_H, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int WINAPI WinMain(HINSTANCE h1, HINSTANCE h2, LPSTR s, int n) {
	ogl_font	myfont;								// the font object we will use
	GLuint		screen_rect[4];						// an array containing the screen
													// bounding rectangle

	zmem_init();									// dummy heap debugger, ignore it

	if (!glfwInit())								// initializes GLFW framework
		return -1;
	atexit(glfwTerminate);							// setup the cleaner hook

    if( !glfwOpenWindow(SCREEN_W, SCREEN_H, 8, 8, 8, 8, 0, 0, GLFW_WINDOW ) )
		return -2;									// open a rendering context

    glViewport(0, 0, SCREEN_W, SCREEN_H);

    //glClearColor( 0.5f, 0.5f, 0.5f, 0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear screen & depth buffer

#define FONTFILE "helvetica.fnt"
//#define FONTFILE "DIN.fnt"

	if (!ogl_font_load(&myfont, FONTFILE))			// loads a BMF font
		return -3;

	screen_rect[_X] = screen_rect[_Y] = 0;					// sets up a screen rectangle
	screen_rect[_W] = SCREEN_W; screen_rect[_H] = SCREEN_H;

	ogl_setup_2d();											// (see above) sets up the (x,y)

  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glAlphaFunc( GL_GREATER, 0 );							// set the alpha transparency
  //glEnable( GL_ALPHA_TEST );								// and turn it on
  glDisable(GL_BLEND);

  myfont.scale = 0.5f;
	ogl_printf(&myfont, 0, 40, "The quick brown fox jumps over the lazy dog (%dx%d)", SCREEN_W, SCREEN_H);
	ogl_printf_al(&myfont, al_center, (GLuint *)&screen_rect, "The quick brown fox jumps over the lazy dog");
	ogl_printf(&myfont, 0, 0, "ÅaThe quick brown fox jumps over the lazy dog");
	glfwSwapBuffers();										// flushes primitives to screen
	
	glfwSleep(17.7f);											// wait some time

	ogl_font_destroy(&myfont);								// free the object

	return 0;
}
