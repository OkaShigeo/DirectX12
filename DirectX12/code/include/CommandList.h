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

	class CommandList :
		public BaseObject<ID3D12GraphicsCommandList5>
	{
	public:
		/** コマンドリストの生成 
		 * @param type コマンドリストの種別
		 * @return コマンドリスト
		 */
		static ID3D12GraphicsCommandList5* CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);

	public:
		/** コンストラクタ 
		 * @param type コマンドリストの種別
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
		void SetRscBarrier(const Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after) const;
		/** UAVリソースバリアのセット 
		 * @param リソース
		 */
		void SetUavRscBarrier(const Resource* rsc) const;
		/** レンダーターゲットビューのクリア 
		 * @param rsc リソース
		 * @param color クリアカラー
		 */
		void ClearRenderTargetView(const Resource* rsc, const float color[4]) const;
		/** ディスクリプタヒープのセット
		 * @param heap ディスクリプタヒープ
		 */
		void SetDescriptorHeap(const std::vector<DescriptorHeap*>& heap);
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
		void SetGraphicsResource(const Resource* rsc, const std::uint32_t& param_index) const;
		/** プログラマブル用にリソースをセット
		 * @param rsc リソース
		 * @param param_index ルートシグネチャのパラメータ番号
		 */
		void SetComputeResource(const Resource* rsc, const std::uint32_t& param_index) const;
		/** グラフィックス用パイプラインのセット 
		 * @param pipe パイプライン
		 */
		void SetGraphicsPipeline(const GraphicsPipeline* pipe) const;
		/** プログラマブル用パイプラインのセット 
		 * @param pipe パイプライン
		 */
		void SetComputePipeline(const ComputePipeline* pipe) const;
		/** 頂点リソースによるプリミティブの描画 
		 * @param rsc 頂点リソース
		 * @param vertex_num 頂点数
		 * @param instance_num インスタンス数
		 * @param topology プリミティブのトポロジー
		 */
		void DrawVertexInstance(const Resource* rsc, const std::uint32_t& vertex_num, const std::uint32_t& instance_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		/*＊ インデックスリソースによるプリミティブの描画
		 * @param vertex 頂点リソース
		 * @param vertex_num 頂点数
		 * @param index インデックスリソース
		 * @param index_num インデックス数
		 * @param instance_num インスタンス数
		 * @param topology プリミティブのトポロジー
		 */
		void DrawIndexInstance(const Resource* vertex, const std::uint32_t& vertex_num, const Resource* index, const std::uint32_t& index_num, const std::uint32_t& offset = 0, const std::uint32_t& instancex_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
		/** プログラマブルシェーダの実行
		 * @param thread_x スレッドグループ X
		 * @param thread_y スレッドグループ Y
		 * @param thread_z スレッドグループ Z
		 */
		void Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y = 1, const std::uint64_t& thread_z = 1) const;
	};
}
