#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class ShaderCompiler;
	class RootSignature;
	class Resource;

	class Device :
		public BaseObject<ID3D12Device6>
	{
	public:
		/** デバイスの生成 
		 * @return デバイス
		 */
		static ID3D12Device6* CreateDevice(void);

	public:
		/*＊ コンストラクタ */
		Device();
		/** コンストラクタ
		 * @param device デバイス
		 */
		Device(ID3D12Device6* device);
		/** デストラクタ */
		~Device();

	public:
		/** コマンドアロケータの生成
		 * @param type コマンドタイプ
		 * @return コマンドアロケータ
		 */
		ID3D12CommandAllocator* CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT) const;
		/** コマンドリストの生成
		 * @param type コマンドタイプ
		 * @return コマンドリスト
		 */
		ID3D12GraphicsCommandList5* CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT) const;
		/** コマンドキューの生成
		 * @param type コマンドタイプ
		 * @return コマンドキュー
		 */
		ID3D12CommandQueue* CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT) const;
		/** フェンスの生成
		 * @param count フェンスカウント
		 * @return フェンス
		 */
		ID3D12Fence1* CreateFence(const std::uint64_t& count = 0) const;
		/** ディスクリプタヒープの生成
		 * @param type ディスクリプタヒープタイプ
		 * @param rsc_num リソース数
		 * @param flag ディスクリプタヒープのフラグ
		 * @return ディスクリプタヒープ
		 */
		ID3D12DescriptorHeap* CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		/** コンピュートパイプラインの生成
		 * @param root ルートシグネチャ
		 * @param shader シェーダ情報
		 * @return パイプライン
		 */
		ID3D12PipelineState* CreateComputePipeline(const RootSignature* root, const ShaderCompiler* shader);
		/** レンダーターゲットの生成
		 * @param resource リソース
		 */
		void CreateRenderTarget(const Resource* resource) const;
		/** コンスタントバッファビューの生成
		 * @param resource リソース
		 */
		void CreateConstantBufferView(const Resource* resource) const;
		/** シェーダリソースビューの生成
		 * @param resource リソース
		 */
		void CreateShaderResourceView(const Resource* resource) const;
		/** アンオーダーアクセスビューの生成(バッファ)
		 * @param resource リソース
		 * @param element_num 要素数
		 */
		void CreateUnorderAccessView(Resource* resource, const std::uint64_t& element_num) const;
		/** アンオーダーアクセスビューの生成(テクスチャ)
		 * @param resource リソース
		 */
		void CreateUnorderAccessView(Resource* resource) const;
		/** 加速構造のプリビルド情報の取得
		 * @param input ビルド入力情報 
		 * @return プリビルド情報
		 */
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO GetAccelerationStructurePrebuildInfo(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input) const;
	};
}
