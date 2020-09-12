#pragma once
#include <d3d12.h>
#include <vector>

namespace Dx12
{
	class SubObject
	{
	public:
		/** コンストラクタ */
		SubObject() {}
		/** デストラクタ */
		virtual ~SubObject() {}

	public:
		/** サブオブジェクトの最大数更新
		 @param num メモリ確保数
		 */
		static void Reserve(const std::uint32_t& num)
		{
			if (sub.size() <= 0) {
				sub.reserve(num);
			}
		}

	public:
		/** サブオブジェトの取得 
		 * @param index サブオブジェクト配列のインデックス
		 * @return サブオブジェクト
		 */
		static D3D12_STATE_SUBOBJECT* GetSubObj(const std::uint32_t& index)
		{
			if (index < sub.size()) {
				return &sub[index];
			}

			return nullptr;
		}
		/** サブオブジェクトリストの取得
		 * @return サブオブジェクト配列
		 */
		static std::vector<D3D12_STATE_SUBOBJECT> GetSubObjList(void)
		{
			return sub;
		}
		/** サブオブジェクトリストのポインタの取得
		 * @return サブオブジェクト配列の先頭ポインタ
		 */
		static D3D12_STATE_SUBOBJECT* GetSubObjListPtr(void)
		{
			return sub.data();
		}

	protected:
		/* サブオブジェクトリスト */
		static std::vector<D3D12_STATE_SUBOBJECT>sub;
	};
}
