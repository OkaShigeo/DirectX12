#include "..\include\Dx12Fence.h"
#include "..\include\Dx12CommandQueue.h"
#include "..\include\Dx12Runtime.h"

Dx12Fence::Dx12Fence(const Dx12CommandQueue* queue) :
	queue(queue), count(0)
{
	CreateFence();
}

Dx12Fence::~Dx12Fence()
{
}

std::uint32_t Dx12Fence::CreateFence(void)
{
	if (obj != nullptr) {
		OutputDebugStringA("フェンスが生成済みです\n");
		return S_FALSE;
	}

	auto hr = Dx12Runtime::GetDevice()->CreateFence(count,
		D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：フェンスが生成できませんでした\n");
	}

	return hr;
}

std::uint32_t Dx12Fence::Wait(void)
{
	if (obj == nullptr) {
		OutputDebugStringA("フェンスが生成されていません\n");
		return S_FALSE;
	}

	auto hr = queue->Get()->Signal(obj, ++count);
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：コマンドカウントの設定ができませんでした\n");
		return hr;
	}

	if (obj->GetCompletedValue() != count) {
		void* handle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);

		hr = obj->SetEventOnCompletion(count, handle);
		if (FAILED(hr)) {
			OutputDebugStringA("失敗：待機イベントの設定ができませんでした\n");
			return hr;
		}

		WaitForSingleObjectEx(handle, INFINITE, false);

		CloseHandle(handle);
	}

	return hr;
}
