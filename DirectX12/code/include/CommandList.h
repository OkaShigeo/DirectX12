#pragma once
#include "BaseObject.h"
#include "Vector.h"
#include <vector>

namespace Dx12
{
	class CommandAllocator;
	class DescriptorHeap;
	class Resource;
	class RootSignature;
	class GraphicsPipeline;
	class ComputePipeline;
	class RaytracingPipeline;
	class VertexBuffer;
	class IndexBuffer;
	class AccelerationStructure;
	class ShaderTable;

	class CommandList :
		public BaseObject<ID3D12GraphicsCommandList5>
	{
	public:
		/** コマンドリストの生成 
		 * @param type コマンドタイプ
		 * @return コマンドリスト
		 */
		static ID3D12GraphicsCommandList5* CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);

	public:
		/** コンストラクタ 
		 * @param type コマンドタイプ
		 */
		CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/** コンストラクタ
		 * @param list コマンドリスト
		 */
		CommandList(ID3D12GraphicsCommandList5* list);
		/*＊ デストラクタ */
		~CommandList();

	public:
		/** コマンドリストの内容をリセット
		 * @param allocator コマンドアロケータ
		 */
		void Reset(const CommandAllocator* allocator) const;
		/** コマンドリストの書き込みを閉じる */
		void Close(void) const;
		/** ビューポートのセット 
		 * @param size ウィンドウサイズ
		 * @param depth 最大深度値
		 */
		void SetViewport(const Math::Vec2& size, const float& depth = 1.0f) const;
		/** シザーのセット
		 * @param size ウィンドウサイズ
		 */
		void SetScissors(const Math::Vec2& size) const;
		/** リソースバリアのセット 
		 * @param rsc リソース
		 * @param befor 遷移前のリソース状態
		 * @param after 遷移後のリソース状態
		 */
		void SetResourceBarrier(const Resource* resource, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after) const;
		/** UAVリソースバリアのセット 
		 * @param リソース
		 */
		void SetUavResourceBarrier(const Resource* resource) const;
		/** レンダーターゲットビューのクリア 
		 * @param rsc リソース
		 * @param color クリアカラー
		 */
		void ClearRenderTargetView(const Resource* resource, const float color[4]) const;
		/** ディスクリプタヒープのセット
		 * @param heap ディスクリプタヒープ
		 */
		void SetDescriptorHeap(const std::vector<DescriptorHeap*>& heaps);
		/** グラフィックス用ルートシグネチャのセット 
		 * @param root ルートシグネチャ
		 */
		void SetGraphicsRootSignature(const RootSignature* root) const;
		/** プログラマブル用ルートシグネチャのセット
		 * @param root ルートシグネチャ
		 */
		void SetComputeRootSignature(const RootSignature* root) const;
		/** グラフィックス用にリソースをセット 
		 * @param rsc リソース
		 * @param param_index ルートシグネチャのパラメータ番号
		 */
		void SetGraphicsResource(const Resource* resource, const std::uint32_t& param_index) const;
		/** プログラマブル用にリソースをセット
		 * @param rsc リソース
		 * @param param_index ルートシグネチャのパラメータ番号
		 */
		void SetComputeResource(const Resource* resource, const std::uint32_t& param_index) const;
		/** グラフィックス用パイプラインのセット 
		 * @param pipe パイプライン
		 */
		void SetGraphicsPipeline(const GraphicsPipeline* pipe) const;
		/** プログラマブル用パイプラインのセット 
		 * @param pipe パイプライン
		 */
		void SetComputePipeline(const ComputePipeline* pipe) const;
		/** レイトレーシング用パイプラインのセット
		 * @param pipe パイプライン
		 */
		void SetRaytracingPipeline(const RaytracingPipeline* pipe) const;
		/** 頂点リソースによるプリミティブの描画 
		 * @param vertex 頂点バッファ
		 * @param instance_num インスタンス数
		 * @param topology プリミティブのトポロジー
		 */
		void DrawVertexInstance(const VertexBuffer* vertex, const std::uint32_t& instance_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		/*＊ インデックスリソースによるプリミティブの描画
		 * @param vertex 頂点バッファ
		 * @param index インデックスバッファ
		 * @param instance_num インスタンス数
		 * @param offset インデックスのオフセット
		 * @param topology プリミティブのトポロジー
		 */
		void DrawIndexInstance(const VertexBuffer* vertex, const IndexBuffer* index, const std::uint32_t& instancex_num = 1, const std::uint32_t& offset = 0, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		/** リソースデータのコピー 
		 * @param dst 送信先のリソース
		 * @param src 送信元のリソース
		 */
		void CopyResource(const Resource* dst, const Resource* src) const;
		/** リソースデータのコピー(バッファ)
		 * @param dst コピー先のリソース
		 * @param src コピー元のリソース
		 * @param size コピーサイズ
		 * @param dst_offset コピー先のデータのオフセット
		 * @param src_offset コピー元のデータのオフセット
		 */
		void CopyBufferRegion(const Resource* dst, const Resource* src, const std::uint64_t& size, const std::uint32_t& dst_offset = 0, const std::uint32_t& src_offset = 0);
		/** リソースデータのコピー(テクスチャ)
		 * @param dst コピー先のリソース
		 * @param src コピー元のリソース
		 * @param information サブリソース情報の配列
		 * @param offset サブリソース番号のオフセット
		 */
		void CopyTextureRegion(const Resource* dst, const Resource* src, const std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>& information, const std::uint32_t& offset = 0);
		/** 加速構造のビルド
		 * @param acceleration 加速構造
		 * @param option ビルドオプション
		 */
		void BuildAccelerationStructure(const AccelerationStructure* acceleration, const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS& option = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE);
		/** プログラマブルシェーダの実行
		 * @param thread_x スレッドグループ X
		 * @param thread_y スレッドグループ Y
		 * @param thread_z スレッドグループ Z
		 */
		void Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y = 1, const std::uint64_t& thread_z = 1) const;
		/**
		 * @param viewport ビューポートサイズ
		 * @param ray_gen レイ生成シェーダテーブル
		 * @param closesthit 最も近いヒットシェーダテーブル
		 * @param miss ミスシェーダテーブル
		 */
		void DispatchRays(const Math::Vec2& viewport, const ShaderTable* ray_gen, const ShaderTable* closesthit, const ShaderTable* miss) const;
	};
}
