#include "..\include\RaytracingPipeline.h"
#include "..\include\SubObject.h"
#include "..\include\Runtime.h"
#include <cassert>

ID3D12StateObject* Dx12::RaytracingPipeline::CreateStateObject(void)
{
	D3D12_STATE_OBJECT_DESC desc{};
	desc.NumSubobjects = std::uint32_t(SubObject::GetSubObjList().size());
	desc.pSubobjects   = SubObject::GetSubObjListPtr();
	desc.Type          = D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;

	ID3D12StateObject* pipe = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateStateObject(&desc, IID_PPV_ARGS(&pipe));
	assert(hr == S_OK);

	return pipe;
}

Dx12::RaytracingPipeline::RaytracingPipeline()
{
	obj = CreateStateObject();
}

Dx12::RaytracingPipeline::RaytracingPipeline(ID3D12StateObject* pipe)
{
	Release();
	obj = pipe;
}

Dx12::RaytracingPipeline::~RaytracingPipeline()
{
}
