#include "..\include\CommandAllocator.h"
#include "..\include\Runtime.h"

ID3D12CommandAllocator* Dx12::CommandAllocator::CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	ID3D12CommandAllocator* allocator = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator));
	assert(hr == S_OK);

	return allocator;
}

Dx12::CommandAllocator::CommandAllocator(const D3D12_COMMAND_LIST_TYPE & type)
{
	obj = CreateCommandAllocator(type);
}

Dx12::CommandAllocator::CommandAllocator(ID3D12CommandAllocator * allocator)
{
	Release();
	obj = allocator;
}

Dx12::CommandAllocator::~CommandAllocator()
{
}

void Dx12::CommandAllocator::Reset(void) const
{
	auto hr = obj->Reset();
	assert(hr == S_OK);
}
