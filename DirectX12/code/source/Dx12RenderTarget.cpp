#include "..\include\Dx12RenderTarget.h"
#include "..\include\Dx12SwapChain.h"
#include "..\include\Dx12Heap.h"
#include "..\include\Dx12Resource.h"
#include "..\include\Dx12Runtime.h"

Dx12RenderTarget::Dx12RenderTarget(const Dx12SwapChain* swap) : 
	swap(swap)
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	auto hr = swap->Get()->GetDesc1(&desc);
	heap.CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, desc.BufferCount);
	rsc.resize(desc.BufferCount);

	CreateRenderTarget();
}

Dx12RenderTarget::~Dx12RenderTarget()
{
}

std::uint32_t Dx12RenderTarget::CreateRenderTarget(void)
{
	for (std::uint32_t i = 0; i < rsc.size(); ++i) {
		auto hr = swap->Get()->GetBuffer(i, IID_PPV_ARGS(&rsc[i]));
		if (FAILED(hr)) {
			OutputDebugStringA("失敗：スワップチェインのリソースが取得できませんでした\n");
			return hr;
		}

		Dx12Runtime::RTV(heap.Get(), rsc[i].Get(), i);
	}

	return S_OK;
}

ID3D12Resource1* Dx12RenderTarget::Get(void) const
{
	return rsc[swap->GetBackBufferIndex()].Get();
}

std::uint64_t Dx12RenderTarget::GetCpuAddress(void) const
{
	return heap.GetCpuAddress(swap->GetBackBufferIndex()).ptr;
}
