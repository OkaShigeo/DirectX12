#pragma once
#include "DxBaseObject.h"

class Dx12CommandAllocator :
	public DxBaseObject<ID3D12CommandAllocator>
{
public:
	/* コンストラクタ */
	Dx12CommandAllocator(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE(0));
	/* デストラクタ */
	~Dx12CommandAllocator();
	/* アロケータのリセット */
	std::uint32_t Reset(void) const;

private:
	/* コマンドアロケータの生成 */
	std::uint32_t CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);
};
