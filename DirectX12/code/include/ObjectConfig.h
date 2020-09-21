#pragma once
#include "BaseSubObject.h"

namespace Dx12
{
	class ObjectConfig :
		public BaseSubObject<D3D12_STATE_OBJECT_CONFIG>
	{
	public:
		/** コンストラクタ */
		ObjectConfig();
		/** コンストラクタ
		 * @param flag サブオブジェクトフラグ
		 */
		ObjectConfig(const D3D12_STATE_OBJECT_FLAGS& flag);
		/** コンストラクタ
		 * @param sub 追加先サブオブジェクト
		 * @param flag サブオブジェクトフラグ
		 */
		ObjectConfig(SubObject* sub, const D3D12_STATE_OBJECT_FLAGS& flag);
		/** デストラクタ */
		~ObjectConfig();

	public:
		/** サブオブジェクト情報のセット
		 * @param flag サブオブジェクトフラグ
		 */
		void SetConfig(const D3D12_STATE_OBJECT_FLAGS& flag);
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 * @param flag サブオブジェクトフラグ
		 */
		void AddSubObject(SubObject* sub, const D3D12_STATE_OBJECT_FLAGS& flag);
	};
}
