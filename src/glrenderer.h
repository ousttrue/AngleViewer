#include <stdint.h>
#include <string>


class GlRenderer
{
	int m_initialized = false;
	int m_width = 0;
	int m_height = 0;
	uint8_t m_programObject = 0;
	std::string m_vs;
	std::string m_fs;

public:
	GlRenderer(const std::string &vs, const std::string &fs);
	void resize(int w, int h);
	void update();
	void draw();

private:
	bool initilize();
};
