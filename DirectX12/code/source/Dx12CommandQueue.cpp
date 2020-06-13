#include "..\include\Dx12CommandQueue.h"
#include "..\include\Dx12CommandList.h"
#include "..\include\Dx12Fence.h"
#include "..\include\Dx12Runtime.h"

Dx12CommandQueue::Dx12CommandQueue(const D3D12_COMMAND_LIST_TYPE& type) : 
	fence(nullptr)
{
	CreateCommandQueue(type);
	fence = new Dx12Fence(this);
}

Dx12CommandQueue::~Dx12CommandQueue()
{
	delete fence;
}

std::uint32_t Dx12CommandQueue::CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type)
{
	if (obj != nullptr) {
		OutputDebugStringA("コマンドキューが生成済みです\n");
		return S_FALSE;
	}

	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = type;

	auto hr = Dx12Runtime::GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：コマンドキューが生成できませんでした\n");
	}

	return hr;
}

void Dx12CommandQueue::ExecuteCommand(const std::vector<Dx12CommandList*>& lists)
{
	std::vector<ID3D12CommandList*>list;
	for (auto& i : lists) {
		auto hr = i->Get()->Close();
		if (FAILED(hr)) {
			OutputDebugStringA("コマンドリストがクローズできませんでした\n");
			return;
		}
		list.push_back(i->Get());
	}

	obj->ExecuteCommandLists(std::uint32_t(list.size()), list.data());

	fence->Wait();
}
