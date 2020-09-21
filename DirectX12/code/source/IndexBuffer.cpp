#include "..\include\Runtime.h"

ID3D12Resource2* Dx12::IndexBuffer::CreateIndexBuffer(void* index)
{
	auto* rsc = Runtime::GetDevice()->CreateBufferResource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Resource::GetUploadProp(), _msize(index));

	void* buffer = nullptr;
	auto hr = rsc->Map(0, nullptr, &buffer);
	assert(hr == S_OK);
	std::memcpy(buffer, index, rsc->GetDesc().Width);
	rsc->Unmap(0, nullptr);

	return rsc;
}

Dx12::IndexBuffer::IndexBuffer()
{
}

Dx12::IndexBuffer::IndexBuffer(void* index, const std::uint64_t& index_num)
{
	obj = CreateIndexBuffer(index);
	num = index_num;
}

Dx12::IndexBuffer::IndexBuffer(ID3D12Resource2* resource)
{
	obj = resource;
}

Dx12::IndexBuffer::~IndexBuffer()
{
}

void Dx12::IndexBuffer::UpdateIndex(const void* index) const
{
	void* buffer = nullptr;
	auto hr = obj->Map(0, nullptr, &buffer);
	assert(hr == S_OK);
	std::memcpy(buffer, index, obj->GetDesc().Width);
	obj->Unmap(0, nullptr);
}

std::uint64_t Dx12::IndexBuffer::GetNum(void) const
{
	return num;
}

void Dx12::IndexBuffer::SetNum(const std::uint64_t& index_num)
{
	num = index_num;
}
