#pragma once
#include "DxBaseObject.h"

class Dx12CommandAllocator :
	public DxBaseObject<ID3D12CommandAllocator>
{
public:
	/* �R���X�g���N�^ */
	Dx12CommandAllocator(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE(0));
	/* �f�X�g���N�^ */
	~Dx12CommandAllocator();
	/* �A���P�[�^�̃��Z�b�g */
	std::uint32_t Reset(void) const;

private:
	/* �R�}���h�A���P�[�^�̐��� */
	std::uint32_t CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);
};
