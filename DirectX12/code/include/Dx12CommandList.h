#pragma once
#include "DxBaseObject.h"
#include "Vector.h"

class Dx12CommandAllocator;

class Dx12CommandList :
	public DxBaseObject<ID3D12GraphicsCommandList4>
{
public:
	/* コンストラクタ */
	Dx12CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE(0));
	/* デストラクタ */
	~Dx12CommandList();
	/* コマンドリストのリセット */
	std::uint32_t Reset(const Dx12CommandAllocator* allocator);
	/* バリアのセット */
	void SetBarrier(ID3D12Resource* rsc,
		const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
	/* 範囲のセット */
	void SetRect(const st::Vec2& size);

private:
	/* コマンドリストの生成 */
	std::uint32_t CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);
};
