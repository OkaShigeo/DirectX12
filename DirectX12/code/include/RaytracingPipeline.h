#pragma once
#include "BaseSubObject.h"
#include "String.h"

namespace Dx12
{
	class RaytracingPipeline :
		public BaseObject<ID3D12StateObject>, BaseSubObject<D3D12_EXISTING_COLLECTION_DESC>
	{
	public:
		/*＊ ステータスオブジェクトの生成 
		 * @param type 状態オブジェクトタイプ
		 * @param sub サブオブジェクト
		 * @return ステートオブジェクト
		 */
		static ID3D12StateObject* CreateStateObject(const D3D12_STATE_OBJECT_TYPE& type, const SubObject* sub);

	public:
		/** コンストラクタ */
		RaytracingPipeline();
		/** コンストラクタ 
		 * @param type 状態オブジェクトタイプ
		 * @param sub サブオブジェクト
		 */
		RaytracingPipeline(const D3D12_STATE_OBJECT_TYPE& type, const SubObject* sub);
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
		virtual void AddSubObject(SubObject* sub) override;
		/** エントリーシェーダレコードの取得 
		 * @param entry_name エントリーシェーダ名
		 * 
		 */
		void* GetShaderIdentifier(const Str::String& entry_name);

	public:
		/** ステータスオブジェクトプロパティの取得
		 * @return ステータスオブジェクトプロパティ
		 */
		ID3D12StateObjectProperties* GetStateProp(void);

	private:
		/* ステータスオブジェクトプロパティ */
		ID3D12StateObjectProperties* prop;
	};
}
