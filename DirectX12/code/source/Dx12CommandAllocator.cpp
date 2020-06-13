#include "..\include\Dx12CommandAllocator.h"
#include "..\include\Dx12Runtime.h"
#include <d3d12.h>

Dx12CommandAllocator::Dx12CommandAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	CreateCommandAllocator(type);
}

Dx12CommandAllocator::~Dx12CommandAllocator()
{
}

std::uint32_t Dx12CommandAllocator::CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	if (obj != nullptr) {
		OutputDebugStringA("コマンドアロケータが生成済みです\n");
		return S_FALSE;
	}

	auto hr = Dx12Runtime::GetDevice()->CreateCommandAllocator(type, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：コマンドアロケータが生成できませんでした\n");
	}

	return hr;
}

std::uint32_t Dx12CommandAllocator::Reset(void) const
{
	if (obj == nullptr) {
		OutputDebugStringA("コマンドアロケータが生成されていません\n");
		return S_FALSE;
	}

	auto hr = obj->Reset();
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：コマンドアロケータのリセットができませんでした\n");
	}

	return hr;
}
