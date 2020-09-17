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
		/** コマンドキューの生成 
		 * @param type コマンドタイプ
		 * @return コマンドキュー
		 */
		static ID3D12CommandQueue* CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);

	public:
		/** コンストラクタ 
		 * @param type コマンドタイプ
		 */
		CommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/** コンストラクタ
		 * @param queue コマンドキュー
		 */
		CommandQueue(ID3D12CommandQueue* queue);
		/** デストラクタ */
		~CommandQueue();

	public:
		/** コマンド実行 
		 * @param lists コマンドリストクラスの配列
		 */
		void Execution(const std::vector<CommandList*>& lists);
	};
}
