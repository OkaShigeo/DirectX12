#include "..\include\Runtime.h"
#include <wrl.h>

D3D12_DESCRIPTOR_RANGE1 Dx12::RootSignature::GetRange(const D3D12_DESCRIPTOR_RANGE_TYPE& type, const std::uint32_t& index, const std::uint32_t& descriptor_num, const std::uint32_t& space)
{
	D3D12_DESCRIPTOR_RANGE1 range{};
	range.BaseShaderRegister                = index;
	range.Flags                             = D3D12_DESCRIPTOR_RANGE_FLAGS::D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
	range.NumDescriptors                    = 1;
	range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	range.RangeType                         = type;
	range.RegisterSpace                     = space;

	return range;
}

D3D12_ROOT_PARAMETER1 Dx12::RootSignature::GetParam(const D3D12_ROOT_PARAMETER_TYPE& type, const std::uint32_t& index, const std::vector<D3D12_DESCRIPTOR_RANGE1>& ranges, const std::uint32_t& value, const std::uint32_t& space)
{
	D3D12_ROOT_PARAMETER1 param{};
	param.ParameterType    = type;
	param.ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;

	switch (param.ParameterType)
	{
	case D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
		param.Constants.Num32BitValues = value;
		param.Constants.RegisterSpace  = index;
		param.Constants.ShaderRegister = space;
		break;
	case D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_CBV:
	case D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_SRV:
	case D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_UAV:
		param.Descriptor.Flags          = D3D12_ROOT_DESCRIPTOR_FLAGS::D3D12_ROOT_DESCRIPTOR_FLAG_NONE;
		param.Descriptor.RegisterSpace  = index;
		param.Descriptor.ShaderRegister = space;
		break;
	case D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
		param.DescriptorTable.NumDescriptorRanges = std::uint32_t(ranges.size());
		param.DescriptorTable.pDescriptorRanges   = ranges.data();
		break;
	default:
		return D3D12_ROOT_PARAMETER1();
		break;
	}

	return param;
}

ID3D12RootSignature* Dx12::RootSignature::CreateRootSignature(const ShaderCompiler * shader)
{
	return Runtime::GetDevice()->CreateRootSignature(shader);
}

ID3D12RootSignature * Dx12::RootSignature::CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS & flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, 
	const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler)
{
	D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc{};
	desc.Desc_1_1.Flags             = flag;
	desc.Desc_1_1.NumParameters     = std::uint32_t(param.size());
	desc.Desc_1_1.NumStaticSamplers = std::uint32_t(sampler.size());
	desc.Desc_1_1.pParameters       = param.data();
	desc.Desc_1_1.pStaticSamplers   = sampler.data();
	desc.Version                    = D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_1;

	Microsoft::WRL::ComPtr<ID3DBlob>sig   = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob>error = nullptr;

	auto hr = D3D12SerializeVersionedRootSignature(&desc, &sig, &error);
	assert(hr == S_OK);

	return CreateRootSignature(&ShaderCompiler(sig.Get()));
}

Dx12::RootSignature::RootSignature()
{
}

Dx12::RootSignature::RootSignature(const ShaderCompiler* shader)
{
	obj = CreateRootSignature(shader);
}

Dx12::RootSignature::RootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler)
{
	obj = CreateRootSignature(flag, param, sampler);
}

Dx12::RootSignature::RootSignature(ID3D12RootSignature* root)
{
	obj = root;
}

Dx12::RootSignature::~RootSignature()
{
}

void Dx12::RootSignature::SetConfig(const D3D12_STATE_SUBOBJECT_TYPE& type)
{
	config.pDesc = &obj;
	config.Type  = type;
}

void Dx12::RootSignature::AddSubObject(SubObject* sub)
{
	sub->AddSubObject(config);
}

void Dx12::RootSignature::AddSubObject(SubObject* sub, const D3D12_STATE_SUBOBJECT_TYPE& type)
{
	SetConfig(type);
	AddSubObject(sub);
}

void Dx12::RootSignature::AddSubObject(SubObject* sub, const D3D12_STATE_SUBOBJECT_TYPE& type, const std::vector<Str::String>& func_name)
{
	AddSubObject(sub, type);
	SetAssociation(func_name);

	association.pSubobjectToAssociate = &(*sub->GetSubObjects().rbegin());
	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION, &association);
}
