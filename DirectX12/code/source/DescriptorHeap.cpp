#include "..\include\DescriptorHeap.h"
#include "..\include\Runtime.h"

ID3D12DescriptorHeap* Dx12::DescriptorHeap::CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag)
{
	return Runtime::GetDevice()->CreateDescriptorHeap(type, rsc_num, flag);
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

		Runtime::GetDevice()->CreateRenderTarget(resource);

		return true;
	}

	return false;
}

bool Dx12::DescriptorHeap::CreateConstantBufferView(Resource* resource)
{
	if (count < obj->GetDesc().NumDescriptors) {
		resource->heap  = this;
		resource->count = count++;

		Runtime::GetDevice()->CreateConstantBufferView(resource);

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

		Runtime::GetDevice()->CreateUnorderAccessView(resource, element_num);

		return true;
	}

	return false;
}

bool Dx12::DescriptorHeap::CreateUnorderAccessView(Resource * resource)
{
	if (count < obj->GetDesc().NumDescriptors) {
		resource->heap  = this;
		resource->count = count++;

		Runtime::GetDevice()->CreateUnorderAccessView(resource);

		return true;
	}

	return false;
}

D3D12_DESCRIPTOR_HEAP_TYPE Dx12::DescriptorHeap::GetType(void) const
{
	if (obj != nullptr) {
		return obj->GetDesc().Type;
	}

	return D3D12_DESCRIPTOR_HEAP_TYPE();
}
