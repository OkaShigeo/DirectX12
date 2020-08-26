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
		/* �t�F���X�̐��� */
		static ID3D12Fence1* CreateFence(const std::uint64_t& count = 0);

	public:
		/* �R���X�g���N�^ */
		Fence();
		Fence(const CommandQueue* queue);
		Fence(ID3D12Fence1* fence);
		/* �f�X�g���N�^ */
		~Fence();

	public:
		/* �R�}���h�����̑ҋ@ */
		void Wait(void);

	public:
		/* �t�F���X�J�E���g�̎擾 */
		std::uint64_t GetCount(void) const;

	public:
		/* �t�F���X�J�E���g�̃Z�b�g */
		void SetCount(const std::uint64_t& count);
		
	private:
		/* �R�}���h�L���[ */
		const CommandQueue* queue{ nullptr };
		/* �t�F���X�J�E���g */
		std::uint64_t count{ 0 };
	};
}
