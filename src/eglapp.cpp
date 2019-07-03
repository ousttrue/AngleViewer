#include "eglapp.h"
#include <assert.h>

EglApp::EglApp(HWND hwnd)
{
    // m_display
    m_display = eglGetDisplay(GetDC(hwnd));
    assert(m_display);

    EGLint major, minor;
    assert(eglInitialize(m_display, &major, &minor));

    assert(eglBindAPI(EGL_OPENGL_ES_API));

    // config
    EGLint configAttributes[] =
        {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_BUFFER_SIZE, 32,
            EGL_DEPTH_SIZE, 24,
            EGL_STENCIL_SIZE, 8,

            // EGL_RED_SIZE, 5,
            // EGL_GREEN_SIZE, 6,
            // EGL_BLUE_SIZE, 5,
            // EGL_ALPHA_SIZE, 0,
            // EGL_DEPTH_SIZE, 16,
            // EGL_STENCIL_SIZE, 0,
            // EGL_SAMPLES, 4,

            EGL_NONE};
    EGLConfig config;
    EGLint num_config;
    assert(eglChooseConfig(m_display, configAttributes, &config, 1, &num_config));

    // context
    EGLint contextAttributes[] =
        {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE};
    EGLContext context = eglCreateContext(m_display, config, NULL, contextAttributes);
    assert(context);

    // surface
    m_surface = eglCreateWindowSurface(m_display, config, hwnd, NULL);
    assert(m_surface);

    assert(eglMakeCurrent(m_display, m_surface, m_surface, context));
}

EglApp::~EglApp()
{
    if (m_context != EGL_NO_CONTEXT)
    {
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(m_display, m_context);
        m_context = EGL_NO_CONTEXT;
    }

    if (m_surface != EGL_NO_SURFACE)
    {
        eglDestroySurface(m_display, m_surface);
        m_surface = EGL_NO_SURFACE;
    }

    if (m_display != EGL_NO_DISPLAY)
    {
        eglTerminate(m_display);
        m_display = EGL_NO_DISPLAY;
    }
}

void EglApp::present()
{
    assert(eglSwapBuffers(m_display, m_surface));
}
