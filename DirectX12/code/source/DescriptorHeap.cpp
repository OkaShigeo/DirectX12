#include "..\include\DescriptorHeap.h"
#include "..\include\Runtime.h"

ID3D12DescriptorHeap* Dx12::DescriptorHeap::CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = flag;
	desc.NodeMask       = 0;
	desc.NumDescriptors = std::uint32_t(rsc_num);
	desc.Type           = type;

	ID3D12DescriptorHeap* heap = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	assert(hr == S_OK);

	return heap;
}

Dx12::DescriptorHeap::DescriptorHeap()
{
}

Dx12::DescriptorHeap::DescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE & type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS & flag)
{
	obj = CreateDescriptorHeap(type, rsc_num, flag);
}

Dx12::DescriptorHeap::DescriptorHeap(ID3D12DescriptorHeap* heap)
{
	obj = heap;
}

Dx12::DescriptorHeap::~DescriptorHeap()
{
}

bool Dx12::DescriptorHeap::CreateRenderTargetView(Resource * resource)
{
	if (count < obj->GetDesc().NumDescriptors) {
		resource->heap  = this;
		resource->count = count++;

		D3D12_RENDER_TARGET_VIEW_DESC desc{};
		desc.Format        = resource->Get()->GetDesc().Format;
		desc.Texture2D     = {};
		desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;

		Runtime::GetDevice()->Get()->CreateRenderTargetView(resource->Get(), &desc, resource->GetCpuHandle());

		return true;
	}

	return false;
}

bool Dx12::DescriptorHeap::CreateConstantBufferView(Resource* resource)
{
	if (count < obj->GetDesc().NumDescriptors) {
		resource->heap  = this;
		resource->count = count++;

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
		desc.BufferLocation = resource->Get()->GetGPUVirtualAddress();
		desc.SizeInBytes    = std::uint32_t(resource->Get()->GetDesc().Width);

		Runtime::GetDevice()->Get()->CreateConstantBufferView(&desc, resource->GetCpuHandle());

		return true;
	}

	return false;
}

bool Dx12::DescriptorHeap::CreateShaderResourceView(Resource * resource)
{
	if (count < obj->GetDesc().NumDescriptors) {
		resource->heap  = this;
		resource->count = count++;

		D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format                  = resource->Get()->GetDesc().Format;
		desc.ViewDimension           = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels     = resource->Get()->GetDesc().MipLevels;
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		Runtime::GetDevice()->Get()->CreateShaderResourceView(resource->Get(), &desc, resource->GetCpuHandle());

		return true;
	}

	return false;
}

bool Dx12::DescriptorHeap::CreateUnorderAccessView(Resource* resource, const std::uint64_t& element_num)
{
	if (count < obj->GetDesc().NumDescriptors) {
		resource->heap  = this;
		resource->count = count++;

		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.Buffer.CounterOffsetInBytes = 0;
		desc.Buffer.FirstElement         = 0;
		desc.Buffer.Flags                = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
		desc.Buffer.NumElements          = std::uint32_t(element_num);
		desc.Buffer.StructureByteStride  = std::uint32_t(resource->Get()->GetDesc1().Width) / desc.Buffer.NumElements;
		desc.Format                      = resource->Get()->GetDesc().Format;
		desc.ViewDimension               = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;

		Runtime::GetDevice()->Get()->CreateUnorderedAccessView(resource->Get(), nullptr, &desc, resource->GetCpuHandle());

		return true;
	}

	return false;
}

bool Dx12::DescriptorHeap::CreateUnorderAccessView(Resource * resource)
{
	if (count < obj->GetDesc().NumDescriptors) {
		resource->heap  = this;
		resource->count = count++;

		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.Format        = resource->Get()->GetDesc().Format;
		desc.Texture2D     = {};
		desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;

		Runtime::GetDevice()->Get()->CreateUnorderedAccessView(resource->Get(), nullptr, &desc, resource->GetCpuHandle());

		return true;
	}

	return false;
}
