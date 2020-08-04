#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class CommandAllocator;

	class CommandList :
		public BaseObject<ID3D12GraphicsCommandList5>
	{
	public:
		/* コンストラクタ */
		CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/* デストラクタ */
		~CommandList();

	public:
		/* リセット */
		void Reset(const CommandAllocator* allocator) const;
		/* 閉じる */
		void Close(void) const;

	private:
		/* コマンドリストの生成 */
		ID3D12GraphicsCommandList5* CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);
	};
}
