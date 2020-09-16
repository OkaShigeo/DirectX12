#include "..\include\Fence.h"
#include "..\include\Runtime.h"

ID3D12Fence1* Dx12::Fence::CreateFence(const std::uint64_t& count)
{
	ID3D12Fence1* fence = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateFence(count, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(hr == S_OK);

	return fence;
}

Dx12::Fence::Fence()
{
}

Dx12::Fence::Fence(const CommandQueue * queue) :
	queue(queue)
{
	obj = CreateFence(count);
}

Dx12::Fence::Fence(ID3D12Fence1* fence)
{
	obj = fence;
}

Dx12::Fence::~Fence()
{
}

void Dx12::Fence::Wait(void)
{
	assert(obj != nullptr);

	auto hr = queue->Get()->Signal(obj, ++count);
	assert(hr == S_OK);

	if (obj->GetCompletedValue() != count) {
		void* handle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
		hr = obj->SetEventOnCompletion(count, handle);
		assert(hr == S_OK);
		assert(handle != nullptr);

		WaitForSingleObjectEx(handle, INFINITE, false);

		assert(CloseHandle(handle) != 0);
	}
}

std::uint64_t Dx12::Fence::GetCount(void) const
{
	return count;
}

void Dx12::Fence::SetCount(const std::uint64_t& count)
{
	this->count = count;
}
