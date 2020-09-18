#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class SubObject;

	class RaytracingPipeline :
		public BaseObject<ID3D12StateObject>
	{
	public:
		/*�� �X�e�[�^�X�I�u�W�F�N�g�̐��� 
		 * @param sub �T�u�I�u�W�F�N�g
		 * @param type ��ԃI�u�W�F�N�g�^�C�v
		 * @return �X�e�[�g�I�u�W�F�N�g
		 */
		static ID3D12StateObject* CreateStateObject(const SubObject* sub, const D3D12_STATE_OBJECT_TYPE& type);

	public:
		/** �R���X�g���N�^ */
		RaytracingPipeline();
		/** �R���X�g���N�^ 
		 * @param sub �T�u�I�u�W�F�N�g
		 * @param type ��ԃI�u�W�F�N�g�^�C�v
		 */
		RaytracingPipeline(const SubObject* sub, const D3D12_STATE_OBJECT_TYPE& type);
		/** �R���X�g���N�^
		 * @param pipe �p�C�v���C��
		 */
		RaytracingPipeline(ID3D12StateObject* pipe);
		/*�� �f�X�g���N�^ */
		~RaytracingPipeline();

	public:
		/** �T�u�I�u�W�F�N�g�̒ǉ� 
		 * @param sub �T�u�I�u�W�F�N�g
		 */
		void AddSubObject(SubObject* sub);

	private:
		/* �R���N�V������� */
		D3D12_EXISTING_COLLECTION_DESC collection{};
	};
}
