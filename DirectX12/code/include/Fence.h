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
		/*＊ フェンスの生成
		 * @param count フェンスカウント
		 * @return フェンス
		 */
		static ID3D12Fence1* CreateFence(const std::uint64_t& count = 0);

	public:
		/** コンストラクタ */
		Fence();
		/** コンストラクタ
		 * @param queue コマンドキュー
		 */
		Fence(const CommandQueue* queue);
		/** コンストラクタ 
		 * @param fence フェンス
		 */
		Fence(ID3D12Fence1* fence);
		/** デストラクタ */
		~Fence();

	public:
		/** コマンド完了の待機 */
		void Wait(void);

	public:
		/** フェンスカウントの取得 
		 * @return フェンスカウント
		 */
		std::uint64_t GetCount(void) const;

	public:
		/** フェンスカウントのセット 
		 * @param count フェンスカウント
		 */
		void SetCount(const std::uint64_t& count);
		
	private:
		/* コマンドキュー */
		const CommandQueue* queue{ nullptr };
		/* フェンスカウント */
		std::uint64_t count{ 0 };
	};
}
