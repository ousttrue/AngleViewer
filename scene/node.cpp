#include "node.h"
#include <plog/Log.h>


void Node::SetPosition(float x, float y, float z)
{
	m_transform[3] = glm::vec4(x, y, z, 0);
}

void Node::Forward(float d)
{
	auto position = m_transform[3];
	position.z += d;
	m_transform[3] = position;
}

void Node::ForwardWheel(int d)
{
	auto position = m_transform[3];
	if (d < 0) {
		position.z *= 1.1f;
	}
	else if (d > 0) {
		position.z *= 0.9f;
	}
	LOGD << position.z;
	m_transform[3] = position;
}
