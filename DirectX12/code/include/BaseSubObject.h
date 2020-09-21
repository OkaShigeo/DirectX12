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
		/** コンストラクタ */
		BaseSubObject(){}
		/** デストラクタ */
		virtual ~BaseSubObject() {}

	public:
		/** サブオブジェクトに追加 
		 * @param sub 追加先サブオブジェクト
		 */
		virtual void AddSubObject(SubObject* sub) = 0;

	protected:
		/** 関連付け
		 * @param 関連付けする関数名
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
		/* サブオブジェクト情報 */
		T config;
		/* 関連付け情報 */
		D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association{};
		/* 関連付けする関数名 */
		std::vector<const wchar_t*>asso_name;
	};
}
