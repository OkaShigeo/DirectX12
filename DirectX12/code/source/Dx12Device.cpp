#include "..\include\Dx12Device.h"
#include <wrl.h>

namespace {
	/* 機能レベル一覧 */
	const D3D_FEATURE_LEVEL levels[]{
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
	};
}

Dx12Device::Dx12Device()
{
	CreateDevice();
}

Dx12Device::~Dx12Device()
{
}

std::uint32_t Dx12Device::CreateDevice(void)
{
	Microsoft::WRL::ComPtr<IDXGIFactory7>factory;
	std::uint32_t debug_flag = 0;
#ifdef _DEBUG
	debug_flag = DXGI_CREATE_FACTORY_DEBUG;
#endif
	auto hr = CreateDXGIFactory2(debug_flag, IID_PPV_ARGS(&factory));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：ファクトリーが生成できませんでした\n");
		return hr;
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter1>adaptor = nullptr;
	DXGI_ADAPTER_DESC1 desc{};
	for (std::uint32_t i = 0U; factory->EnumAdapters1(i, (IDXGIAdapter1**)&adaptor) != DXGI_ERROR_NOT_FOUND; ++i) {
		hr = adaptor->GetDesc1(&desc);
		if (FAILED(hr)) {
			OutputDebugStringA("失敗：デバイスの詳細が不明です\n");
			return S_FALSE;
		}

		for (const D3D_FEATURE_LEVEL& level : levels) {
			hr = D3D12CreateDevice(adaptor.Get(), level, IID_PPV_ARGS(&obj));
			if (SUCCEEDED(hr)) {
				return S_OK;
			}
		}
	}

	hr = factory->EnumWarpAdapter(IID_PPV_ARGS(&adaptor));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：デフォルトアダプターの取得ができませんでした\n");
		return hr;
	}

	for (const D3D_FEATURE_LEVEL& level : levels) {
		hr = D3D12CreateDevice(adaptor.Get(), level, IID_PPV_ARGS(&obj));
		if (SUCCEEDED(hr)) {
			return hr;
		}
	}

	OutputDebugStringA("失敗：デバイスが生成できませんでした\n");
	return hr;
}
