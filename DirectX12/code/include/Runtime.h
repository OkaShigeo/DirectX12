#pragma once
#include "Window.h"
#include "Device.h"
#include "CommandAllocator.h"
#include "CommandList.h"
#include "CommandQueue.h"
#include "Fence.h"
#include "SwapChain.h"
#include "Descriptor.h"
#include "Resource.h"
#include "ShaderCompiler.h"
#include "RootSignature.h"
#include "GraphicsPipeline.h"
#include "ComputePipeline.h"

namespace Dx12
{
	class Runtime
	{
	public:
		/* 初期化 */
		static void Initialize(const Dx12::Vec2& size, const Dx12::Vec2& pos = 0x80000000);
		/* 終了 */
		static void UnInitialize(void);
		/* クリア */
		static void Clear(void);
		/* 実行 */
		static void Execution(const std::vector<CommandList*>& lists = {});
		/* ディスクリプタヒープのセット */
		static void SetDescriptorHeap(const std::vector<Descriptor*>& heap);
		/* グラフィックス用ルートシグネチャのセット */
		static void SetGraphicsRootSignature(const RootSignature* root);
		/* プログラマブル用ルートシグネチャのセット */
		static void SetComputeRootSignature(const RootSignature* root);
		/* グラフィックス用にリソースをセット */
		static void SetGraphicsResource(const Resource* rsc, const std::uint32_t& param_index);
		/* プログラマブル用にリソースをセット */
		static void SetComputeResource(const Resource* rsc, const std::uint32_t& param_index);
		/* グラフィックス用パイプラインのセット */
		static void SetGraphicsPipeline(const GraphicsPipeline* pipe);
		/* プログラマブル用パイプラインのセット */
		static void SetComputePipeline(const ComputePipeline* pipe);
		/* リソースバリアのセット */
		static void SetRscBarrier(const Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
		/* UAVリソースバリアのセット */
		static void SetUavRscBarrier(const Resource* rsc);
		/* 頂点リソースによるプリミティブの描画 */
		static void DrawVertexInstance(const Resource* rsc, const std::uint32_t& vertex_num, const std::uint32_t& instance_num = 1, 
			const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		/* インデックスリソースのよるプリミティブの描画 */
		static void DrawIndexInstance(const Resource* vertex, const std::uint32_t& vertex_num, const Resource* index, const std::uint32_t& index_num, const std::uint32_t& offset = 0,
			const std::uint32_t& instance_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		/* リソースデータのコピー */
		static void CopyResource(const Resource* dst, const Resource* src);

		/* プログラマブルシェーダの実行 */
		static void Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y = 1, const std::uint64_t& thread_z = 1);

	public:
		/* ウィンドウの取得 */
		static Window* GetWindow(void);
		/* デバイスの取得 */
		static Device* GetDevice(void);

	private:
		/* コンストラクタ */
		Runtime();
		/* デストラクタ */
		~Runtime();

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
		static Descriptor* heap;
		/* リソース */
		static std::vector<Resource*>rsc;
	};
}
