#pragma once
#include "BaseObject.h"
#include "Vector.h"

class Window;

namespace Dx12
{
	class CommandQueue;
	class Resource;

	class SwapChain :
		public BaseObject<IDXGISwapChain4>
	{
	public:
		/*＊ スワップチェインの生成 
		 * @param window ウィンドウ
		 * @param queue コマンドキュー
		 * @param buffer_num バックバッファー数
		 * @return スワップチェイン
		 */
		static IDXGISwapChain4* CreateSwapChain(const Window* window, const CommandQueue* queue, const std::uint32_t& buffer_num);

	public:
		/** コンストラクタ */
		SwapChain();
		/** コンストラクタ
		 * @param window ウィンドウ
		 * @param queue コマンドキュー
		 * @param buffer_num バックバッファー数
		 */
		SwapChain(const Window* window, const CommandQueue* queue, const std::uint32_t& buffer_num = 2);
		/** コンストラクタ
		 * @param swap スワップチェイン
		 */
		SwapChain(IDXGISwapChain4* swap);
		/*＊ デストラクタ */
		~SwapChain();

	public:
		/** リソースの取得 
		 * @param index リソース配列のインデックス
		 * @return リソース
		 */
		ID3D12Resource2* GetResource(const std::uint32_t& index);
		/*＊ バッファの切り替え*/
		void Present(void) const;

	public:
		/*＊ サイズの取得 
		 * @return リソースの縦横サイズ
		 */
		Math::Vec2 GetSize(void) const;
		/** バッファ数の取得 
		 * @return バックバッファー数
		 */
		std::uint32_t GetBufferNum(void) const;
		/*＊ 現在のバッファ番号を取得 
		 * @return バックバッファーのインデックス
		 */
		std::uint32_t GetBufferIndex(void) const;
	};
}
