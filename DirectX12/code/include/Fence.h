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
		/*�� �t�F���X�̐���
		 * @param count �t�F���X�J�E���g
		 * @return �t�F���X
		 */
		static ID3D12Fence1* CreateFence(const std::uint64_t& count = 0);

	public:
		/** �R���X�g���N�^ */
		Fence();
		/** �R���X�g���N�^
		 * @param queue �R�}���h�L���[
		 */
		Fence(const CommandQueue* queue);
		/** �R���X�g���N�^ 
		 * @param fence �t�F���X
		 */
		Fence(ID3D12Fence1* fence);
		/** �f�X�g���N�^ */
		~Fence();

	public:
		/** �R�}���h�����̑ҋ@ */
		void Wait(void);

	public:
		/** �t�F���X�J�E���g�̎擾 
		 * @return �t�F���X�J�E���g
		 */
		std::uint64_t GetCount(void) const;

	public:
		/** �t�F���X�J�E���g�̃Z�b�g 
		 * @param count �t�F���X�J�E���g
		 */
		void SetCount(const std::uint64_t& count);
		
	private:
		/* �R�}���h�L���[ */
		const CommandQueue* queue{ nullptr };
		/* �t�F���X�J�E���g */
		std::uint64_t count{ 0 };
	};
}
