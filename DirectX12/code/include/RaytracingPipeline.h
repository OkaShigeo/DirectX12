#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class RaytracingPipeline :
		public BaseObject<ID3D12StateObject>
	{
	public:
		/*�� �X�e�[�^�X�I�u�W�F�N�g�̐��� 
		 * @return �X�e�[�g�I�u�W�F�N�g
		 */
		static ID3D12StateObject* CreateStateObject(void);

	public:
		/** �R���X�g���N�^ */
		RaytracingPipeline();
		/** �R���X�g���N�^
		 * @param pipe �p�C�v���C��
		 */
		RaytracingPipeline(ID3D12StateObject* pipe);
		/*�� �f�X�g���N�^ */
		~RaytracingPipeline();
	};
}
