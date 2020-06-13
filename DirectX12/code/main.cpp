#include "include/Dx12Runtime.h"
#include "include/Dx12CommandAllocator.h"
#include "include/Dx12CommandList.h"

int main()
{
	Dx12Runtime::Initialized(st::Vec2(640, 480));
	auto* allocator = new Dx12CommandAllocator();
	auto* list      = new Dx12CommandList();

	float color[] = {
		1.0f, 0.0f, 0.0f, 1.0f
	};
	while (Dx12Runtime::CheckMsg()) {
		allocator->Reset();
		list->Reset(allocator);

		list->SetRect(Dx12Runtime::GetWindowSize());

		list->SetBarrier(Dx12Runtime::GetRenderTarget(),
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
		Dx12Runtime::ClearRenderTarget(list, color);

		list->SetBarrier(Dx12Runtime::GetRenderTarget(),
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

		Dx12Runtime::ExecuteCommand({ list });
	}

	delete list;
	delete allocator;
	Dx12Runtime::UnInitialized();

	return 0;
}
