#pragma once
#include "animation_time.h"
#include <glm/glm.hpp>


namespace agv {
	namespace scene {

		class Node;
		class INodeAnimation
		{
		public:
			virtual void Update(Node *pNode, const AnimationTime &time) = 0;
		};


		class NodeRotation : public INodeAnimation
		{
			float m_angularVelocityRadians;
			float m_angle = 0;

		public:
			NodeRotation(float angularVelocity)
				: m_angularVelocityRadians(glm::radians<float>(angularVelocity))
			{}

			void Update(Node *pNode, const AnimationTime &time)override;
		};
	}
}
