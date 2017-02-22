#include <GL\glew.h>
#include <GL\GL.h>

#include <imgui.h>
#include <assimp\scene.h>

#include "SampleScene.h"
#include "AssimpUtil.h"

SampleScene::SampleScene() :
	Scene::Scene()
{
	clearColor = glm::vec3(0.6, 0.6, 0.7);
	camera = std::unique_ptr<Camera>(new Camera());
}

SampleScene::~SampleScene() {}

void SampleScene::UpdateGUI()
{
	ImGui::Begin("Debug");
	ImGui::ColorEdit3("Clear Color", (float*)&clearColor[0]);
	ImGui::Text("Camera Position: (%f, %f, %f)", camera->position.x, camera->position.y, camera->position.z);
	glm::vec3 lookdir = camera->GetForwardVec();
	ImGui::Text("Camera Look Dir: (%f, %f, %f)", lookdir.x, lookdir.y, lookdir.z);
	ImGui::End();
}

void SampleScene::UpdateScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Scene::UpdateScene();
}