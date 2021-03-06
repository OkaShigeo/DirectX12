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

		Runtime::GetDevice()->CreateShaderResourceView(resource);

		return true;
	}

	return false;
}

bool Dx12::DescriptorHeap::CreateShaderResourceView(AccelerationStructure* acceleration)
{
	if (count < obj->GetDesc().NumDescriptors) {
		acceleration->heap  = this;
		acceleration->count = count++;

		Runtime::GetDevice()->CreateShaderResourceView(acceleration);

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
