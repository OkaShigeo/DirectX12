#pragma once
#include "BaseObject.h"
#include "String.h"

namespace Dx12
{
	class SubObject;

	template<typename T>
	class BaseSubObject
	{
	public:
		/** �R���X�g���N�^ */
		BaseSubObject(){}
		/** �f�X�g���N�^ */
		virtual ~BaseSubObject() {}

	public:
		/** �T�u�I�u�W�F�N�g�ɒǉ� 
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 */
		virtual void AddSubObject(SubObject* sub) = 0;

	protected:
		/** �֘A�t��
		 * @param �֘A�t������֐���
		 */
		void SetAssociation(const std::vector<Str::String>& func_name)
		{
			asso_name.clear();

			for (auto& i : func_name) {
				asso_name.push_back(i.GetUniCodePtr());
			}

			association.NumExports = std::uint32_t(asso_name.size());
			association.pExports = asso_name.data();
		}

	protected:
		/* �T�u�I�u�W�F�N�g��� */
		T config;
		/* �֘A�t����� */
		D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association{};
		/* �֘A�t������֐��� */
		std::vector<const wchar_t*>asso_name;
	};
}
