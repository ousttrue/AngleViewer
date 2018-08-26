#include <stdint.h>


class GlRenderer
{
	int m_initialized = false;
	int m_width = 0;
	int m_height = 0;
	uint8_t m_programObject = 0;
public:
    bool initilize();
	void resize(int w, int h);
    void update();
    void draw();
};
