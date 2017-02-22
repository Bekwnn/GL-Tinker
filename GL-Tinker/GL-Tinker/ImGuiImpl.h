/// Implimentation of ImGui with the win32 opengl context
/// Based on imgui_impl_glfw_gl3.h/cpp in the imgui examples

#pragma once
#include <imgui.h>
#include "GLWindowH.h"

IMGUI_API bool        ImGui_ImplGL3_Init(GLWindowH* window);
IMGUI_API void        ImGui_ImplGL3_Shutdown();
IMGUI_API void        ImGui_ImplGL3_NewFrame();

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplGL3_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplGL3_CreateDeviceObjects();

// ImGui's wndproc function for input
//IMGUI_API LRESULT     ImGui_ImplGL3_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);