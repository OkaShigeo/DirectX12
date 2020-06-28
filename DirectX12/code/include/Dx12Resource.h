#pragma once
#include "DxBaseObject.h"

class Dx12Resource :
	public DxBaseObject<ID3D12Resource1>
{
public:
	/* コンストラクタ */
	Dx12Resource();
	/* デストラクタ */
	~Dx12Resource();
	/* リソースの生成(バッファー) */
	std::uint32_t CreateResource(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_FLAGS& flag, const std::uint64_t& size, 
		const D3D12_RESOURCE_STATES& state);
	/* リソースの生成(テクスチャ) */
	std::uint32_t CreateResource(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_FLAGS& flag, const std::uint64_t& size_x, const std::uint32_t& size_y,
		const D3D12_RESOURCE_STATES& state, const DXGI_FORMAT& format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	/* バッファの確保 */
	std::uint32_t Map(void** buffer, const std::uint64_t& size);
	/* バッファの解放 */
	void Unmap(void);

	/* デフォルトヒーププロパティの取得 */
	static D3D12_HEAP_PROPERTIES GetDefaultProp(void);
	/* アップロードヒーププロパティの取得 */
	static D3D12_HEAP_PROPERTIES GetUploadProp(void);
};
