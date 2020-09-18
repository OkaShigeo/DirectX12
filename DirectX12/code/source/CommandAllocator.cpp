#include "..\include\Runtime.h"

ID3D12CommandAllocator* Dx12::CommandAllocator::CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	return Runtime::GetDevice()->CreateCommandAllocator(type);
}

Dx12::CommandAllocator::CommandAllocator(const D3D12_COMMAND_LIST_TYPE & type)
{
	obj = CreateCommandAllocator(type);
}

Dx12::CommandAllocator::CommandAllocator(ID3D12CommandAllocator* allocator)
{
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
