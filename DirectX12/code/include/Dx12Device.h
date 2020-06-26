#pragma once
#include "DxBaseObject.h"

struct ID3D12Device5;

class Dx12Device :
	public DxBaseObject<ID3D12Device5>
{
public:
	/* コンストラクタ */
	Dx12Device();
	/* デストラクタ */
	~Dx12Device();

private:
	/* デバイスの生成 */
	std::uint32_t CreateDevice(void);
};
