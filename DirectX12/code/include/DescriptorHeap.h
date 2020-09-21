#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class Resource;
	class AccelerationStructure;

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
		/*＊ レンダーターゲットの生成 
		 * @param resource リソース
		 * @return true:生成の成功 / false:生成の失敗
		 */
		bool CreateRenderTargetView(Resource* resource);
		/** コンスタントバッファビューの生成 
		 * @param resource リソース
		 * @return true:生成の成功 / false:生成の失敗
		 */
		bool CreateConstantBufferView(Resource* resource);
		/** シェーダリソースビューの生成 
		 * @param resource リソース
		 * @return true:生成の成功 / false:生成の失敗
		 */
		bool CreateShaderResourceView(Resource* resource);
		/** シェーダリソースビューの生成(加速構造)
		 * @param acceleration 加速構造
		 * @return true:生成の成功 / false:生成の失敗
		 */
		bool CreateShaderResourceView(AccelerationStructure* acceleration);
		/** アンオーダーアクセスビューの生成(バッファ) 
		 * @param resource リソース
		 * @param element_num 要素数
		 * @return true:生成の成功 / false:生成の失敗
		 */
		bool CreateUnorderAccessView(Resource* resource, const std::uint64_t& element_num);
		/** アンオーダーアクセスビューの生成(テクスチャ) 
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
