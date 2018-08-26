#include <stdint.h>
#include <string>


class Shader;
class GlRenderer
{
	std::shared_ptr<Shader> m_shader;
	int m_width = 0;
	int m_height = 0;
	std::string m_vs;
	std::string m_fs;

public:
	GlRenderer(const std::string &vs, const std::string &fs);
	void resize(int w, int h);
	void update();
	void draw();
};
