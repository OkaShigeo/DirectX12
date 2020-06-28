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
	std::uint32_t CreateResource(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_FLAGS& flag, const std::uint64_t& size, 
		const D3D12_RESOURCE_STATES& state);
	/* ���\�[�X�̐���(�e�N�X�`��) */
	std::uint32_t CreateResource(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_FLAGS& flag, const std::uint64_t& size_x, const std::uint32_t& size_y,
		const D3D12_RESOURCE_STATES& state, const DXGI_FORMAT& format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	/* �o�b�t�@�̊m�� */
	std::uint32_t Map(void** buffer, const std::uint64_t& size);
	/* �o�b�t�@�̉�� */
	void Unmap(void);

	/* �f�t�H���g�q�[�v�v���p�e�B�̎擾 */
	static D3D12_HEAP_PROPERTIES GetDefaultProp(void);
	/* �A�b�v���[�h�q�[�v�v���p�e�B�̎擾 */
	static D3D12_HEAP_PROPERTIES GetUploadProp(void);
};
