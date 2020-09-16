#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class RaytracingPipeline :
		public BaseObject<ID3D12StateObject>
	{
	public:
		/*＊ ステータスオブジェクトの生成 
		 * @return ステートオブジェクト
		 */
		static ID3D12StateObject* CreateStateObject(void);

	public:
		/** コンストラクタ */
		RaytracingPipeline();
		/** コンストラクタ
		 * @param pipe パイプライン
		 */
		RaytracingPipeline(ID3D12StateObject* pipe);
		/*＊ デストラクタ */
		~RaytracingPipeline();
	};
}
