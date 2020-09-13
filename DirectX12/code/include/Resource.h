#pragma once
#include "BaseObject.h"
#include <vector>

namespace Dx12
{
	class DescriptorHeap;

	class Resource :
		public BaseObject<ID3D12Resource2>
	{
		friend DescriptorHeap;
	public:
		/*＊ デフォルトヒーププロパティの取得 
		 * @return デフォルトのヒーププロパティ
		 */
		static D3D12_HEAP_PROPERTIES GetDefaultProp(void);
		/*＊ アップデートヒーププロパティの取得 
		 * @return アップロードのヒーププロパティ
		 */
		static D3D12_HEAP_PROPERTIES GetUploadProp(void);
		/*＊ リードバックヒーププロパティの取得 
		 * @return リードバックのヒーププロパティ
		 */
		static D3D12_HEAP_PROPERTIES GetReadbackProp(void);
		/** バッファーリソースの生成 
		 * @param state リソース状態
		 * @param prop ヒーププロパティ
		 * @param size バッファーサイズ
		 * @param flag リソースフラグ
		 * @param clear クリア情報
		 * @return バッファリソース
		 */
		static ID3D12Resource2* CreateBufferResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr);
		/** テクスチャリソースの生成 
		 * @param state リソース状態
		 * @param prop ヒーププロパティ
		 * @param format テクスチャフォーマット
		 * @param width テクスチャの横幅
		 * @param height テクスチャの立幅
		 * @param flag リソースフラグ
		 * @param clear クリア情報
		 * @return テクスチャリソース
		 */
		static ID3D12Resource2* CreateTextureResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format, const std::uint64_t& width, const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag, const D3D12_CLEAR_VALUE* clear = nullptr);

	public:
		/*＊ コンストラクタ */
		Resource();
		/** コンストラクタ
		 * @param state リソース状態
		 * @param prop ヒーププロパティ
		 * @param size バッファーサイズ
		 * @param flag リソースフラグ
		 * @param clear クリア情報
		 */
		Resource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr);
		/** コンストラクタ
		 * @param state リソース状態
		 * @param prop ヒーププロパティ
		 * @param format テクスチャフォーマット
		 * @param width テクスチャの横幅
		 * @param height テクスチャの立幅
		 * @param flag リソースフラグ
		 * @param clear クリア情報
		 */
		Resource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format, const std::uint64_t& width, const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr);
		/** コンストラクタ
		 * @param rsc リソース
		 */
		Resource(ID3D12Resource2* rsc);
		/*＊ デストラクタ */
		~Resource();

	public:
		/** サブリソースの更新 
		 * @param data 更新データ
		 * @param information サブリソース情報
		 * @param offset サブリソース番号のオフセット
		 * @param texture_num テクスチャ数(三次元テクスチャのみ)
		 * @return アップロードリソース
		 */
		ID3D12Resource2* UpdateSubResource(const std::vector<std::uint8_t>& data, std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>& information, const std::uint32_t& offset = 0, const std::uint32_t& texture_num = 1);
		/*＊ 確保したバッファの解放 */
		void ReleaseBuffer(void) const;

	public:
		/** 参照しているディスクリプタヒープの取得 
		 * @return ディスクリプタヒープ
		 */
		DescriptorHeap* GetHeap(void) const;
		/** 仮想アドレスの取得
		 * @return	仮想アドレス
		 */
		D3D12_GPU_VIRTUAL_ADDRESS GetAddress(void) const;
		/** CPUハンドルの取得 
		 * @return ディスクリプタヒープのCPUハンドル
		 */
		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(void) const;
		/** GPUハンドルの取得 
		 * return ディスクリプタヒープのGPUハンドル
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(void) const;
		/** 確保したバッファの取得 
		 * @return 確保したバッファーのポインタ
		 */
		std::uint8_t* GetBuffer(void) const;
		/** データサイズの取得 
		 * @param information サブリソース情報
		 * @param num サブリソース数
		 * @param offset サブリソース番号のオフセット
		 * @return データサイズ
		 */
		std::uint64_t GetSize(D3D12_PLACED_SUBRESOURCE_FOOTPRINT* information = nullptr, const std::uint64_t& num = 1, const std::uint32_t& offset = 0) const;
	
	private:
		/* 参照しているディスクリプタヒープ */
		DescriptorHeap* heap{ nullptr };
		/* ビューアドレスのオフセットカウント*/
		std::uint32_t count{ 0 };
	};
}
