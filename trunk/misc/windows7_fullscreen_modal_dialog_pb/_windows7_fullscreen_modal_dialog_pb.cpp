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

BOOL initOpenGL(HWND hWnd)
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
