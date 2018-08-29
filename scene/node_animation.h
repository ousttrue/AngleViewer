#pragma once
#include "animation_time.h"
#define _USE_MATH_DEFINES
#include <math.h>
const auto TO_RAD = M_PI / 180.0;


class Node;
class INodeAnimation
{
public:
    virtual void Update(Node *pNode, const AnimationTime &time)=0;
};


class NodeRotation: public INodeAnimation
{
    float m_angularVelocityRadians;
    float m_angle=0;

public:
    NodeRotation(float angularVelocity)
        : m_angularVelocityRadians(static_cast<float>(angularVelocity * TO_RAD))
    {}

    void Update(Node *pNode, const AnimationTime &time)override;
};
