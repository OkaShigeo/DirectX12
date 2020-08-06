#pragma once
#include "Vector.h"

struct HWND__;

class Window
{
public:
	/* コンストラクタ */
	Window(const Dx12::Vec2& size, const Dx12::Vec2& location = 0x80000000);
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

public:
	/* メッセージの確認 */
	static bool CheckMsg(void);

private:
	/* ウィンドウの生成 */
	HWND__* CreateWindowHandle(const Dx12::Vec2& size, const Dx12::Vec2& location);

private:
	/* ハンドル */
	HWND__* handle{ nullptr };
	/* クラス名 */
	const wchar_t* name{ nullptr };
	/* インスタンス */
	const void* instance{ nullptr };
};
