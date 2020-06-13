#pragma once
#include <vector>
#include <memory>

struct ID3D12Resource1;
class Dx12SwapChain;
class Dx12Heap;

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

private:
	/* スワップチェイン */
	const Dx12SwapChain* swap;
	/* ヒープ */
	std::unique_ptr<Dx12Heap>heap;
	/* リソース */
	std::vector<ID3D12Resource1*>rsc;
};

