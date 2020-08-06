#include "..\include\RootSignature.h"
#include "..\include\Runtime.h"
#include <wrl.h>

Dx12::RootSignature::RootSignature(const ShaderCompiler * shader)
{
	obj = CreateRootSignature(shader);
}

Dx12::RootSignature::RootSignature(const D3D12_ROOT_SIGNATURE_FLAGS & flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler)
{
	obj = CreateRootSignature(flag, param, sampler);
}

Dx12::RootSignature::RootSignature(ID3D12RootSignature * root)
{
	obj = root;
}

Dx12::RootSignature::~RootSignature()
{
}

ID3D12RootSignature* Dx12::RootSignature::CreateRootSignature(const ShaderCompiler * shader)
{
	ID3D12RootSignature* root = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateRootSignature(0, shader->Get()->GetBufferPointer(), shader->Get()->GetBufferSize(), IID_PPV_ARGS(&root));
	assert(hr == S_OK);

	return root;
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

	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob>error = nullptr;

	auto hr = D3D12SerializeVersionedRootSignature(&desc, &sig, &error);
	assert(hr == S_OK);

	return CreateRootSignature(&ShaderCompiler(sig.Get()));
}
