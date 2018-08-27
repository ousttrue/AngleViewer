#include "resource.h"
#include "eglapp.h"
#include <gles3renderer.h>
#include <scene.h>

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


///
/// static functions
///
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

		case WM_ERASEBKGND:
			return 0;

		case WM_SIZE:
			g_renderer->Resize(LOWORD(lParam), HIWORD(lParam));
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
		g_scene->Update();
		g_renderer->Draw(g_scene);
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
