#include "..\include\Runtime.h"

ID3D12Resource2* Dx12::VertexBuffer::CreateVertexBuffer(void* vertex)
{
	auto* rsc = Runtime::GetDevice()->CreateBufferResource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Resource::GetUploadProp(), _msize(vertex));
	
	void* buffer = nullptr;
	auto hr = rsc->Map(0, nullptr, &buffer);
	assert(hr == S_OK);
	std::memcpy(buffer, vertex, rsc->GetDesc().Width);
	rsc->Unmap(0, nullptr);

	return rsc;
}

Dx12::VertexBuffer::VertexBuffer()
{
}

Dx12::VertexBuffer::VertexBuffer(void* vertex, const std::uint64_t& vertex_num)
{
	obj = CreateVertexBuffer(vertex);
	num = vertex_num;
}

Dx12::VertexBuffer::VertexBuffer(ID3D12Resource2* resource)
{
	obj = resource;
}

Dx12::VertexBuffer::~VertexBuffer()
{
}

void Dx12::VertexBuffer::UpdateVertex(const void* vertex) const
{
	void* buffer = nullptr;
	auto hr = obj->Map(0, nullptr, &buffer);
	assert(hr == S_OK);
	std::memcpy(buffer, vertex, obj->GetDesc().Width);
	obj->Unmap(0, nullptr);
}

std::uint64_t Dx12::VertexBuffer::GetNum(void) const
{
	return num;
}

void Dx12::VertexBuffer::SetNum(const std::uint64_t& vertex_num)
{
	num = vertex_num;
}
