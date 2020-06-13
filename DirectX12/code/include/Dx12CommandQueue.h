#pragma once
#include "DxBaseObject.h"
#include <vector>

class Dx12CommandList;
class Dx12Fence;

class Dx12CommandQueue :
	public DxBaseObject<ID3D12CommandQueue>
{
public:
	/* コンストラクタ */
	Dx12CommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE(0));
	/* デストラクタ */
	~Dx12CommandQueue();
	/* コマンドの実行 */
	void ExecuteCommand(const std::vector<Dx12CommandList*>& lists);

private:
	/* コマンドキューの生成 */
	std::uint32_t CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type);


private:
	/* フェンス */
	Dx12Fence* fence;
};
