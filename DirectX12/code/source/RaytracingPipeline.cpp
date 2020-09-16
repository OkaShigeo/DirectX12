#include "..\include\RaytracingPipeline.h"
#include "..\include\SubObject.h"
#include "..\include\Runtime.h"
#include <cassert>

ID3D12StateObject* Dx12::RaytracingPipeline::CreateStateObject(void)
{
	auto& sub = SubObject::GetSubObjList();

	D3D12_STATE_OBJECT_DESC desc{};
	desc.NumSubobjects = std::uint32_t(sub.size());
	desc.pSubobjects   = sub.data();
	desc.Type          = D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;

	ID3D12StateObject* pipe = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateStateObject(&desc, IID_PPV_ARGS(&pipe));
	assert(hr == S_OK);

	sub.clear();
	sub.reserve(desc.NumSubobjects);

	return pipe;
}

Dx12::RaytracingPipeline::RaytracingPipeline()
{
	obj = CreateStateObject();
}

Dx12::RaytracingPipeline::RaytracingPipeline(ID3D12StateObject* pipe)
{
	obj = pipe;
}

Dx12::RaytracingPipeline::~RaytracingPipeline()
{
}
