#include "node.h"
#include <plog/Log.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>


glm::mat4 Node::GetTransform()const
{
	return glm::translate(glm::mat4(1), position)
		* glm::eulerAngleYXZ(euler.y, euler.x, euler.z)
		;
}

glm::mat4 Node::GetInverse()const
{
	return glm::transpose(glm::eulerAngleYXZ(euler.y, euler.x, euler.z))
		* glm::translate(glm::mat4(1), -position)
		;
}

void Node::ForwardWheel(int d)
{
	if (d < 0) {
		position.z *= 1.1f;
	}
	else if (d > 0) {
		position.z *= 0.9f;
	}
}
