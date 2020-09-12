#include "..\include\Runtime.h"
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

std::vector<D3D12_STATE_SUBOBJECT> Dx12::SubObject::sub;
Window* Dx12::Runtime::window = nullptr;
Dx12::Device* Dx12::Runtime::device = nullptr;
Dx12::CommandAllocator* Dx12::Runtime::allocator = nullptr;
Dx12::CommandList* Dx12::Runtime::list = nullptr;
Dx12::CommandQueue* Dx12::Runtime::queue = nullptr;
Dx12::Fence* Dx12::Runtime::fence = nullptr;
Dx12::SwapChain* Dx12::Runtime::swap = nullptr;
Dx12::DescriptorHeap* Dx12::Runtime::heap = nullptr;
std::vector<Dx12::Resource*> Dx12::Runtime::rsc;

namespace {
	/* クリアカラー */
	float color[] = {
		1.0f, 1.0f, 1.0f, 1.0f
	};
}

void Dx12::Runtime::Initialize(const Math::Vec2& size, const Math::Vec2& pos)
{
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug3>debug = nullptr;
	auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	if (SubObject::GetSubObjList().capacity() == 0) {
		SubObject::Reserve(UCHAR_MAX);
	}

	window    = new Window(size, pos);
	device    = new Device();
	allocator = new CommandAllocator();
	list      = new CommandList();
	queue     = new CommandQueue();
	fence     = new Fence(queue);
	swap      = new SwapChain(window, queue);
	heap      = new DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swap->GetBufferNum(), D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	for (std::uint32_t i = 0; i < heap->Get()->GetDesc().NumDescriptors; ++i) {
		rsc.push_back(new Resource(swap->GetResource(i)));
		heap->CreateRenderTargetView(rsc[i]);
	}

	window->Show();
}

void Dx12::Runtime::UnInitialize(void)
{
	for (auto& i : rsc) {
		if (i != nullptr) {
			delete i;
		}
	}
	if (heap != nullptr) {
		delete heap;
	}
	if (swap != nullptr) {
		delete swap;
	}
	if (fence != nullptr) {
		delete fence;
	}
	if (queue != nullptr) {
		delete queue;
	}
	if (list != nullptr) {
		delete list;
	}
	if (allocator != nullptr) {
		delete allocator;
	}
	if (device != nullptr) {
		delete device;
	}
	if (window != nullptr) {
		delete window;
	}
}

void Dx12::Runtime::Clear(void)
{
	allocator->Reset();
	list->Reset(allocator);

	list->SetViewport(swap->GetSize());
	list->SetScissors(swap->GetSize());

	list->SetRscBarrier(rsc[swap->GetBufferIndex()], D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
	list->ClearRenderTargetView(rsc[swap->GetBufferIndex()], color);
}

void Dx12::Runtime::Execution(const std::vector<CommandList*>& lists)
{
	list->SetRscBarrier(rsc[swap->GetBufferIndex()], D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

	std::vector<CommandList*>tmp = lists;
	tmp.push_back(list);
	for (auto& i : tmp) {
		i->Close();
	}
	queue->Execution(tmp);

	swap->Present();

	fence->Wait();
}

void Dx12::Runtime::SetDescriptorHeap(const std::vector<DescriptorHeap*>& heaps)
{
	list->SetDescriptorHeap(heaps);
}

void Dx12::Runtime::SetGraphicsRootSignature(const RootSignature * root)
{
	list->SetGraphicsRootSignature(root);
}

void Dx12::Runtime::SetComputeRootSignature(const RootSignature * root)
{
	list->SetComputeRootSignature(root);
}

void Dx12::Runtime::SetGraphicsResource(const Resource * rsc, const std::uint32_t & param_index)
{
	list->SetGraphicsResource(rsc, param_index);
}

void Dx12::Runtime::SetComputeResource(const Resource * rsc, const std::uint32_t & param_index)
{
	list->SetComputeResource(rsc, param_index);
}

void Dx12::Runtime::SetGraphicsPipeline(const GraphicsPipeline * pipe)
{
	list->SetGraphicsPipeline(pipe);
}

void Dx12::Runtime::SetComputePipeline(const ComputePipeline * pipe)
{
	list->SetComputePipeline(pipe);
}

void Dx12::Runtime::SetResourceBarrier(const Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after)
{
	list->SetRscBarrier(rsc, befor, after);
}

void Dx12::Runtime::SetUavResourceBarrier(const Resource* rsc)
{
	list->SetUavRscBarrier(rsc);
}

void Dx12::Runtime::DrawVertexInstance(const Resource * rsc, const std::uint32_t & vertex_num, const std::uint32_t & instance_num, const D3D12_PRIMITIVE_TOPOLOGY & topology)
{
	list->DrawVertexInstance(rsc, vertex_num, instance_num, topology);
}

void Dx12::Runtime::DrawIndexInstance(const Resource * vertex, const std::uint32_t & vertex_num, const Resource * index, 
	const std::uint32_t & index_num, const std::uint32_t & offset, const std::uint32_t& instance_num, const D3D12_PRIMITIVE_TOPOLOGY & topology)
{
	list->DrawIndexInstance(vertex, vertex_num, index, index_num, offset, instance_num, topology);
}

void Dx12::Runtime::CopyResource(const Resource* dst, const Resource* src)
{
	list->CopyResource(dst, src);
}

void Dx12::Runtime::CopyBufferRegion(const Resource* dst, const Resource* src, const std::uint64_t& size, const std::uint32_t& dst_offset, const std::uint32_t& src_offset)
{
	list->CopyBufferRegion(dst, src, dst_offset, src_offset, size);
}

void Dx12::Runtime::CopyTextureRegion(const Resource* dst, const Resource* src, const std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>& information, const std::uint32_t& offset)
{
	list->CopyTextureRegion(dst, src, information, offset);
}

void Dx12::Runtime::Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y, const std::uint64_t& thread_z)
{
	list->Dispatch(thread_x, thread_y, thread_z);
}

Window * Dx12::Runtime::GetWindow(void)
{
	return window;
}

Dx12::Device * Dx12::Runtime::GetDevice(void)
{
	return device;
}

Dx12::Runtime::Runtime()
{
}

Dx12::Runtime::~Runtime()
{
}
