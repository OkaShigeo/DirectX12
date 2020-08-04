#pragma once
#include "Window.h"
#include "Device.h"

namespace Dx12
{
	class Runtime
	{
	public:
		/* 初期化 */
		static void Initialize(const Dx12::Vec2& size, const Dx12::Vec2& pos = 0x80000000);
		/* 終了 */
		static void UnInitialize(void);

	public:
		/* ウィンドウの取得 */
		static Window* GetWindow(void);
		/* デバイスの取得 */
		static Device* GetDevice(void);

	private:
		/* コンストラクタ */
		Runtime();
		/* デストラクタ */
		~Runtime();

	private:
		/* ウィンドウ */
		static Window* window;
		/* デバイス */
		static Device* device;
	};
}
