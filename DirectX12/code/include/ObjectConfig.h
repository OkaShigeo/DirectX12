#pragma once
#include "BaseSubObject.h"

namespace Dx12
{
	class ObjectConfig :
		public BaseSubObject<D3D12_STATE_OBJECT_CONFIG>
	{
	public:
		/** �R���X�g���N�^ */
		ObjectConfig();
		/** �R���X�g���N�^
		 * @param flag �T�u�I�u�W�F�N�g�t���O
		 */
		ObjectConfig(const D3D12_STATE_OBJECT_FLAGS& flag);
		/** �R���X�g���N�^
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param flag �T�u�I�u�W�F�N�g�t���O
		 */
		ObjectConfig(SubObject* sub, const D3D12_STATE_OBJECT_FLAGS& flag);
		/** �f�X�g���N�^ */
		~ObjectConfig();

	public:
		/** �T�u�I�u�W�F�N�g���̃Z�b�g
		 * @param flag �T�u�I�u�W�F�N�g�t���O
		 */
		void SetConfig(const D3D12_STATE_OBJECT_FLAGS& flag);
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param flag �T�u�I�u�W�F�N�g�t���O
		 */
		void AddSubObject(SubObject* sub, const D3D12_STATE_OBJECT_FLAGS& flag);
	};
}
