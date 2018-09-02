#include "node.h"
#include <plog/Log.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


glm::mat4 Node::GetTransform()const
{
	return glm::translate(m_position) 
		* glm::rotate(m_angleDegree, m_axis)
		* glm::scale(m_scale)
		;
}

void Node::ForwardWheel(int d)
{
	if (d < 0) {
		m_position.z *= 1.1f;
	}
	else if (d > 0) {
		m_position.z *= 0.9f;
	}
}
