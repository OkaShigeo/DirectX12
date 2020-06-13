#pragma once
#include "Vector.h"

struct HWND__;

class Window
{
public:
	/* コンストラクタ */
	Window(const st::Vec2& size, const st::Vec2& pos = 0x80000000);
	/* デストラクタ */
	~Window();
	/* ウィンドウ表示 */
	void ShowWndow(void) const;

public:
	/* ウィンドウハンドルの取得 */
	HWND__* Get(void) const;
	/* ウィンドウのサイズを求める */
	st::Vec2 GetWindowSize(void) const;
	/* ウィンドウの座標を求める */
	st::Vec2 GetWindowPos(void) const;

private:
	/* ウィンドウハンドルの生成 */
	std::uint32_t CreateWindowHandle(const st::Vec2& size, const st::Vec2& pos = 0x80000000);


private:
	/* ウィンドウハンドル */
	HWND__* handle;
	/* ウィンドウクラス名 */
	const wchar_t* name;
	/* インスタンス */
	const void* instance;
};
