#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class SubObject;

	class RaytracingPipeline :
		public BaseObject<ID3D12StateObject>
	{
	public:
		/*＊ ステータスオブジェクトの生成 
		 * @param sub サブオブジェクト
		 * @param type 状態オブジェクトタイプ
		 * @return ステートオブジェクト
		 */
		static ID3D12StateObject* CreateStateObject(const SubObject* sub, const D3D12_STATE_OBJECT_TYPE& type);

	public:
		/** コンストラクタ */
		RaytracingPipeline();
		/** コンストラクタ 
		 * @param sub サブオブジェクト
		 * @param type 状態オブジェクトタイプ
		 */
		RaytracingPipeline(const SubObject* sub, const D3D12_STATE_OBJECT_TYPE& type);
		/** コンストラクタ
		 * @param pipe パイプライン
		 */
		RaytracingPipeline(ID3D12StateObject* pipe);
		/*＊ デストラクタ */
		~RaytracingPipeline();

	public:
		/** サブオブジェクトの追加 
		 * @param sub サブオブジェクト
		 */
		void AddSubObject(SubObject* sub);

	private:
		/* コレクション情報 */
		D3D12_EXISTING_COLLECTION_DESC collection{};
	};
}
