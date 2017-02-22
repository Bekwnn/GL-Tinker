#include <windows.h>

#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <imgui.h>

#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include "ApplicationH.h"

#include "ImGuiImpl.h"
#include "STime.h"
#include "SampleScene.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	ApplicationH* app = ApplicationH::CreateApplicationH(hInstance, WndProc, _T("OpenGL"));

	GLWindowH* mainWindow = app->CreateGLWindowH("Main Window", WIN_WIDTH, WIN_HEIGHT);

	//INITIALIZE GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Error: %s\n", glewGetErrorString(err));
		return 0;
	}

	//display window
	ShowWindow(mainWindow->hWnd,
		nCmdShow);
	UpdateWindow(mainWindow->hWnd);

	//INITIALIZE IMGUI
	ImGui_ImplGL3_Init(mainWindow);

	//INITIALIZE SCENE
	mainWindow->SetScene(new SampleScene());

	//INITIALIZE SCENE TIME CLASS
	STime::InitTime();

	// Main message loop:
	bool bflicker = false;
	MSG msg;
	GetMessage(&msg, mainWindow->hWnd, 0, 0);
	while (msg.message != WM_QUIT) //add some && bGameIsPlaying bool to cond
	{
		//process and remove win32 event messages
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//render update
		else
		{
			STime::UpdateDeltaTime();

			mainWindow->scene->UpdateScene();

			ImGui_ImplGL3_NewFrame();
			mainWindow->scene->UpdateGUI();
			ImGui::Render();

			SwapBuffers(mainWindow->hDc);
		}
	}

	ImGui_ImplGL3_Shutdown();

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
extern LRESULT ImGui_ImplGL3_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplGL3_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}