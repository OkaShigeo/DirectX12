#include "..\include\Descriptor.h"
#include "..\include\Runtime.h"

Dx12::Descriptor::Descriptor(const D3D12_DESCRIPTOR_HEAP_TYPE & type, const std::uint32_t & rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS & flag)
{
	obj = CreateDescriptorHeap(type, rsc_num, flag);
}

Dx12::Descriptor::Descriptor(ID3D12DescriptorHeap * heap)
{
	obj = heap;
}

Dx12::Descriptor::~Descriptor()
{
}

ID3D12DescriptorHeap * Dx12::Descriptor::CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE & type, const std::uint32_t & rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS & flag)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = flag;
	desc.NodeMask       = 0;
	desc.NumDescriptors = rsc_num;
	desc.Type           = type;

	ID3D12DescriptorHeap* heap = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	assert(hr == S_OK);

	return heap;
}

bool Dx12::Descriptor::CreateRenderTargetView(Resource * rsc)
{
	if (count < obj->GetDesc().NumDescriptors) {
		rsc->heap  = this;
		rsc->count = count++;

		D3D12_RENDER_TARGET_VIEW_DESC desc{};
		desc.Format        = rsc->Get()->GetDesc().Format;
		desc.Texture2D     = {};
		desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;

		Runtime::GetDevice()->Get()->CreateRenderTargetView(rsc->Get(), &desc, rsc->GetCpuHandle());

		return true;
	}

	return false;
}

bool Dx12::Descriptor::CreateShaderResourceView(Resource * rsc)
{
	if (count < obj->GetDesc().NumDescriptors) {
		rsc->heap  = this;
		rsc->count = count++;

		D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format                  = rsc->Get()->GetDesc().Format;
		desc.ViewDimension           = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels     = rsc->Get()->GetDesc().MipLevels;
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		Runtime::GetDevice()->Get()->CreateShaderResourceView(rsc->Get(), &desc, rsc->GetCpuHandle());

		return true;
	}

	return false;
}

bool Dx12::Descriptor::CreateUnorderAccessView(Resource* rsc, const std::uint32_t& element_num)
{
	if (count < obj->GetDesc().NumDescriptors) {
		rsc->heap  = this;
		rsc->count = count++;

		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.Buffer.CounterOffsetInBytes = 0;
		desc.Buffer.FirstElement         = 0;
		desc.Buffer.Flags                = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
		desc.Buffer.NumElements          = std::uint32_t(element_num);
		desc.Buffer.StructureByteStride  = std::uint32_t(rsc->Get()->GetDesc1().Width) / desc.Buffer.NumElements;
		desc.Format                      = rsc->Get()->GetDesc().Format;
		desc.ViewDimension               = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;

		Runtime::GetDevice()->Get()->CreateUnorderedAccessView(rsc->Get(), nullptr, &desc, rsc->GetCpuHandle());

		return true;
	}

	return false;
}

bool Dx12::Descriptor::CreateUnorderAccessView(Resource * rsc)
{
	if (count < obj->GetDesc().NumDescriptors) {
		rsc->heap  = this;
		rsc->count = count++;

		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.Format        = rsc->Get()->GetDesc().Format;
		desc.Texture2D     = {};
		desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;

		Runtime::GetDevice()->Get()->CreateUnorderedAccessView(rsc->Get(), nullptr, &desc, rsc->GetCpuHandle());

		return true;
	}

	return false;
}
