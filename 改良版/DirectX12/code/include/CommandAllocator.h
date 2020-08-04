#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class CommandAllocator :
		public BaseObject<ID3D12CommandAllocator>
	{
	public:
		/* コンストラクタ */
		CommandAllocator(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/* デストラクタ */
		~CommandAllocator();

	public:
		/* リセット */
		void Reset(void) const;

	private:
		/* コマンドアロケータの生成 */
		ID3D12CommandAllocator* CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);
	};
}
