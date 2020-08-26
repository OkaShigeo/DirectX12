#include "..\include\HitGroup.h"

Dx12::HitGroup::HitGroup()
{
}

Dx12::HitGroup::HitGroup(const std::wstring& hit_name, const std::vector<std::wstring>& func_name)
{
	AddSubObj(hit_name, func_name);
}

Dx12::HitGroup::~HitGroup()
{
}

void Dx12::HitGroup::AddSubObj(const std::wstring & hit_name, const std::vector<std::wstring> & func_name)
{
	D3D12_HIT_GROUP_DESC desc{};
	desc.HitGroupExport = hit_name.c_str();

	std::uint32_t index = 0;
	for (auto& i : func_name) {
		switch (index)
		{
		case 0:
			desc.ClosestHitShaderImport = i.c_str();
			break;
		case 1:
			desc.AnyHitShaderImport = i.c_str();
			break;
		case 2:
			desc.IntersectionShaderImport = i.c_str();
			break;
		default:
			break;
		}

		++index;
	}

	sub.push_back({ D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP, &desc });
}
