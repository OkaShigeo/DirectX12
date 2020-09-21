#pragma once
#include "BaseSubObject.h"
#include "String.h"

namespace Dx12
{
	class RaytracingPipeline :
		public BaseObject<ID3D12StateObject>, BaseSubObject<D3D12_EXISTING_COLLECTION_DESC>
	{
	public:
		/*�� �X�e�[�^�X�I�u�W�F�N�g�̐��� 
		 * @param type ��ԃI�u�W�F�N�g�^�C�v
		 * @param sub �T�u�I�u�W�F�N�g
		 * @return �X�e�[�g�I�u�W�F�N�g
		 */
		static ID3D12StateObject* CreateStateObject(const D3D12_STATE_OBJECT_TYPE& type, const SubObject* sub);

	public:
		/** �R���X�g���N�^ */
		RaytracingPipeline();
		/** �R���X�g���N�^ 
		 * @param type ��ԃI�u�W�F�N�g�^�C�v
		 * @param sub �T�u�I�u�W�F�N�g
		 */
		RaytracingPipeline(const D3D12_STATE_OBJECT_TYPE& type, const SubObject* sub);
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
		virtual void AddSubObject(SubObject* sub) override;
		/** �G���g���[�V�F�[�_���R�[�h�̎擾 
		 * @param entry_name �G���g���[�V�F�[�_��
		 * 
		 */
		void* GetShaderIdentifier(const Str::String& entry_name);

	public:
		/** �X�e�[�^�X�I�u�W�F�N�g�v���p�e�B�̎擾
		 * @return �X�e�[�^�X�I�u�W�F�N�g�v���p�e�B
		 */
		ID3D12StateObjectProperties* GetStateProp(void);

	private:
		/* �X�e�[�^�X�I�u�W�F�N�g�v���p�e�B */
		ID3D12StateObjectProperties* prop;
	};
}
