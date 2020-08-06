#include "..\include\Fence.h"
#include "..\include\Runtime.h"

Dx12::Fence::Fence(const CommandQueue * queue) : 
	queue(queue)
{
	obj = CreateFence();
}

Dx12::Fence::Fence(ID3D12Fence1 * fence)
{
	obj = fence;
}

Dx12::Fence::~Fence()
{
}

ID3D12Fence1 * Dx12::Fence::CreateFence(void)
{
	ID3D12Fence1* fence = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateFence(count, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(hr == S_OK);

	return fence;
}

void Dx12::Fence::Wait(void)
{
	auto hr = queue->Get()->Signal(obj, ++count);
	assert(hr == S_OK);

	if (obj->GetCompletedValue() != count) {
		void* handle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
		hr = obj->SetEventOnCompletion(count, handle);
		assert(hr == S_OK);

		WaitForSingleObjectEx(handle, INFINITE, false);

		CloseHandle(handle);
	}
}
