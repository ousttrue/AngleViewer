#include <Windows.h>
#include <windowsx.h>
#include "eglapp.h"

#include <gles3renderer.h>
#include <scene.h>
#include <gui.h>
#include "guistate.h"

#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>
#include <assert.h>
#include <string>
#include <stdint.h>
#include "resource.h"
#include "save_windowplacement.h"

///
/// const
///
const auto CLASS_NAME = L"AngleViewerWindow";
const auto WINDOW_NAME = L"AngleViewer";

///
/// ShaderResource
///
const auto RESOURCE_TYPE = L"SHADERSOURCE";
static std::string to_string(const std::vector<uint8_t> &src)
{
    return std::string(src.begin(), src.end());
}
static std::vector<uint8_t> GetResource(HINSTANCE hInst, int id, const wchar_t *resource_type)
{
    auto hRes = FindResource(hInst, MAKEINTRESOURCE(id), resource_type);
    auto hMem = LoadResource(hInst, hRes);
    auto size = SizeofResource(hInst, hRes);
    auto locked = LockResource(hMem);
    std::vector<uint8_t> data(size);
    memcpy(data.data(), locked, size);
    FreeResource(hMem);
    return data;
}

static void LoadResource(HINSTANCE hInstance)
{
    // set shader
    agv::renderer::ShaderSourceManager::Instance.SetSource(
        agv::scene::ShaderType::gizmo,
        to_string(GetResource((HINSTANCE)hInstance, ID_GIZMO_VS, RESOURCE_TYPE)),
        to_string(GetResource((HINSTANCE)hInstance, ID_GIZMO_FS, RESOURCE_TYPE)));

    // set shader
    agv::renderer::ShaderSourceManager::Instance.SetSource(
        agv::scene::ShaderType::unlit,
        to_string(GetResource((HINSTANCE)hInstance, ID_UNLIT_VS, RESOURCE_TYPE)),
        to_string(GetResource((HINSTANCE)hInstance, ID_UNLIT_FS, RESOURCE_TYPE)));
}

///
/// globals
///
agv::renderer::GLES3Renderer *g_renderer = nullptr;
agv::scene::Scene *g_scene = nullptr;
agv::gui::GUI *g_gui = nullptr;
GuiState g_guiState;

namespace plog
{
template <class Formatter>
class ImGuiAppender : public IAppender
{
    std::string *m_buffer;

public:
    ImGuiAppender(std::string *buffer)
        : m_buffer(buffer), m_isatty(false)
    {
    }

    virtual void write(const Record &record)
    {
        util::nstring str = Formatter::format(record);
        util::MutexLock lock(m_mutex);

        for (auto c : str)
        {
            m_buffer->push_back(static_cast<char>(c));
        }
    }

protected:
    util::Mutex m_mutex;
    const bool m_isatty;
};
} // namespace plog

class MouseCapture
{
public:
    enum MouseButton : uint8_t
    {
        LEFT = 0x01,
        MIDDLE = 0x02,
        RIGHT = 0x04,
    };

private:
    MouseButton m_mouseBits = static_cast<MouseButton>(0);

public:
    void Down(MouseButton button, HWND hwnd)
    {
        if (!m_mouseBits)
        {
            SetCapture(hwnd);
            //LOGD << "SetCapture";
        }
        m_mouseBits = static_cast<MouseButton>(m_mouseBits | button);
    }

    void Up(MouseButton button)
    {
        m_mouseBits = static_cast<MouseButton>(m_mouseBits & ~button);
        if (!m_mouseBits)
        {
            ReleaseCapture();
            //LOGD << "ReleaseCapture";
        }
    }
};
MouseCapture m_capture;

