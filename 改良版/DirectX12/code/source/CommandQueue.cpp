#include "..\include\CommandQueue.h"
#include "..\include\Runtime.h"
#include <cassert>

Dx12::CommandQueue::CommandQueue(const D3D12_COMMAND_LIST_TYPE & type)
{
	obj = CreateCommandQueue(type);
}

Dx12::CommandQueue::~CommandQueue()
{
}

ID3D12CommandQueue * Dx12::CommandQueue::CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE & type)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = type;

	ID3D12CommandQueue* queue = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
	assert(hr == S_OK);

	return queue;
}

void Dx12::CommandQueue::Execution(const std::vector<CommandList*>& lists)
{
	std::vector<ID3D12CommandList*>tmp;
	for (auto& i : lists) {
		tmp.push_back(i->Get());
	}

	obj->ExecuteCommandLists(std::uint32_t(tmp.size()), tmp.data());
}
