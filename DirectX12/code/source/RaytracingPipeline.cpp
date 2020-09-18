#include "..\include\Runtime.h"

ID3D12StateObject* Dx12::RaytracingPipeline::CreateStateObject(const SubObject* sub, const D3D12_STATE_OBJECT_TYPE& type)
{
	return Runtime::GetDevice()->CreateStateObject(sub, type);
}

Dx12::RaytracingPipeline::RaytracingPipeline()
{
}

Dx12::RaytracingPipeline::RaytracingPipeline(const SubObject* sub, const D3D12_STATE_OBJECT_TYPE& type)
{
	obj = CreateStateObject(sub, type);
}

Dx12::RaytracingPipeline::RaytracingPipeline(ID3D12StateObject* pipe)
{
	obj = pipe;
}

Dx12::RaytracingPipeline::~RaytracingPipeline()
{
}

void Dx12::RaytracingPipeline::AddSubObject(SubObject* sub)
{
	collection.NumExports          = 0;
	collection.pExistingCollection = obj;
	collection.pExports            = nullptr;

	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION, &collection);
}
