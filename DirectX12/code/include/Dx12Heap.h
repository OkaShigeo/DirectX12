#pragma once
#include "DxBaseObject.h"

class Dx12Heap :
	public DxBaseObject<ID3D12DescriptorHeap>
{
public:
	/* �R���X�g���N�^ */
	Dx12Heap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& view_num);
	/* �f�X�g���N�^ */
	~Dx12Heap();

public:
	/* CPU�A�h���X�̎擾 */
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuAddress(const std::uint32_t& index = 0) const;

private:
	/* �q�[�v�̐��� */
	std::uint32_t CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& view_num);
};
