#include "..\include\Runtime.h"
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

Window* Dx12::Runtime::window = nullptr;
Dx12::Device* Dx12::Runtime::device = nullptr;
Dx12::CommandQueue* Dx12::Runtime::queue = nullptr;
Dx12::Fence* Dx12::Runtime::fence = nullptr;
Dx12::SwapChain* Dx12::Runtime::swap = nullptr;

Dx12::Runtime::Runtime()
{
}

Dx12::Runtime::~Runtime()
{
}

void Dx12::Runtime::Initialize(const Dx12::Vec2& size, const Dx12::Vec2& pos)
{
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug3>debug = nullptr;
	auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	window = new Window(size, pos);
	device = new Device();
	queue  = new CommandQueue();
	fence  = new Fence(queue);
	swap   = new SwapChain(window, queue);

	window->Show();
}

void Dx12::Runtime::UnInitialize(void)
{
	if (swap != nullptr) {
		delete swap;
	}
	if (fence != nullptr) {
		delete fence;
	}
	if (queue != nullptr) {
		delete queue;
	}
	if (device != nullptr) {
		delete device;
	}
	if (window != nullptr) {
		delete window;
	}
}

Window * Dx12::Runtime::GetWindow(void)
{
	return window;
}

Dx12::Device * Dx12::Runtime::GetDevice(void)
{
	return device;
}
