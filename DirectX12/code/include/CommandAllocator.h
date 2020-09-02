#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class CommandAllocator :
		public BaseObject<ID3D12CommandAllocator>
	{
	public:
		/* �R�}���h�A���P�[�^�̐��� */
		static ID3D12CommandAllocator* CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);

	public:
		/* �R���X�g���N�^ */
		CommandAllocator(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		CommandAllocator(ID3D12CommandAllocator* allocator);
		/* �f�X�g���N�^ */
		~CommandAllocator();

	public:
		/* ���Z�b�g */
		void Reset(void) const;	
	};
}