#pragma once
#include "Vector.h"

struct HWND__;

class Window
{
public:
	/* メッセージの確認 */
	static bool CheckMsg(void);
	/* ウィンドウの生成 */
	static HWND__* CreateWindowHandle(const Dx12::Vec2& size, const Dx12::Vec2& location);

public:
	/* コンストラクタ */
	Window();
	Window(const Dx12::Vec2& size, const Dx12::Vec2& location = 0x80000000);
	Window(HWND__* window);
	/* デストラクタ */
	~Window();

public:
	/* ウィンドウの表示 */
	void Show(void) const;

public:
	/* ハンドルの取得 */
	HWND__* Get(void) const;
	/* サイズの取得 */
	Dx12::Vec2 GetSize(void) const;
	/* 表示座標の取得 */
	Dx12::Vec2 GetLocation(void) const;

private:
	/* メモリの解放 */
	void Release(void);

private:
	/* ハンドル */
	HWND__* handle{ nullptr };
};
