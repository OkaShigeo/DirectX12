#pragma once
#include "Resource.h"
#include <tuple>

namespace Dx12
{
	class AccelerationStructure
	{
	public:
		/*�� �{�g�����x���̐��� 
		 * @param vertex ���_���\�[�X
		 * @param vertex_num ���_��
		 * @param transform_matrix ���_�ϊ��s��̉��z�A�h���X
		 * @param index �C���f�b�N�X���\�[�X
		 * @param index_num �C���f�b�N�X��
		 */
		static std::tuple<ID3D12Resource2*, ID3D12Resource2*> CreateBottomLevel(const std::vector<Resource*>& vertex, const std::vector<std::uint64_t>& vertex_num, const std::uint64_t& transform_matrix_addr = 0, const std::vector<Resource*>& index = {}, const std::vector<std::uint64_t>& index_num = {});

	public:
		/** �R���X�g���N�^ */
		AccelerationStructure();
		/** �R���X�g���N�^ */
		AccelerationStructure(const std::tuple<ID3D12Resource2*, ID3D12Resource2*>& rsc);
		/** �f�X�g���N�^ */
		~AccelerationStructure();

	public:
		/*�� �������̉�� */
		void Release(void);

		/*----------�I�y���[�^�I�[�o�[���[�h�֐�----------*/
	public:
		void operator=(const std::tuple<ID3D12Resource2*, ID3D12Resource2*>& rsc)
		{
			Release();

			scratch = std::get<0>(rsc);
			result  = std::get<1>(rsc);
		}

	private:
		/* �X�N���b�`���\�[�X */
		ID3D12Resource2* scratch{ nullptr };
		/* ���U���g���\�[�X */
		ID3D12Resource2* result{ nullptr };
		/* �C���X�^���X���\�[�X */
		ID3D12Resource2* instance{ nullptr };
	};
}
