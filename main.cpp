#include "eglapp.h"
#include "glrenderer.h"
#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>
#include <assert.h>


GlRenderer *g_renderer=nullptr;


static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

		case WM_ERASEBKGND:
			return 0;

		case WM_SIZE:
			g_renderer->resize(LOWORD(lParam), HIWORD(lParam));
			return 0;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);
			}
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


static int mainloop(HWND hwnd)
{
	EglApp app(hwnd);
	LOGD << "egl initialized";

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
		g_renderer->update();
		g_renderer->draw();
		app.present();
	}

	// never reach here
	return 0;
}


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

	GlRenderer renderer;
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
