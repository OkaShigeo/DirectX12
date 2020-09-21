#pragma once
#include "BaseSubObject.h"

namespace Dx12
{
	class HitGroup :
		public BaseSubObject<D3D12_HIT_GROUP_DESC>
	{
	public:
		/*＊ コンストラクタ */
		HitGroup();
		/** コンストラクタ
		 * @param group_name ヒットグループ名
		 * @param closesthit_name 最も近いヒット関数名
		 * @param anyhit_name その他のヒット関数名
		 * @param intersection_name ヒット判定関数名
		 */
		HitGroup(const Str::String& group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersection_name = nullptr);
		/** コンストラクタ
		 * @param sub 追加先サブオブジェクト
		 * @param group_name ヒットグループ名
		 * @param closesthit_name 最も近いヒット関数名
		 * @param anyhit_name その他のヒット関数名
		 * @param intersection_name ヒット判定関数名
		 */
		HitGroup(SubObject* sub, const Str::String& group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersection_name = nullptr);
		/** デストラクタ */
		~HitGroup();

	public:
		/** サブオブジェクト情報のセット
		 * @param group_name ヒットグループ名
		 * @param closesthit_name 最も近いヒット関数名
		 * @param anyhit_name その他のヒット関数名
		 * @param intersection_name ヒット判定関数名
		 */
		void SetConfig(const Str::String& group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersection_name = nullptr);
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 * @param group_name ヒットグループ名
		 * @param closesthit_name 最も近いヒット関数名
		 * @param anyhit_name その他のヒット関数名
		 * @param intersection_name ヒット判定関数名
		 */
		void AddSubObject(SubObject* sub, const Str::String& group_name, const char* closetshit_name, const char* anyhit_name = nullptr, const char* intersection_name = nullptr);
	};
}
