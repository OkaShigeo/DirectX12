#pragma once
#include "DxBaseObject.h"

class Dx12Heap :
	public DxBaseObject<ID3D12DescriptorHeap>
{
public:
	/* コンストラクタ */
	Dx12Heap();
	/* デストラクタ */
	~Dx12Heap();

public:
	/* ヒープの生成 */
	std::uint32_t CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& view_num);
	/* CPUアドレスの取得 */
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuAddress(const std::uint32_t& index = 0) const;
	/* GPUアドレスの取得 */
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuAddress(const std::uint32_t& index = 0) const;
};
