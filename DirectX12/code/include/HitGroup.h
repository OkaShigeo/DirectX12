#pragma once
#include "BaseObject.h"
#include "String.h"

namespace Dx12
{
	class SubObject;

	class HitGroup
	{
	public:
		/*�� �R���X�g���N�^ */
		HitGroup();
		/** �R���X�g���N�^
		 * @param sub �T�u�I�u�W�F�N�g�̒ǉ���
		 * @param hit_name �q�b�g�O���[�v��
		 * @param closesthit_name �ŒZ�q�b�g�֐���
		 * @param ���̑��q�b�g�֐���
		 * @param �q�b�g��`�֐���
		 */
		HitGroup(SubObject* sub, const Str::String& hit_group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersect_name = nullptr);
		/** �f�X�g���N�^ */
		~HitGroup();

	public:
		/** �T�u�I�u�W�F�N�g�̒ǉ� 
		 * @param sub �T�u�I�u�W�F�N�g�̒ǉ���
		 * @param hit_name �q�b�g�O���[�v��
		 * @param closesthit_name �ŒZ�q�b�g�֐���
		 * @param ���̑��q�b�g�֐���
		 * @param �q�b�g��`�֐���
		 */
		void AddSubObject(SubObject* sub, const Str::String& hit_group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersect_name = nullptr);

	private:
		/* �q�b�g�O���[�v��� */
		D3D12_HIT_GROUP_DESC hit{};
	};
}
