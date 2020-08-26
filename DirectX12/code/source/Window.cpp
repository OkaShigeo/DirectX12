#include "..\include\Window.h"
#include <Windows.h>
#include <vector>
#include <cassert>

/* ウィンドウコールバック */
static INT64 __stdcall WindowCallback(HWND__* handle, UINT32 message, UINT64 wparam, INT64 lparam)
{
	switch (message)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(handle, message, wparam, lparam);
}

HWND__ * Window::CreateWindowHandle(const Dx12::Vec2& size, const Dx12::Vec2& location)
{
	WNDCLASSEX wnd{};
	wnd.cbClsExtra    = 0;
	wnd.cbSize        = sizeof(WNDCLASSEX);
	wnd.cbWndExtra    = 0;
	wnd.hbrBackground = CreateSolidBrush(COLOR_APPWORKSPACE);
	wnd.hInstance     = GetModuleHandle(nullptr);
	wnd.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wnd.hIcon         = LoadIcon(wnd.hInstance, nullptr);
	wnd.hIconSm       = wnd.hIcon;
	wnd.lpfnWndProc   = WNDPROC(WindowCallback);
	wnd.lpszClassName = L"おかもん";
	wnd.lpszMenuName  = wnd.lpszClassName;
	wnd.style         = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wnd);

	RECT rect{};
	rect.bottom = size.y;
	rect.left   = 0;
	rect.right  = size.x;
	rect.top    = 0;
	
	return CreateWindowEx(WS_EX_ACCEPTFILES, wnd.lpszClassName, wnd.lpszClassName, WS_OVERLAPPEDWINDOW, 0x80000000, 0x80000000,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);
}

bool Window::CheckMsg(void)
{
	static MSG msg{};
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		switch (msg.message)
		{
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if (msg.wParam == VK_ESCAPE) {
				return false;
			}
			break;
		case WM_QUIT:
			return false;
		default:
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}


Window::Window()
{
}

Window::Window(const Dx12::Vec2& size, const Dx12::Vec2& location)
{
	handle = CreateWindowHandle(size, location);
	assert(handle != nullptr);
}

Window::Window(HWND__* window)
{
	Release();
	handle = window;
}

Window::~Window()
{
	Release();
}

void Window::Show(void) const
{
	if (handle != nullptr) {
		ShowWindow(handle, SW_SHOW);
	}
}

HWND__ * Window::Get(void) const
{
	return handle;
}

Dx12::Vec2 Window::GetSize(void) const
{
	Dx12::Vec2 size = 0;
	if (handle != nullptr) {
		RECT rect{};
		if (GetClientRect(handle, &rect) != 0) {
			size.x = rect.right;
			size.y = rect.bottom;
		}
	}

	return size;
}

Dx12::Vec2 Window::GetLocation(void) const
{
	Dx12::Vec2 location = 0;
	if (handle != nullptr) {
		RECT rect{};
		if (GetWindowRect(handle, &rect) != 0) {
			location.x = rect.right;
			location.y = rect.bottom;
		}
	}

	return location;
}

void Window::Release(void)
{
	if (handle != nullptr) {
		std::vector<wchar_t>name(256);
		std::uint32_t num = GetClassName(handle, &name[0], std::int32_t(name.size()));
		assert(num != 0);
		name.resize(num + 1);

		WNDCLASSEX wnd{};
		assert(GetClassInfoEx(GetModuleHandle(nullptr), name.data(), &wnd) != 0);
		
		assert(DestroyWindow(handle) != 0);
		assert(UnregisterClass(wnd.lpszClassName, wnd.hInstance) != 0);
		handle = nullptr;
	}
}
