#pragma once
#include <d3d12.h>
#include <vector>

namespace Dx12
{
	class SubObject
	{
	public:
		/* �R���X�g���N�^ */
		SubObject() {}
		/* �f�X�g���N�^ */
		virtual ~SubObject() {}

	public:
		/* �T�u�I�u�W�F�N�g�̍ő吔�X�V */
		static void Reserve(const std::uint32_t& num)
		{
			if (sub.size() <= 0) {
				sub.reserve(num);
			}
		}

	public:
		/* �T�u�I�u�W�F�g�̎擾 */
		static D3D12_STATE_SUBOBJECT* GetSubObj(const std::uint32_t& index)
		{
			if (index < sub.size()) {
				return &sub[index];
			}

			return nullptr;
		}
		/* �T�u�I�u�W�F�N�g���X�g�̎擾 */
		static std::vector<D3D12_STATE_SUBOBJECT> GetSubObjList(void)
		{
			return sub;
		}
		/* �T�u�I�u�W�F�N�g���X�g�̃|�C���^�̎擾 */
		static D3D12_STATE_SUBOBJECT* GetSubObjListPtr(void)
		{
			return sub.data();
		}

	protected:
		/* �T�u�I�u�W�F�N�g���X�g */
		static std::vector<D3D12_STATE_SUBOBJECT>sub;
	};
}