#include "..\include\Window.h"
#include <Windows.h>

/* ウィンドウコールバック */
static INT64 __stdcall WindowCallback(HWND__* handle, UINT32 message, UINT64 wparam, INT64 lparam) {
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(handle);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(handle, message, wparam, lparam);
}

Window::Window(const st::Vec2& size, const st::Vec2& pos) : 
	handle(nullptr), name(nullptr), instance(nullptr)
{
	CreateWindowHandle(size, pos);
}

Window::~Window()
{
	if (handle != nullptr) {
		UnregisterClass(name, (HINSTANCE)instance);
		handle = nullptr;
	}
}

std::uint32_t Window::CreateWindowHandle(const st::Vec2& size, const st::Vec2& pos)
{
	if (handle != nullptr) {
		OutputDebugStringA("失敗：ウィンドウハンドル生成済み\n");
		return S_FALSE;
	}

	WNDCLASSEX desc{};
	desc.cbClsExtra    = 0;
	desc.cbSize        = sizeof(WNDCLASSEX);
	desc.cbWndExtra    = 0;
	desc.hbrBackground = CreateSolidBrush(COLOR_APPWORKSPACE);
	desc.hInstance     = GetModuleHandle(0);
	desc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	desc.hIcon         = LoadIcon(desc.hInstance, nullptr);
	desc.hIconSm       = desc.hIcon;
	desc.lpfnWndProc   = WNDPROC(WindowCallback);
	desc.lpszClassName = L"おかもん";
	desc.lpszMenuName  = desc.lpszClassName;
	desc.style         = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&desc);

	RECT rect{};
	rect.bottom = size.y;
	rect.left   = 0;
	rect.right  = size.x;
	rect.top    = 0;

	handle = CreateWindowEx(WS_EX_ACCEPTFILES, desc.lpszClassName, L"おかもん", WS_OVERLAPPEDWINDOW, pos.x, pos.y,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, desc.hInstance, nullptr);
	if (handle == nullptr) {
		OutputDebugStringA("失敗：ウィンドウハンドルの生成ができませんでした\n");
		return S_FALSE;
	}

	name     = desc.lpszClassName;
	instance = desc.hInstance;

	return S_OK;
}

void Window::ShowWndow(void) const
{
	if (handle == nullptr) {
		OutputDebugStringA("失敗：ウィンドウハンドルが生成されていません\n");
	}

	ShowWindow(handle, SW_SHOW);
}

HWND__* Window::Get(void) const
{
	if (handle == nullptr) {
		OutputDebugStringA("ウィンドウハンドルが生成されていません\n");
	}

	return handle;
}

st::Vec2 Window::GetWindowSize(void) const
{
	if (handle == nullptr) {
		OutputDebugStringA("失敗：ウィンドウハンドルが生成されていません\n");
		return 0;
	}

	RECT rect{};
	if (GetClientRect(handle, &rect) == 0) {
		OutputDebugStringA("失敗：ウィンドウサイズを求めることができませんでした\n");
		return 0;
	}

	return { std::int32_t(rect.right), std::int32_t(rect.bottom) };
}

st::Vec2 Window::GetWindowPos(void) const
{
	if (handle == nullptr) {
		OutputDebugStringA("失敗：ウィンドウハンドルが生成されていません\n");
		return 0;
	}

	RECT rect{};
	if (GetWindowRect(handle, &rect) == 0) {
		OutputDebugStringA("失敗：ウィンドウ座標を求めることができませんでした\n");
		return 0;
	}

	return { std::int32_t(rect.right), std::int32_t(rect.bottom) };
}
