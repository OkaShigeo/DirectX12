#include "..\include\SwapChain.h"
#include "..\include\Runtime.h"
#include <wrl.h>

Dx12::SwapChain::SwapChain(const Window * window, const CommandQueue * queue, const std::uint32_t& buffer_num)
{
	obj = CreateSwapChain(window, queue, buffer_num);
}

Dx12::SwapChain::SwapChain(IDXGISwapChain4 * swap)
{
	obj = swap;
}

Dx12::SwapChain::~SwapChain()
{
}

IDXGISwapChain4 * Dx12::SwapChain::CreateSwapChain(const Window * window, const CommandQueue * queue, const std::uint32_t& buffer_num)
{
	Microsoft::WRL::ComPtr<IDXGIFactory7>factory;
	std::uint32_t debug_flag = 0;
#ifdef _DEBUG
	debug_flag = DXGI_CREATE_FACTORY_DEBUG;
#endif
	auto hr = CreateDXGIFactory2(debug_flag, IID_PPV_ARGS(&factory));
	assert(hr == S_OK);

	Dx12::Vec2 size = window->GetSize();

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = buffer_num;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height      = size.y;
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Width       = size.x;

	IDXGISwapChain4* swap = nullptr;
	hr = factory->CreateSwapChainForHwnd(queue->Get(), window->Get(), &desc, nullptr, nullptr, (IDXGISwapChain1**)&swap);
	assert(hr == S_OK);

	return swap;
}

ID3D12Resource2 * Dx12::SwapChain::GetRsc(const std::uint32_t & index)
{
	ID3D12Resource2* rsc = nullptr;
	auto hr = obj->GetBuffer(index, IID_PPV_ARGS(&rsc));
	assert(hr == S_OK);

	return rsc;
}

void Dx12::SwapChain::Present(void) const
{
	obj->Present(0, 0);
}

Dx12::Vec2 Dx12::SwapChain::GetSize(void) const
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	auto hr = obj->GetDesc1(&desc);
	assert(hr == S_OK);

	return Dx12::Vec2(desc.Width, desc.Height);
}

std::uint32_t Dx12::SwapChain::GetBufferNum(void) const
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	auto hr = obj->GetDesc1(&desc);
	assert(hr == S_OK);

	return desc.BufferCount;
}

std::uint32_t Dx12::SwapChain::GetBufferIndex(void) const
{
	return obj->GetCurrentBackBufferIndex();
}
