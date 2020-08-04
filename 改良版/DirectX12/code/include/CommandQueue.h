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
		/* コンストラクタ */
		CommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/* デストラクタ */
		~CommandQueue();

	public:
		/* コマンド実行 */
		void Execution(const std::vector<CommandList*>& lists);

	private:
		/* コマンドキューの生成 */
		ID3D12CommandQueue* CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type);
	};
}
