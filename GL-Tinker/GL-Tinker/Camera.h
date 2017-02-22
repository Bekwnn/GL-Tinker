#include "Actor.h"

#include <GL\glew.h>

class Camera : public Actor
{
public:
	Camera();
	~Camera();

	virtual void Tick() override;

	void UpdateVP();
	glm::vec2 GetMousePos();
	void Movement();
	void FPSLook();

	GLint viewport[4];
	double nearClip, farClip;
	double fovHalf;

	float speed, sensitivity;

	bool rmbWasDown;
	glm::vec2 mousePrevious;
	glm::vec2 mouseDelta;

	glm::vec3 cameraLookAt;
	glm::vec3 cameraUp;

	glm::mat4x4 view;
	glm::mat4x4 projection;
	glm::mat4x4 VP;
};