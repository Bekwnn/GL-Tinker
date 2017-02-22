//wrapper for the WINAPI window

#pragma once
#include <windows.h>
#include <string>
#include <memory>
#include "Scene.h"

class GLWindowH
{
public:
	GLWindowH();
	~GLWindowH();

	void SetScene(Scene* scene);

	HWND hWnd; //WINAPI handle for the window
	HDC hDc; //WINAPI handle for device context
	HGLRC glC; //WINAPI handle for opengl context

	std::unique_ptr<Scene> scene;
};