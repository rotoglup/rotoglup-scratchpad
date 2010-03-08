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

#define USE_OPENGL 1

#define POPUP_MODAL_ON_TIMER 0

#define WORKAROUND_ATTEMPT 3

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>

#include <math.h>

#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

#if USE_OPENGL

#include <GL/GL.h>
#pragma comment (lib, "opengl32.lib")

HDC g_gl_hdc = NULL;

BOOL initOpenGL(HWND hWnd);

/* WGL_ARB_pixel_format */
#define GL_CONST(name, value) unsigned const name = value;
GL_CONST(WGL_NUMBER_PIXEL_FORMATS_ARB, 0x2000)
GL_CONST(WGL_DRAW_TO_WINDOW_ARB, 0x2001)
GL_CONST(WGL_DRAW_TO_BITMAP_ARB, 0x2002)
GL_CONST(WGL_ACCELERATION_ARB, 0x2003)
GL_CONST(WGL_NEED_PALETTE_ARB, 0x2004)
GL_CONST(WGL_NEED_SYSTEM_PALETTE_ARB, 0x2005)
GL_CONST(WGL_SWAP_LAYER_BUFFERS_ARB, 0x2006)
GL_CONST(WGL_SWAP_METHOD_ARB, 0x2007)
GL_CONST(WGL_NUMBER_OVERLAYS_ARB, 0x2008)
GL_CONST(WGL_NUMBER_UNDERLAYS_ARB, 0x2009)
GL_CONST(WGL_TRANSPARENT_ARB, 0x200A)
GL_CONST(WGL_TRANSPARENT_RED_VALUE_ARB, 0x2037)
GL_CONST(WGL_TRANSPARENT_GREEN_VALUE_ARB, 0x2038)
GL_CONST(WGL_TRANSPARENT_BLUE_VALUE_ARB, 0x2039)
GL_CONST(WGL_TRANSPARENT_ALPHA_VALUE_ARB, 0x203A)
GL_CONST(WGL_TRANSPARENT_INDEX_VALUE_ARB, 0x203B)
GL_CONST(WGL_SHARE_DEPTH_ARB, 0x200C)
GL_CONST(WGL_SHARE_STENCIL_ARB, 0x200D)
GL_CONST(WGL_SHARE_ACCUM_ARB, 0x200E)
GL_CONST(WGL_SUPPORT_GDI_ARB, 0x200F)
GL_CONST(WGL_SUPPORT_OPENGL_ARB, 0x2010)
GL_CONST(WGL_DOUBLE_BUFFER_ARB, 0x2011)
GL_CONST(WGL_STEREO_ARB, 0x2012)
GL_CONST(WGL_PIXEL_TYPE_ARB, 0x2013)
GL_CONST(WGL_COLOR_BITS_ARB, 0x2014)
GL_CONST(WGL_RED_BITS_ARB, 0x2015)
GL_CONST(WGL_RED_SHIFT_ARB, 0x2016)
GL_CONST(WGL_GREEN_BITS_ARB, 0x2017)
GL_CONST(WGL_GREEN_SHIFT_ARB, 0x2018)
GL_CONST(WGL_BLUE_BITS_ARB, 0x2019)
GL_CONST(WGL_BLUE_SHIFT_ARB, 0x201A)
GL_CONST(WGL_ALPHA_BITS_ARB, 0x201B)
GL_CONST(WGL_ALPHA_SHIFT_ARB, 0x201C)
GL_CONST(WGL_ACCUM_BITS_ARB, 0x201D)
GL_CONST(WGL_ACCUM_RED_BITS_ARB, 0x201E)
GL_CONST(WGL_ACCUM_GREEN_BITS_ARB, 0x201F)
GL_CONST(WGL_ACCUM_BLUE_BITS_ARB, 0x2020)
GL_CONST(WGL_ACCUM_ALPHA_BITS_ARB, 0x2021)
GL_CONST(WGL_DEPTH_BITS_ARB, 0x2022)
GL_CONST(WGL_STENCIL_BITS_ARB, 0x2023)
GL_CONST(WGL_AUX_BUFFERS_ARB, 0x2024)
GL_CONST(WGL_NO_ACCELERATION_ARB, 0x2025)
GL_CONST(WGL_GENERIC_ACCELERATION_ARB, 0x2026)
GL_CONST(WGL_FULL_ACCELERATION_ARB, 0x2027)
GL_CONST(WGL_SWAP_EXCHANGE_ARB, 0x2028)
GL_CONST(WGL_SWAP_COPY_ARB, 0x2029)
GL_CONST(WGL_SWAP_UNDEFINED_ARB, 0x202A)
GL_CONST(WGL_TYPE_RGBA_ARB, 0x202B)

