#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class CommandAllocator :
		public BaseObject<ID3D12CommandAllocator>
	{
	public:
		/* コマンドアロケータの生成 */
		static ID3D12CommandAllocator* CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);

	public:
		/* コンストラクタ */
		CommandAllocator(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		CommandAllocator(ID3D12CommandAllocator* allocator);
		/* デストラクタ */
		~CommandAllocator();

	public:
		/* リセット */
		void Reset(void) const;	
	};
}
