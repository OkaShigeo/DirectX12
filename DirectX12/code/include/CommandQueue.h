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
		/* �R�}���h�L���[�̐��� */
		static ID3D12CommandQueue* CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type);

	public:
		/* �R���X�g���N�^ */
		CommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		CommandQueue(ID3D12CommandQueue* queue);
		/* �f�X�g���N�^ */
		~CommandQueue();

	public:
		/* �R�}���h���s */
		void Execution(const std::vector<CommandList*>& lists);
	};
}
