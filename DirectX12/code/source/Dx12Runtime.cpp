#include "..\include\Dx12Runtime.h"
#include "..\include\Window.h"
#include "..\include\Dx12Device.h"
#include "..\include\Dx12CommandList.h"
#include "..\include\Dx12CommandQueue.h"
#include "..\include\Dx12SwapChain.h"
#include "..\include\Dx12Heap.h"
#include "..\include\Dx12Resource.h"
#include "..\include\Dx12RenderTarget.h"
#include <wrl.h>
#include <memory>
#include <Windows.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

Window* Dx12Runtime::window = nullptr;
Dx12Device* Dx12Runtime::device = nullptr;
Dx12CommandQueue* Dx12Runtime::queue = nullptr;
Dx12SwapChain* Dx12Runtime::swap = nullptr;
Dx12RenderTarget* Dx12Runtime::render = nullptr;

Dx12Runtime::Dx12Runtime()
{
}

Dx12Runtime::~Dx12Runtime()
{
}

void Dx12Runtime::Initialized(const st::Vec2& window_size)
{
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug3>debug;
	auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	window = new Window(window_size);
	device = new Dx12Device();
	queue  = new Dx12CommandQueue();
	swap   = new Dx12SwapChain(window, queue);
	render = new Dx12RenderTarget(swap);
	 
	window->ShowWndow();
}

void Dx12Runtime::UnInitialized(void)
{
	delete render;
	delete swap;
	delete queue;
	delete device;
	delete window;
}

bool Dx12Runtime::CheckMsg(void)
{
	static MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		switch (msg.message)
		{
		case WM_CREATE:
			break;
		case WM_QUIT:
			return false;
		default:
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void Dx12Runtime::RTV(const Dx12Heap* heap, const Dx12Resource* rsc, const std::uint32_t& index)
{
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format        = rsc->Get()->GetDesc().Format;
	desc.Texture2D     = {};
	desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;

	device->Get()->CreateRenderTargetView(rsc->Get(), &desc, heap->GetCpuAddress(index));
}

void Dx12Runtime::RTV(ID3D12DescriptorHeap* heap, ID3D12Resource1* rsc, const std::uint32_t& index)
{
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format        = rsc->GetDesc().Format;
	desc.Texture2D     = {};
	desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += device->Get()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	device->Get()->CreateRenderTargetView(rsc, &desc, handle);
}

void Dx12Runtime::CBV(const Dx12Heap* heap, const Dx12Resource* rsc, const std::uint32_t& index)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = rsc->Get()->GetGPUVirtualAddress();
	desc.SizeInBytes    = std::uint32_t(rsc->Get()->GetDesc().Width);

	device->Get()->CreateConstantBufferView(&desc, heap->GetCpuAddress(index));
}

void Dx12Runtime::CBV(ID3D12DescriptorHeap* heap, ID3D12Resource1* rsc, const std::uint32_t& index)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = rsc->GetGPUVirtualAddress();
	desc.SizeInBytes    = std::uint32_t(rsc->GetDesc().Width);

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += device->Get()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	device->Get()->CreateConstantBufferView(&desc, handle);
}

void Dx12Runtime::SRV(const Dx12Heap* heap, const Dx12Resource* rsc, const std::uint32_t& index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format                  = rsc->Get()->GetDesc().Format;
	desc.ViewDimension           = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels     = rsc->Get()->GetDesc().MipLevels;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	device->Get()->CreateShaderResourceView(rsc->Get(), &desc, heap->GetCpuAddress(index));
}

void Dx12Runtime::SRV(ID3D12DescriptorHeap* heap, ID3D12Resource1* rsc, const std::uint32_t& index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format                  = rsc->GetDesc().Format;
	desc.ViewDimension           = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels     = rsc->GetDesc().MipLevels;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += device->Get()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	device->Get()->CreateShaderResourceView(rsc, &desc, handle);
}

void Dx12Runtime::UAV(const Dx12Heap* heap, const Dx12Resource* rsc, 
	const std::uint64_t& stride, const std::uint64_t& num, const std::uint32_t& index)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.Buffer.CounterOffsetInBytes = 0;
	desc.Buffer.FirstElement         = 0;
	desc.Buffer.Flags                = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
	desc.Buffer.NumElements          = std::uint32_t(num);
	desc.Buffer.StructureByteStride  = std::uint32_t(stride);
	desc.Format                      = rsc->Get()->GetDesc().Format;
	desc.ViewDimension               = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;

	device->Get()->CreateUnorderedAccessView(rsc->Get(), nullptr, &desc, heap->GetCpuAddress(index));
}

void Dx12Runtime::UAV(ID3D12DescriptorHeap* heap, ID3D12Resource1* rsc, 
	const std::uint64_t& stride, const std::uint64_t& num, const std::uint32_t& index)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.Buffer.CounterOffsetInBytes = 0;
	desc.Buffer.FirstElement         = 0;
	desc.Buffer.Flags                = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
	desc.Buffer.NumElements          = std::uint32_t(num);
	desc.Buffer.StructureByteStride  = std::uint32_t(stride);
	desc.Format                      = rsc->GetDesc().Format;
	desc.ViewDimension               = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += device->Get()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	device->Get()->CreateUnorderedAccessView(rsc, nullptr, &desc, handle);
}

void Dx12Runtime::UAV(const Dx12Heap* heap, const Dx12Resource* rsc, const std::uint32_t& index)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.Format        = rsc->Get()->GetDesc().Format;
	desc.Texture2D     = {};
	desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;

	device->Get()->CreateUnorderedAccessView(rsc->Get(), nullptr, &desc, heap->GetCpuAddress(index));
}

void Dx12Runtime::UAV(ID3D12DescriptorHeap* heap, ID3D12Resource1* rsc, const std::uint32_t& index)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.Format        = rsc->GetDesc().Format;
	desc.Texture2D     = {};
	desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += device->Get()->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	device->Get()->CreateUnorderedAccessView(rsc, nullptr, &desc, handle);
}

void Dx12Runtime::ClearRenderTarget(const Dx12CommandList* list, const float color[4])
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = { render->GetCpuAddress() };

	list->Get()->OMSetRenderTargets(1, &handle, false, nullptr);
	list->Get()->ClearRenderTargetView(handle, color, 0, nullptr);
}

void Dx12Runtime::ExecuteCommand(const std::vector<Dx12CommandList*>& list)
{
	queue->ExecuteCommand(list);
	swap->SwapBuffer();
}

st::Vec2 Dx12Runtime::GetWindowSize(void)
{
	return window->GetWindowSize();
}

ID3D12Device5* Dx12Runtime::GetDevice(void)
{
	return device->Get();
}

ID3D12Resource1* Dx12Runtime::GetRenderTarget(void)
{
	return render->Get();
}
