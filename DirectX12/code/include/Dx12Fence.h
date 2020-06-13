#pragma once
#include "DxBaseObject.h"

class Dx12CommandQueue;

class Dx12Fence :
	public DxBaseObject<ID3D12Fence1>
{
public:
	/* コンストラクタ */
	Dx12Fence(const Dx12CommandQueue* queue);
	/* デストラクタ */
	~Dx12Fence();
	/* コマンド実行の待機 */
	std::uint32_t Wait(void);

private:
	/* フェンスの生成 */
	std::uint32_t CreateFence(void);

private:
	/* コマンドキュー */
	const Dx12CommandQueue* queue;
	/* カウント */
	std::uint64_t count;
};
