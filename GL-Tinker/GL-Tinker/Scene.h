/// an abstract class which defines the interface of a scene

#pragma once
#include "STime.h" //for convenience
#include <string>
#include <vector>
#include <memory>
#include "Actor.h"
#include "Camera.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>

class Scene
{
public:
	Scene();
	virtual void UpdateGUI() = 0;
	virtual void UpdateScene();

	std::vector<std::unique_ptr<Actor>> sceneActors;
	std::unique_ptr<Camera> camera;
	glm::vec3 light;

	GLuint shadowFrameBuffer;
	GLuint depthTexture;
};