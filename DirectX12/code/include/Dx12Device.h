#pragma once
#include "DxBaseObject.h"

struct ID3D12Device5;

class Dx12Device :
	public DxBaseObject<ID3D12Device5>
{
public:
	/* �R���X�g���N�^ */
	Dx12Device();
	/* �f�X�g���N�^ */
	~Dx12Device();

private:
	/* �f�o�C�X�̐��� */
	std::uint32_t CreateDevice(void);
};
