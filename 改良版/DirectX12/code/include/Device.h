#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class Device :
		public BaseObject<ID3D12Device6>
	{
	public:
		/* �R���X�g���N�^ */
		Device();
		/* �f�X�g���N�^ */
		~Device();

	private:
		/* �f�o�C�X�̐��� */
		ID3D12Device6* CreateDevice(void);
	};
}
