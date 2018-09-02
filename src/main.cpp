#include <Windows.h>
#include <windowsx.h>
#include "resource.h"
#include "eglapp.h"

#include <gles3renderer.h>
#include <scene.h>
#include <gui.h>

#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>
#include <assert.h>
#include <string>
#include <stdint.h>


///
/// const
///
const auto RESOURCE_TYPE = L"SHADERSOURCE";


///
/// globals
///
GLES3Renderer *g_renderer=nullptr;
Scene *g_scene = nullptr;
int g_w = 0;
int g_h = 0;


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
		if (!m_mouseBits) {
			SetCapture(hwnd);
			LOGD << "SetCapture";
		}
		m_mouseBits = static_cast<MouseButton>(m_mouseBits | button);
	}

	void Up(MouseButton button)
	{
		m_mouseBits = static_cast<MouseButton>(m_mouseBits & ~button);
		if (!m_mouseBits) {
			ReleaseCapture();
			LOGD << "ReleaseCapture";
		}
	}
};
MouseCapture m_capture;


///
/// static functions
///
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_ERASEBKGND:
	{
		return 0;
	}

	case WM_SIZE:
	{
		g_w = LOWORD(lParam);
		g_h = HIWORD(lParam);
		g_renderer->Resize(g_w, g_h);
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
		g_scene->MouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		m_capture.Down(MouseCapture::LEFT, hwnd);
		g_scene->MouseLeftDown();
		return 0;
	}

	case WM_LBUTTONUP:
	{
		m_capture.Up(MouseCapture::LEFT);
		g_scene->MouseLeftUp();
		return 0;
	}

	case WM_MBUTTONDOWN:
	{
		m_capture.Down(MouseCapture::MIDDLE, hwnd);
		g_scene->MouseMiddleDown();
		return 0;
	}

	case WM_MBUTTONUP:
	{
		m_capture.Up(MouseCapture::MIDDLE);
		g_scene->MouseMiddleUp();
		return 0;
	}

	case WM_RBUTTONDOWN:
	{
		m_capture.Down(MouseCapture::RIGHT, hwnd);
		g_scene->MouseRightDown();
		return 0;
	}

	case WM_RBUTTONUP:
	{
		m_capture.Up(MouseCapture::RIGHT);
		g_scene->MouseRightUp();
		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		g_scene->MouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		return 0;
	}

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


static int mainloop(HWND hwnd)
{
	EglApp app(hwnd);
	LOGD << "egl initialized";

	GUI gui;

	while (true)
	{
		// message pump
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) {
				return (int)msg.wParam;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// rendering
		g_scene->Update(timeGetTime());
		g_renderer->Draw(g_scene);

		gui.Render(g_scene, g_w, g_h);
		app.present();
	}

	// never reach here
	return 0;
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


///
/// main
///
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
	plog::init(plog::verbose, &debugOutputAppender);

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
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"GL Window";
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	auto vs = GetResource(hInstance, ID_VS, RESOURCE_TYPE);
	auto fs = GetResource(hInstance, ID_FS, RESOURCE_TYPE);

	Scene scene;
	scene.AddTriangle(
		std::string(vs.begin(), vs.end()),
		std::string(fs.begin(), fs.end()));
	g_scene = &scene;

	GLES3Renderer renderer;
	g_renderer = &renderer;

	LOGD << "CreateWindow";
	HWND hwnd = CreateWindow(
		L"GL Window",
		L"GL Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		wndclass.hInstance,
		NULL);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	return mainloop(hwnd);
}
