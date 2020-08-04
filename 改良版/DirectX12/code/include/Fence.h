#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class CommandQueue;

	class Fence :
		public BaseObject<ID3D12Fence1>
	{
	public:
		/* コンストラクタ */
		Fence(const CommandQueue* queue);
		/* デストラクタ */
		~Fence();

	public:
		/* コマンド完了の待機 */
		void Wait(void);

	private:
		/* フェンスの生成 */
		ID3D12Fence1* CreateFence(void);

	private:
		/* コマンドキュー */
		const CommandQueue* queue;
		/* フェンスカウント */
		std::uint64_t count;
	};
}
