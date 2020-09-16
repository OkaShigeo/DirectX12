#include "..\include\Device.h"
#include <wrl.h>
#include <cstdint>

namespace {
	/* ‹@”\ƒŒƒxƒ‹ˆê—— */
	const D3D_FEATURE_LEVEL levels[]{
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
	};
}

ID3D12Device6* Dx12::Device::CreateDevice(void)
{
	Microsoft::WRL::ComPtr<IDXGIFactory7>factory;
	std::uint32_t debug_flag = 0;
#ifdef _DEBUG
	debug_flag = DXGI_CREATE_FACTORY_DEBUG;
#endif
	auto hr = CreateDXGIFactory2(debug_flag, IID_PPV_ARGS(&factory));
	assert(hr == S_OK);

	ID3D12Device6* device = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter1>adaptor = nullptr;
	DXGI_ADAPTER_DESC1 desc{};
	for (std::uint32_t i = 0U; factory->EnumAdapters1(i, (IDXGIAdapter1**)&adaptor) != DXGI_ERROR_NOT_FOUND; ++i) {
		hr = adaptor->GetDesc1(&desc);
		assert(hr == S_OK);

		for (const D3D_FEATURE_LEVEL& level : levels) {
			hr = D3D12CreateDevice(adaptor.Get(), level, IID_PPV_ARGS(&device));
			if (SUCCEEDED(hr)) {
				D3D12_FEATURE_DATA_D3D12_OPTIONS5 option{};
				if (SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_D3D12_OPTIONS5, &option, sizeof(option)))) {
					if (option.RaytracingTier != D3D12_RAYTRACING_TIER::D3D12_RAYTRACING_TIER_NOT_SUPPORTED) {
						return device;
					}
					else {
						device->Release();
					}
				}
			}
		}
	}

	hr = factory->EnumWarpAdapter(IID_PPV_ARGS(&adaptor));
	assert(hr == S_OK);

	for (const D3D_FEATURE_LEVEL& level : levels) {
		hr = D3D12CreateDevice(adaptor.Get(), level, IID_PPV_ARGS(&device));
		if (SUCCEEDED(hr)) {
			return device;
		}
	}

	return nullptr;
}

Dx12::Device::Device()
{
	obj = CreateDevice();
	assert(obj != nullptr);
}

Dx12::Device::Device(ID3D12Device6* device)
{
	obj = device;
}

Dx12::Device::~Device()
{
}
