#include "..\include\Dx12CommandList.h"
#include "..\include\Dx12CommandAllocator.h"
#include "..\include\Dx12Heap.h"
#include "..\include\Dx12Resource.h"
#include "..\include\Dx12RootSignature.h"
#include "..\include\Dx12Pipeline.h"
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

void Dx12CommandList::SetBarrier(ID3D12Resource* rsc)
{
	D3D12_RESOURCE_BARRIER desc{};
	desc.Flags         = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	desc.Type          = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
	desc.UAV.pResource = rsc;

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

void Dx12CommandList::SetRootSignature(const Dx12RootSignature* root)
{
	obj->SetGraphicsRootSignature(root->Get());
}

void Dx12CommandList::SetComputeRootSignature(const Dx12RootSignature* root)
{
	obj->SetComputeRootSignature(root->Get());
}

void Dx12CommandList::SetPipeline(const Dx12Pipeline* pipe)
{
	obj->SetPipelineState(pipe->Get());
}

void Dx12CommandList::DrawVertexInstance(const Dx12Resource* vertex, const std::uint64_t& vertex_num,
	const D3D12_PRIMITIVE_TOPOLOGY& topology, const std::uint32_t& instance_num)
{
	D3D12_VERTEX_BUFFER_VIEW desc{};
	desc.BufferLocation = vertex->Get()->GetGPUVirtualAddress();
	desc.SizeInBytes    = uint32_t(vertex->Get()->GetDesc().Width);
	desc.StrideInBytes  = uint32_t(desc.SizeInBytes / vertex_num);

	obj->IASetVertexBuffers(0, 1, &desc);
	obj->IASetPrimitiveTopology(topology);
	obj->DrawInstanced(uint32_t(vertex_num), instance_num, 0, 0);
}
