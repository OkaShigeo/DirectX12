#include "..\include\Runtime.h"

ID3D12Fence1* Dx12::Fence::CreateFence(const std::uint64_t& count)
{
	return Runtime::GetDevice()->CreateFence(count);
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
	count = queue->AddSignal(this);

	if (obj->GetCompletedValue() != count) {
		void* handle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
		assert(handle != nullptr);
		auto hr = obj->SetEventOnCompletion(count, handle);
		assert(hr == S_OK);
		
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
