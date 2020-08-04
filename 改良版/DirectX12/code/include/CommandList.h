#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class CommandList :
		public BaseObject<ID3D12CommandList>
	{
	public:
		/* コンストラクタ */
		CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/* デストラクタ */
		~CommandList();

	private:
		/* コマンドリストの生成 */
		ID3D12CommandList* CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);
	};
}
