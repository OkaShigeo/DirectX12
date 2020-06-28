#pragma once
#include "Dx12Heap.h"
#include "Dx12Resource.h"
#include <vector>

struct ID3D12Resource1;
class Dx12SwapChain;

class Dx12RenderTarget
{
public:
	/* コンストラクタ */
	Dx12RenderTarget(const Dx12SwapChain* swap);
	/* デストラクタ */
	~Dx12RenderTarget();

public:
	/* リソースの取得 */
	ID3D12Resource1* Get(void) const;
	/* CPUアドレスの取得 */
	std::uint64_t GetCpuAddress(void) const;
	
private:
	/* レンダーターゲットの生成 */
	std::uint32_t CreateRenderTarget(void);

public:
	/* ヒープ */
	Dx12Heap heap;
	/* リソース */
	std::vector<Dx12Resource>rsc;

private:
	/* スワップチェイン */
	const Dx12SwapChain* swap;
};

