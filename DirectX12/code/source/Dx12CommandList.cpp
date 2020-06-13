#include "..\include\Dx12CommandList.h"
#include "..\include\Dx12CommandAllocator.h"
#include "..\include\Dx12Runtime.h"
#include <wrl.h>

Dx12CommandList::Dx12CommandList(const D3D12_COMMAND_LIST_TYPE& type)
{
	CreateCommandList(type);
}

Dx12CommandList::~Dx12CommandList()
{
}

std::uint32_t Dx12CommandList::CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type)
{
	if (obj != nullptr) {
		OutputDebugStringA("コマンドリストが生成済みです\n");
		return S_FALSE;
	}

	auto hr = Dx12Runtime::GetDevice()->CreateCommandList1(0, type,
		D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：コマンドリストが生成できませんでした\n");
	}

	return hr;
}


std::uint32_t Dx12CommandList::Reset(const Dx12CommandAllocator* allocator)
{
	if (obj == nullptr) {
		OutputDebugStringA("コマンドリストが生成されていません\n");
		return S_FALSE;
	}

	auto hr = obj->Reset(allocator->Get(), nullptr);
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：コマンドリストのリセットができませんでした\n");
	}

	return hr;
}

void Dx12CommandList::SetBarrier(ID3D12Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after)
{
	D3D12_RESOURCE_BARRIER desc{};
	desc.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	desc.Transition.pResource   = rsc;
	desc.Transition.StateAfter  = after;
	desc.Transition.StateBefore = befor;
	desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	obj->ResourceBarrier(1, &desc);
}

void Dx12CommandList::SetRect(const st::Vec2& size)
{
	D3D12_VIEWPORT view{};
	view.Height   = float(size.y);
	view.MaxDepth = 1.0f;
	view.MinDepth = 0.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width    = float(size.x);

	obj->RSSetViewports(1, &view);

	D3D12_RECT rect{};
	rect.bottom = size.y;
	rect.left   = 0;
	rect.right  = size.x;
	rect.top    = 0;

	obj->RSSetScissorRects(1, &rect);
}
