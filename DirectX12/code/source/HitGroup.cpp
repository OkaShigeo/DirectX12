#include "..\include\Runtime.h"

Dx12::HitGroup::HitGroup()
{
}

Dx12::HitGroup::HitGroup(const Str::String& group_name, const char* closetshit_name, const char* anyhit_name, const char* intersection_name)
{
	SetConfig(group_name, closetshit_name, anyhit_name, intersection_name);
}

Dx12::HitGroup::HitGroup(SubObject* sub, const Str::String& group_name, const char* closetshit_name, const char* anyhit_name, const char* intersection_name)
{
	AddSubObject(sub, group_name, closetshit_name, anyhit_name, intersection_name);
}

Dx12::HitGroup::~HitGroup()
{
}

void Dx12::HitGroup::SetConfig(const Str::String& group_name, const char* closetshit_name, const char* anyhit_name, const char* intersection_name)
{
	config.HitGroupExport = group_name.GetUniCodePtr();
	
	if (closetshit_name != nullptr) {
		config.ClosestHitShaderImport = Str::String(closetshit_name).GetUniCodePtr();
	}
	
	if (anyhit_name != nullptr) {
		config.AnyHitShaderImport = Str::String(anyhit_name).GetUniCodePtr();
	}
	
	if (intersection_name != nullptr) {
		config.IntersectionShaderImport = Str::String(intersection_name).GetUniCodePtr();
	}
}

void Dx12::HitGroup::AddSubObject(SubObject* sub)
{
	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP, &config);
}

void Dx12::HitGroup::AddSubObject(SubObject* sub, const Str::String& group_name, const char* closetshit_name, const char* anyhit_name, const char* intersection_name)
{
	SetConfig(group_name, closetshit_name, anyhit_name, intersection_name);
	AddSubObject(sub);
}

