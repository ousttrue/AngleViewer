#include "node_animation.h"
#include "node.h"


void NodeRotation::Update(Node *pNode, const AnimationTime &time)
{
    m_angle += m_angularVelocityRadians * time.DeltaSeconds;
    pNode->SetTransform(glm::rotate(glm::mat4(1.0f), m_angle, glm::vec3(0.0f, 0.0f, 1.0f)));
}
