#pragma once
#include "Window.h"
#include "Device.h"
#include "CommandAllocator.h"
#include "CommandList.h"
#include "CommandQueue.h"
#include "Fence.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"
#include "Resource.h"
#include "SubObject.h"
#include "ShaderCompiler.h"
#include "RootSignature.h"
#include "GraphicsPipeline.h"
#include "ComputePipeline.h"
#include "RaytracingPipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "AccelerationStructure.h"
#include "HitGroup.h"
#include "ShaderConfig.h"
#include "PipelineConfig.h"
#include "ObjectConfig.h"
#include "ShaderTable.h"

namespace Dx12
{
	class Runtime
	{
	private:
		/** コンストラクタ */
		Runtime();
		/** デストラクタ */
		~Runtime();
		/** コピーコンストラクタ */
		Runtime(const Runtime&) = delete;

	public:
		/** 初期化済みか確認
		 * @return true:初期化済み / false:未初期化
		 */
		static bool IsInitialized(void);
		/** 初期化 
		 * @param size ウィンドウサイズ
		 * @param pos ウィンドウ座標
		 */
		static void Initialize(const Math::Vec2& size, const Math::Vec2& pos = 0x80000000);
		/*＊ 終了 */
		static void UnInitialize(void);
		/** クリア */
		static void Clear(void);
		/** 実行 
		 * @param lists コマンドリストの配列
		 */
		static void Execution(const std::vector<CommandList*>& lists = {});
		/** ディスクリプタヒープのセット 
		 * @param heaps ディスクリプタヒープの配列
		 */
		static void SetDescriptorHeap(const std::vector<DescriptorHeap*>& heaps);
		/** グラフィックス用ルートシグネチャのセット 
		 * @param root ルートシグネチャ
		 */
		static void SetGraphicsRootSignature(const RootSignature* root);
		/** プログラマブル用ルートシグネチャのセット 
		 * @param root ルートシグネチャ
		 */
		static void SetComputeRootSignature(const RootSignature* root);
		/** グラフィックス用にリソースをセット
		 * @param rsc リソース
		 * @param param_index ルートシグネチャのパラメータ番号
		 */
		static void SetGraphicsResource(const Resource* rsc, const std::uint32_t& param_index);
		/** プログラマブル用にリソースをセット 
		 * @param rsc リソース
		 * @param param_index ルートシグネチャのパラメータ番号
		 */
		static void SetComputeResource(const Resource* rsc, const std::uint32_t& param_index);
		/** グラフィックス用パイプラインのセット 
		 * @param pipe 描画用パイプライン
		 */
		static void SetGraphicsPipeline(const GraphicsPipeline* pipe);
		/*＊ プログラマブル用パイプラインのセット 
		 * @param pipe コンピュート用パイプライン
		 */
		static void SetComputePipeline(const ComputePipeline* pipe);
		/** レイトレーシング用パイプラインのセット 
		 * @param pipe パイプライン
		 */
		static void SetRaytracingPipeline(const RaytracingPipeline* pipe);
		/*＊ リソースバリアのセット 
		 * @param rsc リソース
		 * @param befor 遷移前のリソース状態
		 * @param after 遷移後のリソース状態
		 */
		static void SetResourceBarrier(const Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
		/** UAVリソースバリアのセット 
		 * @param rsc リソース
		 */
		static void SetUavResourceBarrier(const Resource* rsc);
		/** 頂点リソースによるプリミティブの描画 
		 * @param rsc 頂点バッファ
		 * @param instance_num インスタンス数
		 * @param topology トポロジー
		 */
		static void DrawVertexInstance(const VertexBuffer* vertex, const std::uint32_t& instance_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		/** インデックスリソースのよるプリミティブの描画 
		 * @param vertex 頂点バッファ
		 * @param index インデックスバッファ
		 * @param instance_num インスタンス数
		 * @param offset インデックスのオフセット
		 * @param topology トポロジー
		 */
		static void DrawIndexInstance(const VertexBuffer* vertex, const IndexBuffer* index, const std::uint32_t& instance_num = 1, const std::uint32_t& offset = 0, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		/*＊ リソースデータのコピー
		 * @param dst コピー先のリソース
		 * @param src コピー元のリソース
		 */
		static void CopyResource(const Resource* dst, const Resource* src);
		/** リソースデータのコピー(バッファ) 
		 * @param dst コピー先のリソース
		 * @param src コピー元のリソース
		 * @param size コピーサイズ
		 * @param dst_offset コピー先のデータのオフセット
		 * @param src_offset コピー元のデータのオフセット
		 */
		static void CopyBufferRegion(const Resource* dst, const Resource* src, const std::uint64_t& size, const std::uint32_t& dst_offset = 0, const std::uint32_t& src_offset = 0);
		/** リソースデータのコピー(テクスチャ) 
		 * @param dst コピー先のリソース
		 * @param src コピー元のリソース
		 * @param information サブリソース情報の配列
		 * @param offset サブリソース番号のオフセット
		 */
		static void CopyTextureRegion(const Resource* dst, const Resource* src, const std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>& information, const std::uint32_t& offset = 0);
		/** 加速構造のビルド
		 * @param acceleration 加速構造
		 * @param option ビルドオプション
		 */
		static void BuildAccelerationStructure(const AccelerationStructure* acceleraion, const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS& option = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE);
		/*＊ プログラマブルシェーダの実行
		 * @param thread_x スレッドグループX
		 * @param thread_x スレッドグループY
		 * @param thread_x スレッドグループZ
		 */
		static void Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y = 1, const std::uint64_t& thread_z = 1);
		/**
		 * @param ray_gen レイ生成シェーダテーブル
		 * @param closesthit 最も近いヒットシェーダテーブル
		 * @param miss ミスシェーダテーブル
		 */
		static void DispatchRays(const ShaderTable* ray_gen, const ShaderTable* closesthit, const ShaderTable* miss, const Resource* output = nullptr);

	public:
		/** ウィンドウの取得
		 * @return ウィンドウ
		 */
		static Window* GetWindow(void);
		/** デバイスの取得
		 * @return デバイス
		 */
		static Device* GetDevice(void);
		/** ビューポートサイズの取得
		 * @return ビューポートサイズ
		 */
		static Math::Vec2 GetViewportSize(void);

		/*----------オペレータオーバーロード関数----------*/
	private:
		void operator=(const Runtime&) = delete;


	private:
		/* ウィンドウ */
		static Window* window;
		/* デバイス */
		static Device* device;
		/* コマンドアロケータ */
		static CommandAllocator* allocator;
		/* コマンドリスト */
		static CommandList* list;
		/* コマンドキュー */
		static CommandQueue* queue;
		/* フェンス */
		static Fence* fence;
		/* スワップチェイン */
		static SwapChain* swap;
		/* ディスクリプタヒープ */
		static DescriptorHeap* heap;
		/* リソース */
		static std::vector<Resource*>render_resource;
	};
}
