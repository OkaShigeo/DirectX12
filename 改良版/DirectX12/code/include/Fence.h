#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class CommandQueue;

	class Fence :
		public BaseObject<ID3D12Fence1>
	{
	public:
		/* �R���X�g���N�^ */
		Fence(const CommandQueue* queue);
		/* �f�X�g���N�^ */
		~Fence();

	public:
		/* �R�}���h�����̑ҋ@ */
		void Wait(void);

	private:
		/* �t�F���X�̐��� */
		ID3D12Fence1* CreateFence(void);

	private:
		/* �R�}���h�L���[ */
		const CommandQueue* queue;
		/* �t�F���X�J�E���g */
		std::uint64_t count;
	};
}
