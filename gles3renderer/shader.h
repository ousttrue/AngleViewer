#include <stdint.h>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "material.h"


namespace agv {
	namespace renderer {
		class Shader
		{
			uint32_t m_program = 0;

		public:
			Shader(uint32_t program);

			static std::shared_ptr<Shader> Create(const std::shared_ptr<scene::Material> &material);

			uint32_t GetUniformLocation(const std::string &name);
			void SetUniformValue(uint32_t location, const glm::mat4 &m);
			void SetUniformValue(const std::string &name, const glm::mat4 &m);

			void Use();
		};
	}
}