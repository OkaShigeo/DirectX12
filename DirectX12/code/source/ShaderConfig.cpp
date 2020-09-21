#include "..\include\Runtime.h"

Dx12::ShaderConfig::ShaderConfig()
{
}

Dx12::ShaderConfig::ShaderConfig(const std::uint64_t& payload, const std::uint64_t& attributes)
{
	SetConfig(payload, attributes);
}

Dx12::ShaderConfig::ShaderConfig(SubObject* sub, const std::uint64_t& payload, const std::uint64_t& attributes)
{
	AddSubObject(sub, payload, attributes);
}

Dx12::ShaderConfig::ShaderConfig(SubObject* sub, const std::uint64_t& payload, const std::vector<Str::String>& func_name, const std::uint64_t& attributes)
{
	AddSubObject(sub, payload, func_name, attributes);
}

Dx12::ShaderConfig::~ShaderConfig()
{
}

void Dx12::ShaderConfig::SetConfig(const std::uint64_t& payload, const std::uint64_t& attributes)
{
	config.MaxAttributeSizeInBytes = std::uint32_t(attributes);
	config.MaxPayloadSizeInBytes   = std::uint32_t(payload);
}

void Dx12::ShaderConfig::AddSubObject(SubObject* sub)
{
	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG, &config);
}

void Dx12::ShaderConfig::AddSubObject(SubObject* sub, const std::uint64_t& payload, const std::uint64_t& attributes)
{
	SetConfig(payload, attributes);
	AddSubObject(sub);
}

void Dx12::ShaderConfig::AddSubObject(SubObject* sub, const std::uint64_t& payload, const std::vector<Str::String>& func_name, const std::uint64_t& attributes)
{
	AddSubObject(sub, payload, attributes);
	SetAssociation(func_name);

	association.pSubobjectToAssociate = &(*sub->GetSubObjects().rbegin());
	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION, &association);
}
