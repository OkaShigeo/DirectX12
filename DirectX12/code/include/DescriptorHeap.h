#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class Resource;

	class DescriptorHeap :
		public BaseObject<ID3D12DescriptorHeap>
	{
	public:
		/** ディスクリプタヒープの生成 
		 * @param type ディスクリプタヒープタイプ
		 * @param rsc_num リソース数
		 * @param flag ディスクリプタヒープのフラグ
		 * @return ディスクリプタヒープ
		 */
		static ID3D12DescriptorHeap* CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	public:
		/** コンストラクタ */
		DescriptorHeap();
		/** コンストラクタ 
		 * @param type ディスクリプタヒープタイプ
		 * @param rsc_num リソース数
		 * @param flag ディスクリプタヒープのフラグ
		 */
		DescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		/** コンストラクタ
		 * @param heap ディスクリプタヒープ
		 */
		DescriptorHeap(ID3D12DescriptorHeap* heap);
		/*＊ デストラクタ */
		~DescriptorHeap();

	public:
		/*＊ RTVの生成 
		 * @param resource リソース
		 */
		bool CreateRenderTargetView(Resource* resource);
		/** CBVの生成 
		 * @param resource リソース
		 */
		bool CreateConstantBufferView(Resource* resource);
		/** SRVの生成 
		 * @param resource リソース
		 */
		bool CreateShaderResourceView(Resource* resource);
		/** UAVの生成(バッファ) 
		 * @param resource リソース
		 * @param element_num 要素数
		 */
		bool CreateUnorderAccessView(Resource* resource, const std::uint64_t& element_num);
		/** UAVの生成(テクスチャ) 
		 * @param resource リソース
		 */
		bool CreateUnorderAccessView(Resource* resource);

	public:
		/** ヒープタイプの取得
		 * @return ヒープタイプ
		*/
		D3D12_DESCRIPTOR_HEAP_TYPE GetType(void) const;

	private:
		/* ビュー生成カウント */
		std::uint32_t count{ 0 };
	};
}
