#include "node_animation.h"
#include "node.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


void NodeRotation::Update(Node *pNode, const AnimationTime &time)
{
    m_angle += m_angularVelocityRadians * time.DeltaSeconds;
	pNode->transform = glm::rotate(m_angle, glm::vec3(0, 0, 1.0f));
}