#else

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")

LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

BOOL initDirectX(HWND hWnd);

#endif

HINSTANCE hInst;
TCHAR const szWindowClass[] = TEXT("FullscreenWindowClass");

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//----------------------------------------------------------------------------
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
#if USE_OPENGL
    TEXT("// * OPENGL MODE\n")
#else
    TEXT("// * DIRECTX MODE\n")
#endif
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

    float color_r = 0.5f + 0.5f * cos( 1e3f * time );
    float color_g = 0.5f + 0.5f * sin( 5e4f * time );

#if USE_OPENGL

    //------------------------------------------------------------------------

    glClearColor(color_r, color_g,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    ::SwapBuffers(g_gl_hdc);

#else

    //------------------------------------------------------------------------

    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(color_r, color_g, 0.0f, 1.0f), 1.0f, 0 );

    g_pd3dDevice->BeginScene();

    g_pd3dDevice->EndScene();

    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

#endif

    //------------------------------------------------------------------------
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

  //

#if USE_OPENGL

  if (!initOpenGL(hWnd))
    return FALSE;

#else

  if (!initDirectX(hWnd))
    return FALSE;

#endif

  //

#if POPUP_MODAL_ON_TIMER

  SetTimer(hWnd, 123, 5000, NULL);

#endif

  //

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

#if USE_OPENGL

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

BOOL initOpenGL_classic(HWND hWnd);
BOOL initOpenGL_with_WGL_ARB_pixel_format(HWND hWnd);

BOOL initOpenGL(HWND hWnd)
{
#if WORKAROUND_ATTEMPT==3
  return initOpenGL_with_WGL_ARB_pixel_format(hWnd);
#else
  return initOpenGL_classic(hWnd);
#endif
}

//----------------------------------------------------------------------------

BOOL initOpenGL_with_WGL_ARB_pixel_format(HWND hWnd)
{
  // quick and dirty code, no cleanup done on temporary window

  HWND hWndTemp = CreateWindow(TEXT("STATIC"), NULL, WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 0, 0, 0, 0, NULL, NULL, hInst, NULL);

  initOpenGL_classic(hWndTemp);

  //

  typedef PROC (__stdcall *TYPE_wglGetProcAddress)(LPCSTR);
  typedef BOOL (__stdcall *TYPE_wglChoosePixelFormatARB)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

  TYPE_wglGetProcAddress wglGetProcAddress = NULL;
  TYPE_wglChoosePixelFormatARB wglChoosePixelFormatARB = NULL;

  HMODULE library = LoadLibrary( TEXT("opengl32.dll") );
  if (library != 0)
  {
    wglGetProcAddress = (TYPE_wglGetProcAddress) GetProcAddress( library, "wglGetProcAddress" );

    if (wglGetProcAddress != NULL)
    {
      wglChoosePixelFormatARB = (TYPE_wglChoosePixelFormatARB) wglGetProcAddress( "wglChoosePixelFormatARB" );
    }

    FreeLibrary (library);
  }

  if (wglChoosePixelFormatARB == NULL)
  {
    return initOpenGL_classic(hWnd);
  }

  //

  int Attrib[128], i=0;
  Attrib[i++]=WGL_DRAW_TO_WINDOW_ARB;
  Attrib[i++]=true;
  Attrib[i++]=WGL_SUPPORT_OPENGL_ARB;
  Attrib[i++]=true;
  Attrib[i++]=WGL_DOUBLE_BUFFER_ARB;
  Attrib[i++]=true;
  Attrib[i++]=WGL_SWAP_METHOD_ARB;
  Attrib[i++]=WGL_SWAP_COPY_ARB;            //< this is what make the modal dialog work
  Attrib[i++]=WGL_PIXEL_TYPE_ARB;
  Attrib[i++]=WGL_TYPE_RGBA_ARB;
  Attrib[i++]=WGL_COLOR_BITS_ARB;
  Attrib[i++]=32;
  Attrib[i++]=WGL_ALPHA_BITS_ARB;
  Attrib[i++]=8;
  Attrib[i++]=WGL_DEPTH_BITS_ARB;
  Attrib[i++]=24;
  Attrib[i++]=WGL_STENCIL_BITS_ARB;
  Attrib[i++]=8;
  Attrib[i++]=0;

  int selected_pf = 0;
  HDC hdc = ::GetDC(hWnd);

  UINT NumFormats = 0;
  if (!wglChoosePixelFormatARB(hdc, Attrib, NULL, 1, &selected_pf, &NumFormats))
  {
    return FALSE;
  }
  if (NumFormats == 0)
  {
    return FALSE;
  }
  PIXELFORMATDESCRIPTOR pfd;
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
  
  return TRUE;
}