///
/// static functions
///
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
    {
        windowplacement::Save(hwnd);
        PostQuitMessage(0);
        return 0;
    }

    case WM_ERASEBKGND:
    {
        static int s_counter = 0;
        if (s_counter++)
        {
            return 0;
        }
        break;
    }

    case WM_SIZE:
    {
        auto w = LOWORD(lParam);
        auto h = HIWORD(lParam);
        g_scene->GetCamera()->SetScreenSize(w, h);
        g_renderer->Resize(w, h);
        g_gui->SetScreenSize(w, h);
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_MOUSEMOVE:
    {
        auto x = GET_X_LPARAM(lParam);
        auto y = GET_Y_LPARAM(lParam);
        g_gui->MouseMove(x, y);
        g_scene->GetMouseObserver()->MouseMove(x, y);
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        m_capture.Down(MouseCapture::LEFT, hwnd);
        if (g_gui->IsHover())
        {
            g_gui->MouseLeftDown();
        }
        else
        {
            g_scene->GetMouseObserver()->MouseLeftDown();
        }
        return 0;
    }

    case WM_LBUTTONUP:
    {
        m_capture.Up(MouseCapture::LEFT);
        g_gui->MouseLeftUp();
        g_scene->GetMouseObserver()->MouseLeftUp();
        return 0;
    }

    case WM_MBUTTONDOWN:
    {
        m_capture.Down(MouseCapture::MIDDLE, hwnd);
        if (g_gui->IsHover())
        {
            g_gui->MouseMiddleDown();
        }
        else
        {
            g_scene->GetMouseObserver()->MouseMiddleDown();
        }
        return 0;
    }

    case WM_MBUTTONUP:
    {
        m_capture.Up(MouseCapture::MIDDLE);
        g_gui->MouseMiddleUp();
        g_scene->GetMouseObserver()->MouseMiddleUp();
        return 0;
    }

    case WM_RBUTTONDOWN:
    {
        m_capture.Down(MouseCapture::RIGHT, hwnd);
        if (g_gui->IsHover())
        {
            g_gui->MouseRightDown();
        }
        else
        {
            g_scene->GetMouseObserver()->MouseRightDown();
        }
        return 0;
    }

    case WM_RBUTTONUP:
    {
        m_capture.Up(MouseCapture::RIGHT);
        g_gui->MouseRightUp();
        g_scene->GetMouseObserver()->MouseRightUp();
        return 0;
    }

    case WM_MOUSEWHEEL:
    {
        auto d = GET_WHEEL_DELTA_WPARAM(wParam);

        if (g_gui->IsHover())
        {
            g_gui->MouseWheel(d);
        }
        else
        {
            g_scene->GetMouseObserver()->MouseWheel(d);
        }

        return 0;
    }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

static int mainloop(HWND hwnd)
{
    EglApp app(hwnd);
    LOGD << "egl initialized";

    DWORD lastTime = 0;

    while (true)
    {
        // message pump
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
            {
                return (int)msg.wParam;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // rendering
        auto now = timeGetTime();
        auto delta = now - lastTime;
        lastTime = now;

        {
            g_gui->Begin(delta * 0.001f);

            g_scene->Update(now);
            g_guiState.Update(g_scene);

            g_renderer->Draw(g_scene);
            g_gui->End();
        }
        app.present();
    }

    // never reach here
    return 0;
}

static std::wstring SjisToUnicode(const std::string &src)
{
    auto size = MultiByteToWideChar(CP_OEMCP, 0, src.c_str(), -1, NULL, 0);
    if (size == 0)
    {
        return L"";
    }
    std::vector<wchar_t> buf(size - 1);
    size = MultiByteToWideChar(CP_OEMCP, 0, src.c_str(), -1, &buf[0], static_cast<int>(buf.size()));
    return std::wstring(buf.begin(), buf.end());
}

///
/// main
///
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadResource(hInstance);

    //static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
    static plog::ImGuiAppender<plog::TxtFormatter> appender(&g_guiState.logger);
    plog::init(plog::verbose, &appender);

    // setup window
    WNDCLASSEX wndclass;
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = GetModuleHandle(NULL);
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    ;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = CLASS_NAME;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wndclass);

    agv::scene::Scene scene;

    if (__argc == 1)
    {
        scene.CreateDefaultScene();
    }
    else
    {
        scene.Load(SjisToUnicode(__argv[1]));
    }

    g_scene = &scene;

    agv::renderer::GLES3Renderer renderer;
    g_renderer = &renderer;

    agv::gui::GUI gui;
    g_gui = &gui;

    LOGD << "CreateWindow";
    HWND hwnd = CreateWindow(
        CLASS_NAME,
        WINDOW_NAME,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        wndclass.hInstance,
        NULL);

    // ShowWindow(hwnd, SW_SHOWNORMAL);
    windowplacement::Restore(hwnd, SW_SHOWNORMAL);

    auto code = mainloop(hwnd);

    return code;
}
