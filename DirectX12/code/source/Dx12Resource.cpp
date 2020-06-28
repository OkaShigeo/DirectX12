#include "..\include\Dx12Resource.h"
#include "..\include\Dx12Runtime.h"

Dx12Resource::Dx12Resource()
{
}

Dx12Resource::~Dx12Resource()
{
}

std::uint32_t Dx12Resource::CreateResource(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_FLAGS& flag, const std::uint64_t& size,
	const D3D12_RESOURCE_STATES& state)
{
	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.DepthOrArraySize = 1;
	desc.Flags            = flag;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = size;

	auto hr = Dx12Runtime::GetDevice()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, state, nullptr, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：バッファー用のリソースが生成できませんでした\n");
	}

	return hr;
}

std::uint32_t Dx12Resource::CreateResource(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_FLAGS& flag, const std::uint64_t& size_x, const std::uint32_t& size_y,
	const D3D12_RESOURCE_STATES& state, const DXGI_FORMAT& format)
{
	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags            = flag;
	desc.Format           = format;
	desc.Height           = size_y;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = size_x;

	auto hr = Dx12Runtime::GetDevice()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, state, nullptr, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：バッファー用のリソースが生成できませんでした\n");
	}

	return hr;
}

std::uint32_t Dx12Resource::Map(void** buffer, const std::uint64_t& size)
{
	D3D12_RANGE range{};
	auto hr = obj->Map(0, &range, &(*buffer));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：リソースのメモリ確保ができませんでした\n");
	}

	return hr;
}

void Dx12Resource::Unmap(void)
{
	D3D12_RANGE range{};
	obj->Unmap(0, &range);
}

D3D12_HEAP_PROPERTIES Dx12Resource::GetDefaultProp(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask      = 0;

	return prop;
}

D3D12_HEAP_PROPERTIES Dx12Resource::GetUploadProp(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 0;

	return prop;
}
