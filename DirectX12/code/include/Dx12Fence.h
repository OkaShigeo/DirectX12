#pragma once
#include "DxBaseObject.h"

class Dx12CommandQueue;

class Dx12Fence :
	public DxBaseObject<ID3D12Fence1>
{
public:
	/* �R���X�g���N�^ */
	Dx12Fence(const Dx12CommandQueue* queue);
	/* �f�X�g���N�^ */
	~Dx12Fence();
	/* �R�}���h���s�̑ҋ@ */
	std::uint32_t Wait(void);

private:
	/* �t�F���X�̐��� */
	std::uint32_t CreateFence(void);

private:
	/* �R�}���h�L���[ */
	const Dx12CommandQueue* queue;
	/* �J�E���g */
	std::uint64_t count;
};
