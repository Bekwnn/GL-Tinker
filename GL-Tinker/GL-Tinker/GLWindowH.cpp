
#include "GLWindowH.h"

GLWindowH::GLWindowH()
{
}

GLWindowH::~GLWindowH()
{
	ReleaseDC(hWnd, hDc);
	wglDeleteContext(glC);
	DestroyWindow(hWnd);
}

void GLWindowH::SetScene(Scene* scene)
{
	this->scene = std::unique_ptr<Scene>(scene);
}
