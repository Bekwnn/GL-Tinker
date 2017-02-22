/// represents an object which exists in space within the scene

#pragma once

#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

class Scene;

class Actor
{
public:
	Scene const* scene;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	virtual void Tick() {};
	virtual void ShadowPass() {};
	virtual void Render() {};

	void SetPosition(glm::vec3 val);
	void SetRotation(glm::quat val);
	void SetScale(glm::vec3 val);
	glm::mat4x4 GetModelMatrix();

	glm::vec3 GetForwardVec();
	glm::vec3 GetUpVec();
	glm::vec3 GetRightVec();
};