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

	auto handle = heap->Get()->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += device->Get()->GetDescriptorHandleIncrementSize(heap->Get()->GetDesc().Type) * index;
	device->Get()->CreateRenderTargetView(rsc->Get(), &desc, handle);
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
