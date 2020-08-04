#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class CommandAllocator;

	class CommandList :
		public BaseObject<ID3D12GraphicsCommandList5>
	{
	public:
		/* �R���X�g���N�^ */
		CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/* �f�X�g���N�^ */
		~CommandList();

	public:
		/* ���Z�b�g */
		void Reset(const CommandAllocator* allocator) const;
		/* ���� */
		void Close(void) const;

	private:
		/* �R�}���h���X�g�̐��� */
		ID3D12GraphicsCommandList5* CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);
	};
}
