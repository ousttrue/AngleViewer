#include <stdint.h>
#include <memory>
#include <string>
#include <glm/glm.hpp>


class Shader
{
	uint32_t m_program = 0;

public:
	Shader(uint32_t program);

	static std::shared_ptr<Shader> Create(const std::string &vs, const std::string &fs);
	
	uint32_t GetUniformLocation(const std::string &name);
	void SetUniformValue(uint32_t location, const glm::mat4 &m);

	void Use();
};
