#pragma once
#include "BaseObject.h"
#include <vector>

namespace Dx12
{
	class CommandList;

	class CommandQueue :
		public BaseObject<ID3D12CommandQueue>
	{
	public:
		/** �R�}���h�L���[�̐��� 
		 * @param type �R�}���h�^�C�v
		 * @return �R�}���h�L���[
		 */
		static ID3D12CommandQueue* CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);

	public:
		/** �R���X�g���N�^ 
		 * @param type �R�}���h�^�C�v
		 */
		CommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/** �R���X�g���N�^
		 * @param queue �R�}���h�L���[
		 */
		CommandQueue(ID3D12CommandQueue* queue);
		/** �f�X�g���N�^ */
		~CommandQueue();

	public:
		/** �R�}���h���s 
		 * @param lists �R�}���h���X�g�N���X�̔z��
		 */
		void Execution(const std::vector<CommandList*>& lists);
	};
}
