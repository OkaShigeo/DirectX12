#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class Descriptor;

	class Resource :
		public BaseObject<ID3D12Resource2>
	{
		friend Descriptor;
	public:
		/* �f�t�H���g�q�[�v�v���p�e�B�̎擾 */
		static D3D12_HEAP_PROPERTIES GetDefaultProp(void);
		/* �A�b�v�f�[�g�q�[�v�v���p�e�B�̎擾 */
		static D3D12_HEAP_PROPERTIES GetUploadProp(void);

	public:
		/* �R���X�g���N�^ */
		Resource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size,
			const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE);
		Resource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format, const std::uint64_t& width,
			const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr);
		Resource(ID3D12Resource2* rsc);
		/* �f�X�g���N�^ */
		~Resource();

	public:
		/* �m�ۂ����o�b�t�@�̉�� */
		void ReleaseBuffer(void) const;

	public:
		/* �Q�Ƃ��Ă���f�B�X�N���v�^�q�[�v�̎擾 */
		Descriptor* GetHeap(void) const;
		/* CPU�n���h���̎擾 */
		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(void) const;
		/* GPU�n���h���̎擾 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(void) const;
		/* �m�ۂ����o�b�t�@�̎擾 */
		std::uint8_t* GetBuffer(void) const;

	private:
		/* �o�b�t�@�[���\�[�X�̐��� */
		ID3D12Resource2* CreateBufferResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size, const D3D12_RESOURCE_FLAGS& flag);
		/* �e�N�X�`�����\�[�X�̐��� */
		ID3D12Resource2* CreateTextureResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format,
			const std::uint64_t& width, const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag, const D3D12_CLEAR_VALUE* clear);

	private:
		/* �Q�Ƃ��Ă���f�B�X�N���v�^�q�[�v */
		Descriptor* heap{ nullptr };
		/* �r���[�A�h���X�̃I�t�Z�b�g�J�E���g*/
		std::uint32_t count{ 0 };
	};
}
