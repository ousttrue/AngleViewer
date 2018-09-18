#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace agv {
	namespace scene {
		class ICamera
		{
		public:
			virtual glm::mat4 GetMatrix()const = 0;
			virtual void SetScreenSize(int w, int h) = 0;
		};


		class PersepectiveCamera : public ICamera
		{
			float m_fovyDegree = 30.0f;
			float m_aspect = 1.0f;
			float m_near = 0.05f;
			float m_far = 100.0f;

		public:
			PersepectiveCamera()
			{
			}

			void SetScreenSize(int w, int h)
			{
				m_aspect = w / (float)h;
			}

			glm::mat4 GetMatrix()const override
			{
				return  glm::perspective(glm::radians(m_fovyDegree), m_aspect, m_near, m_far);
			}
		};
	}
}