#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class Device :
		public BaseObject<ID3D12Device6>
	{
	public:
		/** �f�o�C�X�̐��� 
		 * @return �f�o�C�X
		 */
		static ID3D12Device6* CreateDevice(void);

	public:
		/*�� �R���X�g���N�^ */
		Device();
		/** �R���X�g���N�^
		 * @param device �f�o�C�X
		 */
		Device(ID3D12Device6* device);
		/** �f�X�g���N�^ */
		~Device();
	};
}
