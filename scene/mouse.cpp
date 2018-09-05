#include "mouse.h"
#include "node.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>


const float RORATION_SPEED =  glm::radians(1.0f);
const float SHIFT_SPEED = 0.001f;

namespace agv {
	namespace scene {
		void OrbitMover::MouseMove(int x, int y)
		{
			auto dx = x - m_mouseLastX;
			auto dy = y - m_mouseLastY;
			m_mouseLastX = x;
			m_mouseLastY = y;

			bool isUpdated = false;
			if (m_mouseLeftIsDown) {

			}

			if (m_mouseMiddleIsDown) {
				m_shiftX += (dx * m_distance * SHIFT_SPEED);
				m_shiftY -= (dy * m_distance * SHIFT_SPEED);
				isUpdated = true;
			}

			if (m_mouseRightIsDown) {
				// Orbit
				m_yaw += (dx * RORATION_SPEED);
				m_pitch += (dy * RORATION_SPEED);
				if (m_pitch > glm::half_pi<float>()) {
					m_pitch = glm::half_pi<float>();
				}
				else if (m_pitch < -glm::half_pi<float>()) {
					m_pitch = -glm::half_pi<float>();
				}
				isUpdated = true;
			}

			if (isUpdated) {
				Update();
			}
		}

		void OrbitMover::MouseLeftDown()
		{
			m_mouseLeftIsDown = true;
		}

		void OrbitMover::MouseLeftUp()
		{
			m_mouseLeftIsDown = false;
		}

		void OrbitMover::MouseMiddleDown()
		{
			m_mouseMiddleIsDown = true;
		}

		void OrbitMover::MouseMiddleUp()
		{
			m_mouseMiddleIsDown = false;
		}

		void OrbitMover::MouseRightDown()
		{
			m_mouseRightIsDown = true;
		}

		void OrbitMover::MouseRightUp()
		{
			m_mouseRightIsDown = false;
		}

		void OrbitMover::MouseWheel(int d)
		{
			if (d < 0) {
				m_distance *= 1.1f;
				Update();
			}
			else if (d > 0) {
				m_distance *= 0.9f;
				Update();
			}
		}

		void OrbitMover::Update()
		{
			m_node->transform =
				glm::translate(glm::mat4(1), glm::vec3(m_shiftX, m_shiftY, -m_distance))
				* glm::transpose(glm::eulerAngleYXZ(-m_yaw, -m_pitch, 0.0f))
				;
		}
	}
}