#pragma once
#include "DxBaseObject.h"

class Window;
class Dx12CommandQueue;

class Dx12SwapChain :
	public DxBaseObject<IDXGISwapChain4>
{
public:
	/* コンストラクタ */
	Dx12SwapChain(const Window* window, const Dx12CommandQueue* queue, const std::uint32_t& back_buffer_num = 2);
	/* デストラクタ */
	~Dx12SwapChain();
	/* バッファを切り替える */
	std::uint32_t SwapBuffer(void) const;

public:
	/* 現在のバックバッファインデックスの取得 */
	std::uint32_t GetBackBufferIndex(void) const;

private:
	/* スワップチェインの生成 */
	std::uint32_t CreateSwapChain(const Window* window, const Dx12CommandQueue* queue, const std::uint32_t& back_buffer_num);
};