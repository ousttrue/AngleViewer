#include "node_animation.h"
#include "node.h"


void NodeRotation::Update(Node *pNode, const AnimationTime &time)
{
    m_angle += m_angularVelocityRadians * time.DeltaSeconds;
	pNode->euler.z = m_angle;
}
