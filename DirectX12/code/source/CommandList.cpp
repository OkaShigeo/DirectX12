#include "..\include\CommandList.h"
#include "..\include\Runtime.h"

Dx12::CommandList::CommandList(const D3D12_COMMAND_LIST_TYPE & type)
{
	obj = CreateCommandList(type);
}

Dx12::CommandList::CommandList(ID3D12GraphicsCommandList5 * list)
{
	obj = list;
}

Dx12::CommandList::~CommandList()
{
}

ID3D12GraphicsCommandList5 * Dx12::CommandList::CreateCommandList(const D3D12_COMMAND_LIST_TYPE & type)
{
	ID3D12GraphicsCommandList5* list = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&list));
	assert(hr == S_OK);

	return list;
}

void Dx12::CommandList::Reset(const CommandAllocator * allocator) const
{
	auto hr = obj->Reset(allocator->Get(), nullptr);
	assert(hr == S_OK);
}

void Dx12::CommandList::Close(void) const
{
	auto hr = obj->Close();
	assert(hr == S_OK);
}

void Dx12::CommandList::SetViewport(const Vec2 & size, const float& depth) const
{
	D3D12_VIEWPORT view{};
	view.Height   = float(size.y);
	view.MaxDepth = depth;
	view.MinDepth = 0.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width    = float(size.x);

	obj->RSSetViewports(1, &view);
}

void Dx12::CommandList::SetScissors(const Vec2 & size) const
{
	RECT scissors{};
	scissors.bottom = size.y;
	scissors.left   = 0;
	scissors.right  = size.x;
	scissors.top    = 0;

	obj->RSSetScissorRects(1, &scissors);
}

void Dx12::CommandList::SetRscBarrier(const Resource * rsc, const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & after) const
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = rsc->Get();
	barrier.Transition.StateAfter  = after;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	obj->ResourceBarrier(1, &barrier);
}

void Dx12::CommandList::SetUavRscBarrier(const Resource * rsc) const
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags         = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Type          = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.UAV.pResource = rsc->Get();

	obj->ResourceBarrier(1, &barrier);
}

void Dx12::CommandList::ClearRenderTargetView(const Resource * rsc, const float color[4]) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = rsc->GetCpuHandle();
	obj->OMSetRenderTargets(1, &handle, false, nullptr);
	obj->ClearRenderTargetView(handle, color, 0, nullptr);
}

void Dx12::CommandList::SetDescriptorHeap(const std::vector<Descriptor*>& heap)
{
	std::vector<ID3D12DescriptorHeap*>tmp;
	for (auto& i : heap) {
		tmp.push_back(i->Get());
	}

	obj->SetDescriptorHeaps(std::uint32_t(tmp.size()), tmp.data());
}

void Dx12::CommandList::SetGraphicsRootSignature(const RootSignature * root) const
{
	obj->SetGraphicsRootSignature(root->Get());
}

void Dx12::CommandList::SetComputeRootSignature(const RootSignature * root) const
{
	obj->SetComputeRootSignature(root->Get());
}

void Dx12::CommandList::SetGraphicsResource(const Resource * rsc, const std::uint32_t & param_index) const
{
	obj->SetGraphicsRootDescriptorTable(param_index, rsc->GetGpuHandle());
}

void Dx12::CommandList::SetComputeResource(const Resource * rsc, const std::uint32_t & param_index) const
{
	obj->SetComputeRootDescriptorTable(param_index, rsc->GetGpuHandle());
}

void Dx12::CommandList::SetGraphicsPipeline(const GraphicsPipeline * pipe) const
{
	obj->SetPipelineState(pipe->Get());
}

void Dx12::CommandList::SetComputePipeline(const ComputePipeline * pipe) const
{
	obj->SetPipelineState(pipe->Get());
}

void Dx12::CommandList::DrawVertexInstance(const Resource * rsc, const std::uint32_t & vertex_num, const std::uint32_t & instance_num, const D3D12_PRIMITIVE_TOPOLOGY & topology)
{
	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = rsc->Get()->GetGPUVirtualAddress();
	view.SizeInBytes    = uint32_t(rsc->Get()->GetDesc1().Width);
	view.StrideInBytes  = uint32_t(view.SizeInBytes / vertex_num);

	obj->IASetVertexBuffers(0, 1, &view);
	obj->IASetPrimitiveTopology(topology);
	obj->DrawInstanced(uint32_t(vertex_num), instance_num, 0, 0);
}

void Dx12::CommandList::DrawIndexInstance(const Resource * vertex, const std::uint32_t & vertex_num, const Resource * index, const std::uint32_t & index_num,
	const std::uint32_t & offset, const std::uint32_t & instance_num, const D3D12_PRIMITIVE_TOPOLOGY & topology)
{
	/* 頂点バッファのセット */
	{
		D3D12_VERTEX_BUFFER_VIEW view{};
		view.BufferLocation = vertex->Get()->GetGPUVirtualAddress();
		view.SizeInBytes    = uint32_t(vertex->Get()->GetDesc1().Width);
		view.StrideInBytes  = uint32_t(view.SizeInBytes / vertex_num);

		obj->IASetVertexBuffers(0, 1, &view);
	}
	/* インデックスバッファのセット */
	{
		D3D12_INDEX_BUFFER_VIEW view{};
		view.BufferLocation = index->Get()->GetGPUVirtualAddress();
		view.Format         = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		view.SizeInBytes    = std::uint32_t(index->Get()->GetDesc1().Width);

		obj->IASetIndexBuffer(&view);
	}

	obj->IASetPrimitiveTopology(topology);
	obj->DrawIndexedInstanced(index_num, instance_num, offset, 0, 0);
}

void Dx12::CommandList::CopyResource(const Resource* dst, const Resource* src) const
{
	auto type = obj->GetType();
	if (!(type == D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT
		|| type == D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE
		|| type == D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE)) {
		return;
	}

	obj->CopyResource(dst->Get(), src->Get());
}

void Dx12::CommandList::Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y, const std::uint64_t& thread_z) const
{
	obj->Dispatch(std::uint32_t(thread_x), std::uint32_t(thread_y), std::uint32_t(thread_z));
}
