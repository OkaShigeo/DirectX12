#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class Device :
		public BaseObject<ID3D12Device6>
	{
	public:
		/** デバイスの生成 
		 * @return デバイス
		 */
		static ID3D12Device6* CreateDevice(void);

	public:
		/*＊ コンストラクタ */
		Device();
		/** コンストラクタ
		 * @param device デバイス
		 */
		Device(ID3D12Device6* device);
		/** デストラクタ */
		~Device();
	};
}
