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
		static /* ディスクリプタヒープの生成 */
			ID3D12DescriptorHeap* CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type,
				const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);

	public:
		/* コンストラクタ */
		DescriptorHeap();
		DescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num,
			const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		DescriptorHeap(ID3D12DescriptorHeap* heap);
		/* デストラクタ */
		~DescriptorHeap();

	public:
		/* RTVの生成 */
		bool CreateRenderTargetView(Resource* rsc);
		/* CBVの生成 */
		bool CreateConstantBufferView(Resource* rsc);
		/* SRVの生成 */
		bool CreateShaderResourceView(Resource* rsc);
		/* UAVの生成(バッファー) */
		bool CreateUnorderAccessView(Resource* rsc, const std::uint64_t& element_num);
		/* UAVの生成(テクスチャ) */
		bool CreateUnorderAccessView(Resource* rsc);

	private:
		/* ビュー生成カウント */
		std::uint32_t count{ 0 };
	};
}
