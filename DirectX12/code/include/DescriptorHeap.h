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
		 * @param type ディスクリプタヒープの種別
		 * @param rsc_num リソース数
		 * @param flag ディスクリプタヒープのフラグ
		 * @return ディスクリプタヒープ
		 */
		static ID3D12DescriptorHeap* CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type,
				const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);

	public:
		/** コンストラクタ */
		DescriptorHeap();
		/** コンストラクタ 
		 * @param type ディスクリプタヒープの種別
		 * @param rsc_num リソース数
		 * @param flag ディスクリプタヒープのフラグ
		 */
		DescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num,
			const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		/** コンストラクタ 
		 * @param heap ディスクリプタヒープ
		 */
		DescriptorHeap(ID3D12DescriptorHeap* heap);
		/*＊ デストラクタ */
		~DescriptorHeap();

	public:
		/*＊ RTVの生成 
		 * @param rsc リソース
		 */
		bool CreateRenderTargetView(Resource* rsc);
		/** CBVの生成 
		 * @param rsc リソース
		 */
		bool CreateConstantBufferView(Resource* rsc);
		/** SRVの生成 
		 * @param rsc リソース
		 */
		bool CreateShaderResourceView(Resource* rsc);
		/** UAVの生成(バッファー) 
		 * @param rsc リソース
		 * @param element_num 要素数
		 */
		bool CreateUnorderAccessView(Resource* rsc, const std::uint64_t& element_num);
		/** UAVの生成(テクスチャ) 
		 * @param rsc リソース
		 */
		bool CreateUnorderAccessView(Resource* rsc);

	private:
		/* ビュー生成カウント */
		std::uint32_t count{ 0 };
	};
}
