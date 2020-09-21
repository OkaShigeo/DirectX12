#pragma once
#include "BaseObject.h"
#include <vector>

namespace Dx12
{
	class SubObject
	{
	public:
		/** �R���X�g���N�^
		 * @param capacity �ő�i�[��
		 */
		SubObject(const std::uint64_t& capacity = 1);
		/** �f�X�g���N�^ */
		~SubObject();

	public:
		/** �T�u�I�u�W�F�N�g�̒ǉ�
		 * @param sub �T�u�I�u�W�F�N�g
		 * @return ture:�ǉ����� / false:�ǉ����s
		 */
		bool AddSubObject(const D3D12_STATE_SUBOBJECT& sub);
		/** �T�u�I�u�W�F�N�g�̒ǉ� 
		 * @param type �T�u�I�u�W�F�N�g�^�C�v
		 * @param data �T�u�I�u�W�F�N�g�f�[�^
		 * @return ture:�ǉ����� / false:�ǉ����s
		 */
		bool AddSubObject(const D3D12_STATE_SUBOBJECT_TYPE& type, const void* data);

	public:
		/** �T�u�I�u�W�F�N�g�̎擾 
		 * @return �T�u�I�u�W�F�N�g
		 */
		const std::vector<D3D12_STATE_SUBOBJECT>& GetSubObjects(void) const;
		/** �i�[�T�u�I�u�W�F�N�g���̎擾
		 * @return ���݂̊i�[��
		 */
		std::uint64_t GetSubObjNum(void) const;
		/** �ő�i�[�T�u�I�u�W�F�N�g���̎擾
		 * @return �ő�i�[��
		 */
		std::uint64_t GetMaxSubObjNum(void) const;

	private:
		/* �i�[�� */
		std::uint32_t index{ 0 };
		/* �T�u�I�u�W�F�N�g */
		std::vector<D3D12_STATE_SUBOBJECT>subs;
	};
}
