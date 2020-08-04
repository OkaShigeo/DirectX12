#include "..\include\CommandList.h"
#include "..\include\Runtime.h"
#include <cassert>

Dx12::CommandList::CommandList(const D3D12_COMMAND_LIST_TYPE & type)
{
	obj = CreateCommandList(type);
}

Dx12::CommandList::~CommandList()
{
}

ID3D12CommandList * Dx12::CommandList::CreateCommandList(const D3D12_COMMAND_LIST_TYPE & type)
{
	ID3D12CommandList* list = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&list));
	assert(hr == S_OK);

	return list;
}
