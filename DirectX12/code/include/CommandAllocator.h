#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class CommandAllocator :
		public BaseObject<ID3D12CommandAllocator>
	{
	public:
		/** コマンドアロケータの生成 
		 * @param type コマンドリストの種別
		 * @return コマンドアロケータ
		 */
		static ID3D12CommandAllocator* CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);

	public:
		/*＊ コンストラクタ
		 * @param type コマンドリストの種別
		 */
		CommandAllocator(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/*＊ コンストラクタ
		 * @param allocator コマンドアロケータ
		 */
		CommandAllocator(ID3D12CommandAllocator* allocator);
		/** デストラクタ */
		~CommandAllocator();

	public:
		/** コマンドアロケータの内容をリセット */
		void Reset(void) const;	
	};
}
