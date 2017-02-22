#pragma once
#include "Scene.h"

class SampleScene : public Scene
{
public:
	SampleScene();
	virtual ~SampleScene();

	void UpdateGUI() override;
	void UpdateScene() override;

	glm::vec3 clearColor;
};