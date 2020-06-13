#include "..\include\Dx12SwapChain.h"
#include "..\include\Window.h"
#include "..\include\Dx12CommandQueue.h"
#include <wrl.h>

Dx12SwapChain::Dx12SwapChain(const Window* window, const Dx12CommandQueue* queue, const std::uint32_t& back_buffer_num)
{
	CreateSwapChain(window, queue, back_buffer_num);
}

Dx12SwapChain::~Dx12SwapChain()
{
}

std::uint32_t Dx12SwapChain::CreateSwapChain(const Window* window, const Dx12CommandQueue* queue, const std::uint32_t& back_buffer_num)
{
	Microsoft::WRL::ComPtr<IDXGIFactory7>factory;
	std::uint32_t debug_flag = 0;
#ifdef _DEBUG
	debug_flag = DXGI_CREATE_FACTORY_DEBUG;
#endif
	auto hr = CreateDXGIFactory2(debug_flag, IID_PPV_ARGS(&factory));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：ファクトリーが生成できませんでした\n");
		return hr;
	}

	auto win_size = window->GetWindowSize();

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = back_buffer_num;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height      = win_size.y;
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Width       = win_size.x;

	hr = factory->CreateSwapChainForHwnd(queue->Get(), window->Get(), &desc, nullptr, nullptr, (IDXGISwapChain1**)&obj);
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：スワップチェインが生成できませんでした\n");
	}

	return hr;
}

std::uint32_t Dx12SwapChain::SwapBuffer(void) const
{
	auto hr = obj->Present(1, 0);
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：バッファの切り替えができませんでした\n");
	}

	return hr;
}

std::uint32_t Dx12SwapChain::GetBackBufferIndex(void) const
{
	return obj->GetCurrentBackBufferIndex();
}
