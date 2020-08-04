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
		/* �R���X�g���N�^ */
		CommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/* �f�X�g���N�^ */
		~CommandQueue();

	public:
		/* �R�}���h���s */
		void Execution(const std::vector<CommandList*>& lists);

	private:
		/* �R�}���h�L���[�̐��� */
		ID3D12CommandQueue* CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type);
	};
}
