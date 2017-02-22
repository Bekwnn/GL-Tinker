#include "ApplicationH.h"
#include <tchar.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <wingdi.h>

// private constructor, see CreateApplicationH to create new 
ApplicationH::ApplicationH()
{ }

ApplicationH::~ApplicationH()
{
	wglMakeCurrent(NULL, NULL);
}

//returns new application, returns nullptr on failure
ApplicationH* ApplicationH::CreateApplicationH(HINSTANCE hInstance, WNDPROC wndProc, TCHAR* szWindowClass)
{
	ApplicationH* retApp = new ApplicationH();

	retApp->hInst = hInstance;

	retApp->wcex.cbSize = sizeof(WNDCLASSEX);
	retApp->wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	retApp->wcex.lpfnWndProc = wndProc;
	retApp->wcex.cbClsExtra = 0;
	retApp->wcex.cbWndExtra = 0;
	retApp->wcex.hInstance = hInstance;
	retApp->wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	retApp->wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	retApp->wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	retApp->wcex.lpszMenuName = NULL;
	retApp->wcex.lpszClassName = szWindowClass;
	retApp->wcex.hIconSm = LoadIcon(retApp->wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx( &(retApp->wcex) ))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("OpenGL Application"),
			NULL);

		return nullptr;
	}

	return retApp;
}

GLWindowH * ApplicationH::CreateGLWindowH(std::string winTitle, int width, int height)
{
	GLWindowH* retWindow = new GLWindowH();

	HWND hWnd = CreateWindow(
		_T("OpenGL"),
		_T(winTitle.c_str()),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL,
		NULL,
		hInst,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("OpenGL Application"),
			NULL);

		return nullptr;
	}

	retWindow->hWnd = hWnd;

	retWindow->hDc = GetDC(retWindow->hWnd);

	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pf = ChoosePixelFormat(retWindow->hDc, &pfd);
	if (pf == 0)
	{
		MessageBox(NULL,
			_T("Choose pixel format failed!"),
			_T("OpenGL Application"),
			NULL);

		return nullptr;
	}

	if (SetPixelFormat(retWindow->hDc, pf, &pfd) == FALSE)
	{
		MessageBox(NULL,
			_T("Set pixel format failed!"),
			_T("OpenGL Application"),
			NULL);

		return nullptr;
	}

	DescribePixelFormat(retWindow->hDc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	retWindow->glC = wglCreateContext(retWindow->hDc);
	wglMakeCurrent(retWindow->hDc, retWindow->glC);

	windows.push_back(std::unique_ptr<GLWindowH>(retWindow));

	return retWindow;
}
