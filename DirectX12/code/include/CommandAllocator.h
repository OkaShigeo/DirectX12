#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class CommandAllocator :
		public BaseObject<ID3D12CommandAllocator>
	{
	public:
		/** �R�}���h�A���P�[�^�̐��� 
		 * @param type �R�}���h���X�g�̎��
		 * @return �R�}���h�A���P�[�^
		 */
		static ID3D12CommandAllocator* CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);

	public:
		/*�� �R���X�g���N�^
		 * @param type �R�}���h���X�g�̎��
		 */
		CommandAllocator(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/*�� �R���X�g���N�^
		 * @param allocator �R�}���h�A���P�[�^
		 */
		CommandAllocator(ID3D12CommandAllocator* allocator);
		/** �f�X�g���N�^ */
		~CommandAllocator();

	public:
		/** �R�}���h�A���P�[�^�̓��e�����Z�b�g */
		void Reset(void) const;	
	};
}
