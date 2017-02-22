#include "Actor.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

void Actor::SetPosition(glm::vec3 val)
{
	position = val;
}

void Actor::SetRotation(glm::quat val)
{
	rotation = val;
}

void Actor::SetScale(glm::vec3 val)
{
	scale = val;
}

glm::mat4x4 Actor::GetModelMatrix()
{
	return glm::translate(glm::mat4x4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4x4(1.0f), scale);
}

glm::vec3 Actor::GetForwardVec()
{
	return glm::vec3(2 * (rotation.x * rotation.z + rotation.w * rotation.y),
		2 * (rotation.y * rotation.z - rotation.w * rotation.x),
		1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y));
}

glm::vec3 Actor::GetUpVec()
{
	return glm::vec3(2 * (rotation.x * rotation.y - rotation.w * rotation.z),
		1 - 2 * (rotation.x * rotation.x + rotation.z * rotation.z),
		2 * (rotation.y * rotation.z + rotation.w * rotation.x));
}

glm::vec3 Actor::GetRightVec()
{
	return -glm::vec3(1 - 2 * (rotation.y * rotation.y + rotation.z * rotation.z),
		2 * (rotation.x * rotation.y + rotation.w * rotation.z),
		2 * (rotation.x * rotation.z - rotation.w * rotation.y));
}
