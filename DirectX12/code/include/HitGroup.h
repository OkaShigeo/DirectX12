#pragma once
#include "SubObject.h"
#include <string>
#include <vector>

namespace Dx12
{
	class HitGroup :
		public SubObject
	{
	public:
		/* �R���X�g���N�^ */
		HitGroup();
		HitGroup(const std::wstring& hit_name, const std::vector<std::wstring>& func_name);
		/* �f�X�g���N�^ */
		~HitGroup();

	public:
		/* �T�u�I�u�W�F�N�g�̒ǉ� */
		void AddSubObj(const std::wstring& hit_name, const std::vector<std::wstring>& func_name);

	private:
		/* �R�s�[�R���X�g���N�^ */
		HitGroup(const HitGroup&) = delete;
	};
}
