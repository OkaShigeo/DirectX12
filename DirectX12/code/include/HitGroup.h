#pragma once
#include "BaseObject.h"
#include "String.h"

namespace Dx12
{
	class SubObject;

	class HitGroup
	{
	public:
		/*＊ コンストラクタ */
		HitGroup();
		/** コンストラクタ
		 * @param sub サブオブジェクトの追加先
		 * @param hit_name ヒットグループ名
		 * @param closesthit_name 最短ヒット関数名
		 * @param その他ヒット関数名
		 * @param ヒット定義関数名
		 */
		HitGroup(SubObject* sub, const Str::String& hit_group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersect_name = nullptr);
		/** デストラクタ */
		~HitGroup();

	public:
		/** サブオブジェクトの追加 
		 * @param sub サブオブジェクトの追加先
		 * @param hit_name ヒットグループ名
		 * @param closesthit_name 最短ヒット関数名
		 * @param その他ヒット関数名
		 * @param ヒット定義関数名
		 */
		void AddSubObject(SubObject* sub, const Str::String& hit_group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersect_name = nullptr);

	private:
		/* ヒットグループ情報 */
		D3D12_HIT_GROUP_DESC hit{};
	};
}
