#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class Resource;
	class AccelerationStructure;

	class DescriptorHeap :
		public BaseObject<ID3D12DescriptorHeap>
	{
	public:
		/** �f�B�X�N���v�^�q�[�v�̐��� 
		 * @param type �f�B�X�N���v�^�q�[�v�^�C�v
		 * @param rsc_num ���\�[�X��
		 * @param flag �f�B�X�N���v�^�q�[�v�̃t���O
		 * @return �f�B�X�N���v�^�q�[�v
		 */
		static ID3D12DescriptorHeap* CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	public:
		/** �R���X�g���N�^ */
		DescriptorHeap();
		/** �R���X�g���N�^ 
		 * @param type �f�B�X�N���v�^�q�[�v�^�C�v
		 * @param rsc_num ���\�[�X��
		 * @param flag �f�B�X�N���v�^�q�[�v�̃t���O
		 */
		DescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		/** �R���X�g���N�^
		 * @param heap �f�B�X�N���v�^�q�[�v
		 */
		DescriptorHeap(ID3D12DescriptorHeap* heap);
		/*�� �f�X�g���N�^ */
		~DescriptorHeap();

	public:
		/*�� �����_�[�^�[�Q�b�g�̐��� 
		 * @param resource ���\�[�X
		 * @return true:�����̐��� / false:�����̎��s
		 */
		bool CreateRenderTargetView(Resource* resource);
		/** �R���X�^���g�o�b�t�@�r���[�̐��� 
		 * @param resource ���\�[�X
		 * @return true:�����̐��� / false:�����̎��s
		 */
		bool CreateConstantBufferView(Resource* resource);
		/** �V�F�[�_���\�[�X�r���[�̐��� 
		 * @param resource ���\�[�X
		 * @return true:�����̐��� / false:�����̎��s
		 */
		bool CreateShaderResourceView(Resource* resource);
		/** �V�F�[�_���\�[�X�r���[�̐���(�����\��)
		 * @param acceleration �����\��
		 * @return true:�����̐��� / false:�����̎��s
		 */
		bool CreateShaderResourceView(AccelerationStructure* acceleration);
		/** �A���I�[�_�[�A�N�Z�X�r���[�̐���(�o�b�t�@) 
		 * @param resource ���\�[�X
		 * @param element_num �v�f��
		 * @return true:�����̐��� / false:�����̎��s
		 */
		bool CreateUnorderAccessView(Resource* resource, const std::uint64_t& element_num);
		/** �A���I�[�_�[�A�N�Z�X�r���[�̐���(�e�N�X�`��) 
		 * @param resource ���\�[�X
		 */
		bool CreateUnorderAccessView(Resource* resource);

	public:
		/** �q�[�v�^�C�v�̎擾
		 * @return �q�[�v�^�C�v
		*/
		D3D12_DESCRIPTOR_HEAP_TYPE GetType(void) const;

	private:
		/* �r���[�����J�E���g */
		std::uint32_t count{ 0 };
	};
}
