/// wrapper for the application instance

#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <memory>
#include "GLWindowH.h"

class ApplicationH
{
private:
	ApplicationH();
public:
	~ApplicationH();

	HINSTANCE hInst; //the application instance
	WNDCLASSEX wcex; //application struct (icon, color, etc)
	std::vector<std::unique_ptr<GLWindowH>> windows;

	static ApplicationH* CreateApplicationH(HINSTANCE hInstance, WNDPROC wndProc, TCHAR* szWindowClass);
	GLWindowH* CreateGLWindowH(std::string winTitle, int width, int height);
};