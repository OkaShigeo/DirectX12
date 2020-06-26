#pragma once
#include "DxBaseObject.h"

class Dx12Heap :
	public DxBaseObject<ID3D12DescriptorHeap>
{
public:
	/* コンストラクタ */
	Dx12Heap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& view_num);
	/* デストラクタ */
	~Dx12Heap();

public:
	/* CPUアドレスの取得 */
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuAddress(const std::uint32_t& index = 0) const;

private:
	/* ヒープの生成 */
	std::uint32_t CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& view_num);
};
