#pragma once
#include "Vector.h"

struct HWND__;

class Window
{
public:
	/*＊ メッセージの確認 
	 * @return メッセージのフラグ true:メッセージあり/false:メッセージ無し 
	 */
	static bool CheckMsg(void);
	/*＊ ウィンドウの生成 
	 * @param size ウィンドウサイズ
	 * @param location 表示左上位置
	 * @return ウィンドウハンドル
	 */
	static HWND__* CreateWindowHandle(const Math::Vec2& size, const Math::Vec2& location);

public:
	/** コンストラクタ */
	Window();
	/** コンストラクタ
	 * @param size ウィンドウサイズ
	 * @param location 表示左上位置
	 */
	Window(const Math::Vec2& size, const Math::Vec2& location = 0x80000000);
	/** コンストラクタ
	 * @param ウィンドウハンドル
	 */
	Window(HWND__* window);
	/** デストラクタ */
	~Window();

public:
	/** ウィンドウの表示 */
	void Show(void) const;

public:
	/** ハンドルの取得
	 * @return ウィンドウハンドル
	 */
	HWND__* Get(void) const;
	/** サイズの取得 
	 * @return ウィンドウサイズ
	 */
	Math::Vec2 GetSize(void) const;
	/*＊ 表示座標の取得 
	 * @return ウィンドウの表示左上位置
	 */
	Math::Vec2 GetLocation(void) const;

private:
	/** メモリの解放 */
	void Release(void);

private:
	/* ハンドル */
	HWND__* handle{ nullptr };
};
