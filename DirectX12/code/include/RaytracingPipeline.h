#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class RaytracingPipeline :
		public BaseObject<ID3D12StateObject>
	{
	public:
		/* ステータスオブジェクトの生成 */
		static ID3D12StateObject* CreateStateObject(void);

	public:
		/* コンストラクタ */
		RaytracingPipeline();
		RaytracingPipeline(ID3D12StateObject* pipe);
		/* デストラクタ */
		~RaytracingPipeline();
	};
}
