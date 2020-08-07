#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class Device :
		public BaseObject<ID3D12Device6>
	{
	public:
		/* コンストラクタ */
		Device();
		Device(ID3D12Device6* device);
		/* デストラクタ */
		~Device();

	private:
		/* デバイスの生成 */
		ID3D12Device6* CreateDevice(void);
	};
}
