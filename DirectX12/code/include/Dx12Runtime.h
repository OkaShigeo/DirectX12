#pragma once
#include "Vector.h"
#include <vector>

struct ID3D12Device6;
struct ID3D12DescriptorHeap;
struct ID3D12Resource1;
class Window;
class Dx12Device;
class Dx12CommandList;
class Dx12CommandQueue;
class Dx12SwapChain;
class Dx12Heap;
class Dx12Resource;
class Dx12RenderTarget;

class Dx12Runtime
{
public:
	/* デストラクタ */
	~Dx12Runtime();
	/* 初期化 */
	static void Initialized(const st::Vec2& window_size);
	/* 終了処理 */
	static void UnInitialized(void);
	/* ウィンドウメッセージの確認 */
	static bool CheckMsg(void);
	/* RTVの生成 */
	static void RTV(const Dx12Heap* heap, const Dx12Resource* rsc, const std::uint32_t& index = 0);
	/* RTVの生成 */
	static void RTV(ID3D12DescriptorHeap* heap, ID3D12Resource1* rsc, const std::uint32_t& index = 0);
	/* レンダーターゲットのクリア */
	static void ClearRenderTarget(const Dx12CommandList* list, const float color[4]);
	/* コマンドの実行 */
	static void ExecuteCommand(const std::vector<Dx12CommandList*>& list);

public:
	/* ウィンドウサイズの取得 */
	static st::Vec2 GetWindowSize(void);
	/* デバイスの取得 */
	static ID3D12Device6* GetDevice(void);
	/* レンダーターゲットのリソースの取得 */
	static ID3D12Resource1* GetRenderTarget(void);

private:
	/* コンストラクタ */
	Dx12Runtime();


private:
	/* ウィンドウ */
	static Window* window;
	/* デバイス */
	static Dx12Device* device;
	/* コマンドキュー */
	static Dx12CommandQueue* queue;
	/* スワップチェイン */
	static Dx12SwapChain* swap;
	/* レンダーターゲット */
	static Dx12RenderTarget* render;
};
