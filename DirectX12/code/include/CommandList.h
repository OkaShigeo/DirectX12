#pragma once
#include "BaseObject.h"
#include "Vector.h"
#include <vector>

namespace Dx12
{
	class CommandAllocator;
	class Descriptor;
	class Resource;
	class RootSignature;
	class GraphicsPipeline;
	class ComputePipeline;

	class CommandList :
		public BaseObject<ID3D12GraphicsCommandList5>
	{
	public:
		/* コンストラクタ */
		CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		CommandList(ID3D12GraphicsCommandList5* list);
		/* デストラクタ */
		~CommandList();

	public:
		/* リセット */
		void Reset(const CommandAllocator* allocator) const;
		/* 閉じる */
		void Close(void) const;
		/* ビューポートのセット */
		void SetViewport(const Vec2& size, const float& depth = 1.0f) const;
		/* シザーのセット */
		void SetScissors(const Vec2& size) const;
		/* リソースバリアのセット */
		void SetRscBarrier(const Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after) const;
		/* UAVリソースバリアのセット */
		void SetUavRscBarrier(const Resource* rsc) const;
		/* レンダーターゲットビューのクリア */
		void ClearRenderTargetView(const Resource* rsc, const float color[4]) const;
		/* ディスクリプタヒープのセット */
		void SetDescriptorHeap(const std::vector<Descriptor*>& heap);
		/* グラフィックス用ルートシグネチャのセット */
		void SetGraphicsRootSignature(const RootSignature* root) const;
		/* プログラマブル用ルートシグネチャのセット */
		void SetComputeRootSignature(const RootSignature* root) const;
		/* グラフィックス用にリソースをセット */
		void SetGraphicsResource(const Resource* rsc, const std::uint32_t& param_index) const;
		/* プログラマブル用にリソースをセット */
		void SetComputeResource(const Resource* rsc, const std::uint32_t& param_index) const;
		/* グラフィックス用パイプラインのセット */
		void SetGraphicsPipeline(const GraphicsPipeline* pipe) const;
		/* プログラマブル用パイプラインのセット */
		void SetComputePipeline(const ComputePipeline* pipe) const;
		/* 頂点リソースによるプリミティブの描画 */
		void DrawVertexInstance(const Resource* rsc, const std::uint32_t& vertex_num, const std::uint32_t& instance_num = 1,
			const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		/* インデックスリソースによるプリミティブの描画 */
		void DrawIndexInstance(const Resource* vertex, const std::uint32_t& vertex_num, const Resource* index, const std::uint32_t& index_num, const std::uint32_t& offset = 0,
			const std::uint32_t& instancex_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		/* リソースデータのコピー */
		void CopyResource(const Resource* dst, const Resource* src) const;
		/* プログラマブルシェーダの実行 */
		void Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y = 1, const std::uint64_t& thread_z = 1) const;

	private:
		/* コマンドリストの生成 */
		ID3D12GraphicsCommandList5* CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);
	};
}
