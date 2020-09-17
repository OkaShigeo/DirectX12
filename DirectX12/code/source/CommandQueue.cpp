#include "..\include\CommandQueue.h"
#include "..\include\Runtime.h"

ID3D12CommandQueue* Dx12::CommandQueue::CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type)
{
	return Runtime::GetDevice()->CreateCommandQueue(type);
}

Dx12::CommandQueue::CommandQueue(const D3D12_COMMAND_LIST_TYPE & type)
{
	obj = CreateCommandQueue(type);
}

Dx12::CommandQueue::CommandQueue(ID3D12CommandQueue* queue)
{
	obj = queue;
}

Dx12::CommandQueue::~CommandQueue()
{
}

void Dx12::CommandQueue::Execution(const std::vector<CommandList*>& lists)
{
	std::vector<ID3D12CommandList*>tmp;
	for (auto& i : lists) {
		tmp.push_back(i->Get());
	}

	obj->ExecuteCommandLists(std::uint32_t(tmp.size()), tmp.data());
}
