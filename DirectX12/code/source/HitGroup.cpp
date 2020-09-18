#include "..\include\Runtime.h"

Dx12::HitGroup::HitGroup()
{
}

Dx12::HitGroup::HitGroup(SubObject* sub, const Str::String& hit_group_name, const char* closetshit_name, const char* anyhit_name, const char* intersect_name)
{
	AddSubObject(sub, hit_group_name, closetshit_name, anyhit_name, intersect_name);
}

Dx12::HitGroup::~HitGroup()
{
}

void Dx12::HitGroup::AddSubObject(SubObject* sub, const Str::String& hit_group_name, const char* closetshit_name, const char* anyhit_name, const char* intersect_name)
{
	auto group = hit_group_name.GetUniCode();

	hit.HitGroupExport = group.c_str();

	if (closetshit_name != nullptr) {
		auto closesthit = Str::String(closetshit_name).GetUniCode();
		hit.ClosestHitShaderImport = closesthit.c_str();
	}

	if (anyhit_name != nullptr) {
		auto anyhit = Str::String(anyhit_name).GetUniCode();
		hit.AnyHitShaderImport = anyhit.c_str();
	}

	if (intersect_name != nullptr) {
		auto intersect = Str::String(intersect_name).GetUniCode();
		hit.IntersectionShaderImport = intersect.c_str();
	}

	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP, &hit);
}
