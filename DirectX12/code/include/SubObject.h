#pragma once
#include "BaseObject.h"
#include <vector>

namespace Dx12
{
	class SubObject
	{
	public:
		/** コンストラクタ
		 * @param capacity 最大格納数
		 */
		SubObject(const std::uint64_t& capacity = 1);
		/** デストラクタ */
		~SubObject();

	public:
		/** サブオブジェクトの追加
		 * @param sub サブオブジェクト
		 * @return ture:追加成功 / false:追加失敗
		 */
		bool AddSubObject(const D3D12_STATE_SUBOBJECT& sub);
		/** サブオブジェクトの追加 
		 * @param type サブオブジェクトタイプ
		 * @param data サブオブジェクトデータ
		 * @return ture:追加成功 / false:追加失敗
		 */
		bool AddSubObject(const D3D12_STATE_SUBOBJECT_TYPE& type, const void* data);

	public:
		/** サブオブジェクトの取得 
		 * @return サブオブジェクト
		 */
		const std::vector<D3D12_STATE_SUBOBJECT>& GetSubObjects(void) const;
		/** 格納サブオブジェクト数の取得
		 * @return 現在の格納数
		 */
		std::uint64_t GetSubObjNum(void) const;
		/** 最大格納サブオブジェクト数の取得
		 * @return 最大格納数
		 */
		std::uint64_t GetMaxSubObjNum(void) const;

	private:
		/* 格納数 */
		std::uint32_t index{ 0 };
		/* サブオブジェクト */
		std::vector<D3D12_STATE_SUBOBJECT>subs;
	};
}
