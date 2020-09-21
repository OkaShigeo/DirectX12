#include "..\include\Runtime.h"

Dx12::ObjectConfig::ObjectConfig()
{
}

Dx12::ObjectConfig::ObjectConfig(const D3D12_STATE_OBJECT_FLAGS& flag)
{
	SetConfig(flag);
}

Dx12::ObjectConfig::ObjectConfig(SubObject* sub, const D3D12_STATE_OBJECT_FLAGS& flag)
{
	AddSubObject(sub, flag);
}

Dx12::ObjectConfig::~ObjectConfig()
{
}

void Dx12::ObjectConfig::SetConfig(const D3D12_STATE_OBJECT_FLAGS& flag)
{
	config.Flags = flag;
}

void Dx12::ObjectConfig::AddSubObject(SubObject* sub)
{
	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_STATE_OBJECT_CONFIG, &config);
}

void Dx12::ObjectConfig::AddSubObject(SubObject* sub, const D3D12_STATE_OBJECT_FLAGS& flag)
{
	SetConfig(flag);
	AddSubObject(sub);
}
