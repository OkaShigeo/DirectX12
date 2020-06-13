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
	std::uint32_t CreateResource(const D3D12_RESOURCE_FLAGS& flag, const std::uint64_t& size, D3D12_RESOURCE_STATES& state);
};
