#pragma once
#include "BaseObject.h"
#include <vector>

namespace Dx12
{
	class ShaderCompiler;
	class RootSignature;
	class DescriptorHeap;
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
		ID3D12DescriptorHeap* CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) const;
		/** リソースの生成(バッファ)
		 * @param state リソース状態
		 * @param prop ヒーププロパティ
		 * @param size バッファーサイズ
		 * @param flag リソースフラグ
		 * @param clear クリア情報
		 * @return バッファリソース
		 */
		ID3D12Resource2* CreateBufferResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr) const;
		/** リソースの生成(テクスチャ)
		 * @param state リソース状態
		 * @param prop ヒーププロパティ
		 * @param format テクスチャフォーマット
		 * @param width テクスチャの横幅
		 * @param height テクスチャの立幅
		 * @param flag リソースフラグ
		 * @param clear クリア情報
		 * @return テクスチャリソース
		 */
		ID3D12Resource2* CreateTextureResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format, const std::uint64_t& width, const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr) const;
		/** ルートシグネチャの生成
		 * @param shader シェーダ情報
		 * @return ルートシグネチャ
		 */
		ID3D12RootSignature* CreateRootSignature(const ShaderCompiler* shader) const;
		/** グラフィックスパイプラインの生成
		 * @param input 入力要素の情報
		 * @param root ルートシグネチャ
		 * @param shader シェーダ情報
		 * @param vertex 頂点シェーダ情報
		 * @param pixel ピクセルシェーダ情報
		 * @param geometory ジオメトリーシェーダ情報
		 * @param topology プリミティブのトポロジーの種別
		 * @param depth 深度値セットのフラグ
		 * @return パイプライン
		 */
		ID3D12PipelineState* CreateGraphicsPipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const RootSignature* root, const ShaderCompiler* vertex, const ShaderCompiler* pixel, const ShaderCompiler* geometory = nullptr, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, const bool& depth = false) const;
		/** コンピュートパイプラインの生成
		 * @param root ルートシグネチャ
		 * @param shader シェーダ情報
		 * @return パイプライン
		 */
		ID3D12PipelineState* CreateComputePipeline(const RootSignature* root, const ShaderCompiler* shader) const;
		/** レイトレーシングパイプラインの生成
		 * @return パイプライン
		 */
		ID3D12StateObject* CreateRaytracingPipeline(void) const;
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
		/** ディスクリプタヒープのサイズの取得
		 * @param heap ディスクリプタヒープ
		 * @return ヒープサイズ
		 */
		std::uint64_t GetHeapSize(const DescriptorHeap* heap) const;
		/** サブリソース情報のコピー
		 * @param resource リソース
		 * @param information サブリソース情報のコピー先
		 * @param offset サブリソース番号のオフセット
		 * @return コピーサイズ
		 */
		std::uint64_t CopySubResourceInfo(const Resource* resource, D3D12_PLACED_SUBRESOURCE_FOOTPRINT* information = nullptr, const std::uint32_t& offset = 0) const;
		/** 加速構造のプリビルド情報の取得
		 * @param input ビルド入力情報 
		 * @return プリビルド情報
		 */
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO GetAccelerationStructurePrebuildInfo(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input) const;
	};
}
