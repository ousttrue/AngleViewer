#include <EGL/egl.h>


class EglApp
{
	EGLContext m_context = nullptr;
	EGLDisplay m_display = nullptr;
	EGLSurface m_surface = nullptr;

public:
	EglApp(HWND hwnd);
	~EglApp();
	void present();
};
