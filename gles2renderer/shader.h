#include <stdint.h>
#include <memory>
#include <string>


class Shader
{
	uint32_t m_program = 0;

public:
	Shader(uint32_t program);

	static std::shared_ptr<Shader> Create(const std::string &vs, const std::string &fs);
	
	void Use();
};
