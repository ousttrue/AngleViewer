#include "node.h"
#define _USE_MATH_DEFINES
#include <math.h>


const auto TO_RAD = M_PI / 180.0;


void Node::Update()
{
    m_angle += static_cast<float>(1 * TO_RAD);
    m_transform = glm::rotate(glm::mat4(1.0f), m_angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

