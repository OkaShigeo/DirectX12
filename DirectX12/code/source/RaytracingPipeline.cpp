#include "..\include\Runtime.h"

ID3D12StateObject* Dx12::RaytracingPipeline::CreateStateObject(const D3D12_STATE_OBJECT_TYPE& type, const SubObject* sub)
{
	return Runtime::GetDevice()->CreateStateObject(type, sub);
}

Dx12::RaytracingPipeline::RaytracingPipeline()
{
}

Dx12::RaytracingPipeline::RaytracingPipeline(const D3D12_STATE_OBJECT_TYPE& type, const SubObject* sub)
{
	obj = CreateStateObject(type, sub);
}

Dx12::RaytracingPipeline::RaytracingPipeline(ID3D12StateObject* pipe)
{
	obj = pipe;
}

Dx12::RaytracingPipeline::~RaytracingPipeline()
{
	if (prop != nullptr) {
		prop->Release();
		prop = nullptr;
	}
}

void Dx12::RaytracingPipeline::AddSubObject(SubObject* sub)
{
	config.NumExports          = 0;
	config.pExistingCollection = obj;
	config.pExports            = nullptr;
	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION, &config);
}

void* Dx12::RaytracingPipeline::GetShaderIdentifier(const Str::String& entry_name)
{
	return GetStateProp()->GetShaderIdentifier(entry_name.GetUniCodePtr());
}

ID3D12StateObjectProperties* Dx12::RaytracingPipeline::GetStateProp(void)
{
	if (prop == nullptr) {
		auto hr = obj->QueryInterface(IID_PPV_ARGS(&prop));
		assert(hr == S_OK);
	}

	return prop;
}
