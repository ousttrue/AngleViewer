#include "node_animation.h"
#include "node.h"


void NodeRotation::Update(Node *pNode, const AnimationTime &time)
{
    m_angle += m_angularVelocityRadians * time.DeltaSeconds;
	pNode->SetRotatin(glm::vec3(0, 0, 1.0f), m_angle);
}
