#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class DescriptorHeap;

	class Resource :
		public BaseObject<ID3D12Resource2>
	{
		friend DescriptorHeap;
	public:
		/* デフォルトヒーププロパティの取得 */
		static D3D12_HEAP_PROPERTIES GetDefaultProp(void);
		/* アップデートヒーププロパティの取得 */
		static D3D12_HEAP_PROPERTIES GetUploadProp(void);
		/* リードバックヒーププロパティの取得 */
		static D3D12_HEAP_PROPERTIES GetReadbackProp(void);
		/* バッファーリソースの生成 */
		static ID3D12Resource2* CreateBufferResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size, const D3D12_RESOURCE_FLAGS& flag);
		/* テクスチャリソースの生成 */
		static ID3D12Resource2* CreateTextureResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format,
			const std::uint64_t& width, const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag, const D3D12_CLEAR_VALUE* clear);

	public:
		/* コンストラクタ */
		Resource();
		Resource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size,
			const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE);
		Resource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format, const std::uint64_t& width,
			const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr);
		Resource(ID3D12Resource2* rsc);
		/* デストラクタ */
		~Resource();

	public:
		/* 確保したバッファの解放 */
		void ReleaseBuffer(void) const;

	public:
		/* 参照しているディスクリプタヒープの取得 */
		DescriptorHeap* GetHeap(void) const;
		/* CPUハンドルの取得 */
		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(void) const;
		/* GPUハンドルの取得 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(void) const;
		/* 確保したバッファの取得 */
		std::uint8_t* GetBuffer(void) const;

	private:
		/* 参照しているディスクリプタヒープ */
		DescriptorHeap* heap{ nullptr };
		/* ビューアドレスのオフセットカウント*/
		std::uint32_t count{ 0 };
	};
}
