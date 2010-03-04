//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Small Win32 sample application made to illustrate a problem encountered on
// Windows 7 with OpenGL in a window covering the whole screen.
//
// In this context, opening a modal dialog on Windows 7 leads the dialog not
// to be shown correctly. The application looks like being freezed while the
// dialog in unvisible. You have to 'Alt-Tab' the application to see the dialog.
//
// On Windows XP, the modal dialog behaves and is properly visible.
//
// PRESS SPACE TO PROVOKE THE DIALOG OPENING
//
// PRESS ESCAPE OR ALT-F4 TO QUIT
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>

#include <math.h>

#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")
#include <GL/GL.h>
#pragma comment (lib, "opengl32.lib")

HINSTANCE hInst;
TCHAR const szWindowClass[] = TEXT("FullscreenWindowClass");
HDC g_gl_hdc = NULL;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//----------------------------------------------------------------------------

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
  TCHAR const text[] = 
    TEXT("// Small Win32 sample application made to illustrate a problem encountered on\n")
    TEXT("// Windows 7 with OpenGL in a window covering the whole screen.\n")
    TEXT("//\n")
    TEXT("// In this context, opening a modal dialog on Windows 7 leads the dialog not\n")
    TEXT("// to be shown correctly. The application looks like being freezed while the\n")
    TEXT("// dialog in unvisible. You have to 'Alt-Tab' the application to see the dialog.\n")
    TEXT("//\n")
    TEXT("// On Windows XP, the modal dialog behaves and is properly visible.\n")
    TEXT("//\n")
    TEXT("// PRESS SPACE TO PROVOKE THE DIALOG OPENING\n")
    TEXT("//\n")
    TEXT("// PRESS ESCAPE OR ALT-F4 TO QUIT\n")
    ;

  if (MessageBox(NULL, text, TEXT("Continue ?"), MB_OKCANCEL) == IDCANCEL)
    return 0;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;

	MyRegisterClass(hInstance);
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// Main message loop:

  bool quit = false;

  while (!quit)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      do
      {
        if (msg.message == WM_QUIT) quit = true;
		    TranslateMessage(&msg);
		    DispatchMessage(&msg);
      } 
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));
    }

    DWORD time = ::timeGetTime();

    float color_r = 0.5f + 0.5f * cos( 1e-3f * time );
    float color_g = 0.5f + 0.5f * sin( 5e-4f * time );

    glClearColor(color_r, color_g,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    ::SwapBuffers(g_gl_hdc);
  }

	return (int) msg.wParam;
}

//----------------------------------------------------------------------------

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

//----------------------------------------------------------------------------

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  HWND hWnd;

  hInst = hInstance; // Store instance handle in our global variable

  int width  = GetSystemMetrics(SM_CXSCREEN);
  int height = GetSystemMetrics(SM_CYSCREEN);
  hWnd = CreateWindow(szWindowClass, TEXT("Fullscreen window"), WS_POPUP,
    0, 0, width, height, NULL, NULL, hInstance, NULL);

  if (!hWnd)
  {
    return FALSE;
  }

  // OpenGL

  PIXELFORMATDESCRIPTOR pfd;
   memset(&pfd, 0, sizeof(pfd));
   pfd.nSize = sizeof(pfd);
   pfd.nVersion = 1;
   pfd.dwFlags = 
    PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | 
		PFD_DOUBLEBUFFER |
		PFD_GENERIC_ACCELERATED |
    PFD_SWAP_EXCHANGE
    ;
   pfd.iPixelType = PFD_TYPE_RGBA;
   pfd.cColorBits = 32;
   pfd.cAlphaBits = 8;
   pfd.cDepthBits = 24;
   pfd.cStencilBits = 8;
   pfd.iLayerType = PFD_MAIN_PLANE;

  int  selected_pf;

  HDC hdc = ::GetDC(hWnd);
  if ( ( selected_pf = ChoosePixelFormat( hdc, &pfd ) ) == 0 )
  {
    return FALSE;
  }

  if ( !SetPixelFormat( hdc, selected_pf, &pfd) )
  {
    return FALSE;
  }

  HGLRC hglrc = wglCreateContext(hdc);
  if (hglrc == 0)
  {
    return FALSE;
  }

  if (!wglMakeCurrent(hdc, hglrc))
  {
    return FALSE;
  }

  g_gl_hdc = hdc;

  //

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

//----------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
  case WM_KEYUP:
    if (wParam == VK_SPACE)
			MessageBox(hWnd, TEXT("This dialog box should open in front of the main window.\n\nDoes it ? On my Win7, it does not."), TEXT("Hello"), MB_OK);
    if (wParam == VK_ESCAPE)
      DestroyWindow(hWnd);
    break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
