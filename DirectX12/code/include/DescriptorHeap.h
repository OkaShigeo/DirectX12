#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class Resource;

	class DescriptorHeap :
		public BaseObject<ID3D12DescriptorHeap>
	{
	public:
		/** �f�B�X�N���v�^�q�[�v�̐��� 
		 * @param type �f�B�X�N���v�^�q�[�v�̎��
		 * @param rsc_num ���\�[�X��
		 * @param flag �f�B�X�N���v�^�q�[�v�̃t���O
		 * @return �f�B�X�N���v�^�q�[�v
		 */
		static ID3D12DescriptorHeap* CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type,
				const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);

	public:
		/** �R���X�g���N�^ */
		DescriptorHeap();
		/** �R���X�g���N�^ 
		 * @param type �f�B�X�N���v�^�q�[�v�̎��
		 * @param rsc_num ���\�[�X��
		 * @param flag �f�B�X�N���v�^�q�[�v�̃t���O
		 */
		DescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num,
			const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		/** �R���X�g���N�^ 
		 * @param heap �f�B�X�N���v�^�q�[�v
		 */
		DescriptorHeap(ID3D12DescriptorHeap* heap);
		/*�� �f�X�g���N�^ */
		~DescriptorHeap();

	public:
		/*�� RTV�̐��� 
		 * @param rsc ���\�[�X
		 */
		bool CreateRenderTargetView(Resource* rsc);
		/** CBV�̐��� 
		 * @param rsc ���\�[�X
		 */
		bool CreateConstantBufferView(Resource* rsc);
		/** SRV�̐��� 
		 * @param rsc ���\�[�X
		 */
		bool CreateShaderResourceView(Resource* rsc);
		/** UAV�̐���(�o�b�t�@�[) 
		 * @param rsc ���\�[�X
		 * @param element_num �v�f��
		 */
		bool CreateUnorderAccessView(Resource* rsc, const std::uint64_t& element_num);
		/** UAV�̐���(�e�N�X�`��) 
		 * @param rsc ���\�[�X
		 */
		bool CreateUnorderAccessView(Resource* rsc);

	private:
		/* �r���[�����J�E���g */
		std::uint32_t count{ 0 };
	};
}
