#include "..\include\RaytracingPipeline.h"
#include "..\include\SubObject.h"
#include "..\include\Runtime.h"
#include <cassert>

ID3D12StateObject* Dx12::RaytracingPipeline::CreateStateObject(void)
{
	return Runtime::GetDevice()->CreateRaytracingPipeline();
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
