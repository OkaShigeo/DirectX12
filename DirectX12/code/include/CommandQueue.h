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
		/* コマンドキューの生成 */
		static ID3D12CommandQueue* CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type);

	public:
		/* コンストラクタ */
		CommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		CommandQueue(ID3D12CommandQueue* queue);
		/* デストラクタ */
		~CommandQueue();

	public:
		/* コマンド実行 */
		void Execution(const std::vector<CommandList*>& lists);
	};
}
