#include "..\include\Dx12Resource.h"
#include "..\include\Dx12Runtime.h"

static D3D12_HEAP_PROPERTIES GetDefaultProp(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask      = 0;

	return prop;
}

Dx12Resource::Dx12Resource()
{
}

Dx12Resource::~Dx12Resource()
{
}

std::uint32_t Dx12Resource::CreateResource(const D3D12_RESOURCE_FLAGS& flag, const std::uint64_t& size, D3D12_RESOURCE_STATES& state)
{
	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Flags            = flag;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = size;

	auto hr = Dx12Runtime::GetDevice()->CreateCommittedResource(&GetDefaultProp(), D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, state, nullptr, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：リソースが生成できませんでした\n");
	}

	return hr;
}
