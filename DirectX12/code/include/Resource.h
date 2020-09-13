#pragma once
#include "BaseObject.h"
#include <vector>

namespace Dx12
{
	class DescriptorHeap;

	class Resource :
		public BaseObject<ID3D12Resource2>
	{
		friend DescriptorHeap;
	public:
		/*�� �f�t�H���g�q�[�v�v���p�e�B�̎擾 
		 * @return �f�t�H���g�̃q�[�v�v���p�e�B
		 */
		static D3D12_HEAP_PROPERTIES GetDefaultProp(void);
		/*�� �A�b�v�f�[�g�q�[�v�v���p�e�B�̎擾 
		 * @return �A�b�v���[�h�̃q�[�v�v���p�e�B
		 */
		static D3D12_HEAP_PROPERTIES GetUploadProp(void);
		/*�� ���[�h�o�b�N�q�[�v�v���p�e�B�̎擾 
		 * @return ���[�h�o�b�N�̃q�[�v�v���p�e�B
		 */
		static D3D12_HEAP_PROPERTIES GetReadbackProp(void);
		/** �o�b�t�@�[���\�[�X�̐��� 
		 * @param state ���\�[�X���
		 * @param prop �q�[�v�v���p�e�B
		 * @param size �o�b�t�@�[�T�C�Y
		 * @param flag ���\�[�X�t���O
		 * @param clear �N���A���
		 * @return �o�b�t�@���\�[�X
		 */
		static ID3D12Resource2* CreateBufferResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr);
		/** �e�N�X�`�����\�[�X�̐��� 
		 * @param state ���\�[�X���
		 * @param prop �q�[�v�v���p�e�B
		 * @param format �e�N�X�`���t�H�[�}�b�g
		 * @param width �e�N�X�`���̉���
		 * @param height �e�N�X�`���̗���
		 * @param flag ���\�[�X�t���O
		 * @param clear �N���A���
		 * @return �e�N�X�`�����\�[�X
		 */
		static ID3D12Resource2* CreateTextureResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format, const std::uint64_t& width, const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag, const D3D12_CLEAR_VALUE* clear = nullptr);

	public:
		/*�� �R���X�g���N�^ */
		Resource();
		/** �R���X�g���N�^
		 * @param state ���\�[�X���
		 * @param prop �q�[�v�v���p�e�B
		 * @param size �o�b�t�@�[�T�C�Y
		 * @param flag ���\�[�X�t���O
		 * @param clear �N���A���
		 */
		Resource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr);
		/** �R���X�g���N�^
		 * @param state ���\�[�X���
		 * @param prop �q�[�v�v���p�e�B
		 * @param format �e�N�X�`���t�H�[�}�b�g
		 * @param width �e�N�X�`���̉���
		 * @param height �e�N�X�`���̗���
		 * @param flag ���\�[�X�t���O
		 * @param clear �N���A���
		 */
		Resource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format, const std::uint64_t& width, const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr);
		/** �R���X�g���N�^
		 * @param rsc ���\�[�X
		 */
		Resource(ID3D12Resource2* rsc);
		/*�� �f�X�g���N�^ */
		~Resource();

	public:
		/** �T�u���\�[�X�̍X�V 
		 * @param data �X�V�f�[�^
		 * @param information �T�u���\�[�X���
		 * @param offset �T�u���\�[�X�ԍ��̃I�t�Z�b�g
		 * @param texture_num �e�N�X�`����(�O�����e�N�X�`���̂�)
		 * @return �A�b�v���[�h���\�[�X
		 */
		ID3D12Resource2* UpdateSubResource(const std::vector<std::uint8_t>& data, std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>& information, const std::uint32_t& offset = 0, const std::uint32_t& texture_num = 1);
		/*�� �m�ۂ����o�b�t�@�̉�� */
		void ReleaseBuffer(void) const;

	public:
		/** �Q�Ƃ��Ă���f�B�X�N���v�^�q�[�v�̎擾 
		 * @return �f�B�X�N���v�^�q�[�v
		 */
		DescriptorHeap* GetHeap(void) const;
		/** ���z�A�h���X�̎擾
		 * @return	���z�A�h���X
		 */
		D3D12_GPU_VIRTUAL_ADDRESS GetAddress(void) const;
		/** CPU�n���h���̎擾 
		 * @return �f�B�X�N���v�^�q�[�v��CPU�n���h��
		 */
		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(void) const;
		/** GPU�n���h���̎擾 
		 * return �f�B�X�N���v�^�q�[�v��GPU�n���h��
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(void) const;
		/** �m�ۂ����o�b�t�@�̎擾 
		 * @return �m�ۂ����o�b�t�@�[�̃|�C���^
		 */
		std::uint8_t* GetBuffer(void) const;
		/** �f�[�^�T�C�Y�̎擾 
		 * @param information �T�u���\�[�X���
		 * @param num �T�u���\�[�X��
		 * @param offset �T�u���\�[�X�ԍ��̃I�t�Z�b�g
		 * @return �f�[�^�T�C�Y
		 */
		std::uint64_t GetSize(D3D12_PLACED_SUBRESOURCE_FOOTPRINT* information = nullptr, const std::uint64_t& num = 1, const std::uint32_t& offset = 0) const;
	
	private:
		/* �Q�Ƃ��Ă���f�B�X�N���v�^�q�[�v */
		DescriptorHeap* heap{ nullptr };
		/* �r���[�A�h���X�̃I�t�Z�b�g�J�E���g*/
		std::uint32_t count{ 0 };
	};
}
