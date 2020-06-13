#pragma once
#include "DxBaseObject.h"

class Dx12Resource :
	public DxBaseObject<ID3D12Resource1>
{
public:
	/* �R���X�g���N�^ */
	Dx12Resource();
	/* �f�X�g���N�^ */
	~Dx12Resource();
	/* ���\�[�X�̐���(�o�b�t�@�[) */
	std::uint32_t CreateResource(const D3D12_RESOURCE_FLAGS& flag, const std::uint64_t& size, D3D12_RESOURCE_STATES& state);
};
