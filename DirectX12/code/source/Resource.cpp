#include "..\include\Resource.h"
#include "..\include\Runtime.h"

D3D12_HEAP_PROPERTIES Dx12::Resource::GetDefaultProp(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask      = 0;

	return prop;
}

D3D12_HEAP_PROPERTIES Dx12::Resource::GetUploadProp(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 0;

	return prop;
}

D3D12_HEAP_PROPERTIES Dx12::Resource::GetReadbackProp(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_READBACK;
	prop.VisibleNodeMask      = 0;

	return prop;
}

ID3D12Resource2 * Dx12::Resource::CreateBufferResource(const D3D12_RESOURCE_STATES & state, const D3D12_HEAP_PROPERTIES & prop, const std::uint64_t & size, const D3D12_RESOURCE_FLAGS & flag, const D3D12_CLEAR_VALUE* clear)
{
	D3D12_RESOURCE_DESC1 desc{};
	desc.Dimension                = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.DepthOrArraySize         = 1;
	desc.Flags                    = flag;
	desc.Format                   = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height                   = 1;
	desc.Layout                   = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels                = 1;
	desc.SampleDesc               = { 1, 0 };
	desc.SamplerFeedbackMipRegion = {};
	desc.Width                    = size;

	ID3D12Resource2* rsc = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, 
		(D3D12_RESOURCE_DESC*)&desc, state, clear, nullptr, IID_PPV_ARGS(&rsc));
	assert(hr == S_OK);

	return rsc;
}

ID3D12Resource2 * Dx12::Resource::CreateTextureResource(const D3D12_RESOURCE_STATES & state, const D3D12_HEAP_PROPERTIES & prop, const DXGI_FORMAT & format, const std::uint64_t & width, const std::uint32_t & height, const D3D12_RESOURCE_FLAGS & flag, const D3D12_CLEAR_VALUE * clear)
{
	D3D12_RESOURCE_DESC1 desc{};
	desc.DepthOrArraySize         = 1;
	desc.Dimension                = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags                    = flag;
	desc.Format                   = format;
	desc.Height                   = height;
	desc.Layout                   = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels                = 1;
	desc.SampleDesc               = { 1, 0 };
	desc.SamplerFeedbackMipRegion = {};
	desc.Width                    = width;

	ID3D12Resource2* rsc = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, 
		(D3D12_RESOURCE_DESC*)&desc, state, clear, nullptr, IID_PPV_ARGS(&rsc));
	assert(hr == S_OK);

	return rsc;
}

Dx12::Resource::Resource()
{
}

Dx12::Resource::Resource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size, const D3D12_RESOURCE_FLAGS& flag, const D3D12_CLEAR_VALUE* clear)
{
	obj = CreateBufferResource(state, prop, size, flag, clear);
}

Dx12::Resource::Resource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format, const std::uint64_t& width, const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag, const D3D12_CLEAR_VALUE* clear)
{
	obj = CreateTextureResource(state, prop, format, width, height, flag, clear);
}

Dx12::Resource::Resource(ID3D12Resource2* rsc)
{
	obj = rsc;
}

Dx12::Resource::~Resource()
{
}

ID3D12Resource2* Dx12::Resource::UpdateSubResource(const std::vector<std::uint8_t>& data, std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>& information, const std::uint32_t& offset, const std::uint32_t& texture_num)
{
	D3D12_RESOURCE_DESC1 desc = obj->GetDesc1();
	information.resize(desc.MipLevels * (desc.Dimension == D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE3D) ? texture_num : desc.DepthOrArraySize);

	ID3D12Resource2* upload = CreateBufferResource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, GetUploadProp(), GetSize(information.data(), information.size(), offset));

	std::uint8_t* buffer = nullptr;
	auto hr = upload->Map(0, nullptr, (void**)&buffer);
	assert(hr == S_OK);
	for (auto& i : information) {
		std::memcpy(buffer + i.Offset, &data[i.Offset], i.Footprint.RowPitch * i.Footprint.Height);
	}
	upload->Unmap(0, nullptr);

	return upload;
}

void Dx12::Resource::ReleaseBuffer(void) const
{
	D3D12_RANGE range{};
	obj->Unmap(0, &range);
}

Dx12::DescriptorHeap * Dx12::Resource::GetHeap(void) const
{
	return heap;
}

D3D12_GPU_VIRTUAL_ADDRESS Dx12::Resource::GetAddress(void) const
{
	return obj->GetGPUVirtualAddress();
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12::Resource::GetCpuHandle(void) const
{
	return { heap->Get()->GetCPUDescriptorHandleForHeapStart().ptr + (Runtime::GetDevice()->Get()->GetDescriptorHandleIncrementSize(heap->Get()->GetDesc().Type) * count) };
}

D3D12_GPU_DESCRIPTOR_HANDLE Dx12::Resource::GetGpuHandle(void) const
{
	return { heap->Get()->GetGPUDescriptorHandleForHeapStart().ptr + (Runtime::GetDevice()->Get()->GetDescriptorHandleIncrementSize(heap->Get()->GetDesc().Type) * count) };
}

std::uint8_t * Dx12::Resource::GetBuffer(void) const
{
	D3D12_RANGE range{};
	std::uint8_t* buffer = nullptr;
	auto hr = obj->Map(0, &range,(void**)&buffer);
	assert(hr == S_OK);
	
	return buffer;
}

std::uint64_t Dx12::Resource::GetSize(D3D12_PLACED_SUBRESOURCE_FOOTPRINT* information, const std::uint64_t& num, const std::uint32_t& offset) const
{
	auto desc = obj->GetDesc();
	std::uint64_t size = 0;
	Dx12::Runtime::GetDevice()->Get()->GetCopyableFootprints(&desc, offset, std::uint32_t(num), 0, information, nullptr, nullptr, &size);

	return size;
}