//----------------------------------------------------------------------------

BOOL initOpenGL_classic(HWND hWnd)
{
#if WORKAROUND_ATTEMPT == 1
  {
    HMODULE library = LoadLibrary( TEXT("DWMAPI.DLL") );
    if (library != 0)
    {
      typedef HRESULT (__stdcall *PFN)(UINT);
      PFN func = (PFN)GetProcAddress( library, "DwmEnableComposition" );
      if (func != 0)
      {
        func( /*DWM_EC_DISABLECOMPOSITION*/ 0 );
      }
      FreeLibrary (library);
    }
  }
#endif

  // OpenGL

  int  selected_pf;
  HDC hdc = ::GetDC(hWnd);

  PIXELFORMATDESCRIPTOR pfd;
   memset(&pfd, 0, sizeof(pfd));
   pfd.nSize = sizeof(pfd);
   pfd.nVersion = 1;
   pfd.dwFlags = 
    PFD_DRAW_TO_WINDOW |
	  PFD_SUPPORT_OPENGL | 
	  PFD_DOUBLEBUFFER |
	  PFD_GENERIC_ACCELERATED |
#if WORKAROUND_ATTEMPT == 2
    PFD_SWAP_COPY
#else
    PFD_SWAP_EXCHANGE
#endif
    ;
   pfd.iPixelType = PFD_TYPE_RGBA;
   pfd.cColorBits = 32;
   pfd.cAlphaBits = 8;
   pfd.cDepthBits = 24;
   pfd.cStencilBits = 8;
   pfd.iLayerType = PFD_MAIN_PLANE;

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
  
  return TRUE;
}

#else

//----------------------------------------------------------------------------

BOOL initDirectX(HWND hWnd)
{
  g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

  if( g_pD3D == NULL )
  {
    return FALSE;
  }

  D3DDISPLAYMODE d3ddm;

  if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
  {
    return FALSE;
  }

  HRESULT hr;

  if( FAILED( hr = g_pD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
    d3ddm.Format, D3DUSAGE_DEPTHSTENCIL,
    D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
  {
    if( hr == D3DERR_NOTAVAILABLE )
      return FALSE;
  }

  //
  // Do we support hardware vertex processing? if so, use it. 
  // If not, downgrade to software.
  //

  D3DCAPS9 d3dCaps;

  if( FAILED( g_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, 
    D3DDEVTYPE_HAL, &d3dCaps ) ) )
  {
    return FALSE;
  }

  DWORD dwBehaviorFlags = 0;

  if( d3dCaps.VertexProcessingCaps != 0 )
    dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
  else
    dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

  //
  // Everything checks out - create a simple, windowed device.
  //

  D3DPRESENT_PARAMETERS d3dpp;
  memset(&d3dpp, 0, sizeof(d3dpp));

  d3dpp.BackBufferCount        = 1;
  d3dpp.BackBufferFormat       = d3ddm.Format;
  d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  d3dpp.Windowed               = TRUE;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
  d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_ONE;

  if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
    dwBehaviorFlags, &d3dpp, &g_pd3dDevice ) ) )
  {
    return FALSE;
  }

  return TRUE;
}

#endif

//----------------------------------------------------------------------------

void modal_dialog(HWND hWnd)
{
#if USE_OPENGL
      MessageBox(hWnd, TEXT("OpenGL mode\n\nThis dialog box should open in front of the main window.\n\nDoes it ? On my Win7, it does not."), TEXT("Hello"), MB_OK);
#else
      MessageBox(hWnd, TEXT("DirectX mode\n\nThis dialog box should open in front of the main window.\n\nDoes it ? On my Win7, it does... Damn OpenGL..."), TEXT("Hello"), MB_OK);
#endif
}

//----------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
#if POPUP_MODAL_ON_TIMER

  case WM_TIMER:
    modal_dialog(hWnd);
    break;

#endif

  case WM_RBUTTONUP:
    modal_dialog(hWnd);
    break;

  case WM_KEYUP:
    if (wParam == VK_SPACE)
      modal_dialog(hWnd);
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

//----------------------------------------------------------------------------
