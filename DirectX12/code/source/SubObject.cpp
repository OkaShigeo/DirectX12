#include "..\include\SubObject.h"

Dx12::SubObject::SubObject(const std::uint64_t& capacity)
{
	subs.reserve(capacity);
}

Dx12::SubObject::~SubObject()
{
}

bool Dx12::SubObject::AddSubObject(const D3D12_STATE_SUBOBJECT_TYPE& type, const void* data)
{
	if (index >= subs.capacity()) {
		subs.push_back({ type, data });
		return true;
	}

	return false;
}

const std::vector<D3D12_STATE_SUBOBJECT>& Dx12::SubObject::GetSubObjects(void) const
{
	return subs;
}

std::uint64_t Dx12::SubObject::GetSubObjNum(void) const
{
	return index;
}

std::uint64_t Dx12::SubObject::GetMaxSubObjNum(void) const
{
	return subs.capacity();
}
