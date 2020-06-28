#pragma once
#include "DxBaseObject.h"
#include "Vector.h"
#include <vector>

class Dx12CommandAllocator;
class Dx12Heap;
class Dx12Resource;
class Dx12RootSignature;
class Dx12Pipeline;

class Dx12CommandList :
	public DxBaseObject<ID3D12GraphicsCommandList4>
{
public:
	/* コンストラクタ */
	Dx12CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE(0));
	/* デストラクタ */
	~Dx12CommandList();

public:
	/* コマンドリストのリセット */
	std::uint32_t Reset(const Dx12CommandAllocator* allocator);
	/* バリアのセット */
	void SetBarrier(ID3D12Resource* rsc,
		const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
	/* UAVバリアのセット */
	void SetBarrier(ID3D12Resource* rsc);
	/* 描画範囲のセット */
	void SetRect(const st::Vec2& size);
	/* ルートシグネチャのセット */
	void SetRootSignature(const Dx12RootSignature* root);
	/* コンピュート用のルートシグネチャのセット */
	void SetComputeRootSignature(const Dx12RootSignature* root);
	/* パイプラインのセット */
	void SetPipeline(const Dx12Pipeline* pipe);
	/* 頂点描画 */
	void DrawVertexInstance(const Dx12Resource* vertex, const std::uint64_t& vertex_num, 
		const D3D12_PRIMITIVE_TOPOLOGY& topology, const std::uint32_t& instance_num = 1);

private:
	/* コマンドリストの生成 */
	std::uint32_t CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);
};
