#pragma once
#include "BaseObject.h"
#include "Vector.h"

class Window;

namespace Dx12
{
	class CommandQueue;

	class SwapChain :
		public BaseObject<IDXGISwapChain4>
	{
	public:
		/* コンストラクタ */
		SwapChain(const Window* window, const CommandQueue* queue, const std::uint32_t& buffer_num = 2);
		/* デストラクタ */
		~SwapChain();

	public:
		/* バッファの切り替え*/
		void Present(void) const;

	public:
		/* サイズの取得 */
		Vec2 GetSize(void) const;
		/* バッファ数の取得 */
		std::uint32_t GetBufferNum(void) const;
		/* 現在のバッファ番号を取得 */
		std::uint32_t GetBufferIndex(void) const;

	private:
		/* スワップチェインの生成 */
		IDXGISwapChain4* CreateSwapChain(const Window* window, const CommandQueue* queue, const std::uint32_t& buffer_num);
	};
}
