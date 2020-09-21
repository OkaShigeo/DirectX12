#pragma once
#include "BaseSubObject.h"

namespace Dx12
{
	class HitGroup :
		public BaseSubObject<D3D12_HIT_GROUP_DESC>
	{
	public:
		/*�� �R���X�g���N�^ */
		HitGroup();
		/** �R���X�g���N�^
		 * @param group_name �q�b�g�O���[�v��
		 * @param closesthit_name �ł��߂��q�b�g�֐���
		 * @param anyhit_name ���̑��̃q�b�g�֐���
		 * @param intersection_name �q�b�g����֐���
		 */
		HitGroup(const Str::String& group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersection_name = nullptr);
		/** �R���X�g���N�^
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param group_name �q�b�g�O���[�v��
		 * @param closesthit_name �ł��߂��q�b�g�֐���
		 * @param anyhit_name ���̑��̃q�b�g�֐���
		 * @param intersection_name �q�b�g����֐���
		 */
		HitGroup(SubObject* sub, const Str::String& group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersection_name = nullptr);
		/** �f�X�g���N�^ */
		~HitGroup();

	public:
		/** �T�u�I�u�W�F�N�g���̃Z�b�g
		 * @param group_name �q�b�g�O���[�v��
		 * @param closesthit_name �ł��߂��q�b�g�֐���
		 * @param anyhit_name ���̑��̃q�b�g�֐���
		 * @param intersection_name �q�b�g����֐���
		 */
		void SetConfig(const Str::String& group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersection_name = nullptr);
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param group_name �q�b�g�O���[�v��
		 * @param closesthit_name �ł��߂��q�b�g�֐���
		 * @param anyhit_name ���̑��̃q�b�g�֐���
		 * @param intersection_name �q�b�g����֐���
		 */
		void AddSubObject(SubObject* sub, const Str::String& group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersection_name = nullptr);
	};
}
