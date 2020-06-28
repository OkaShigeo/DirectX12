#include "..\include\Dx12RootSignature.h"
#include "..\include\ShaderCompiler.h"
#include "..\include\Dx12Runtime.h"
#include <wrl.h>

Dx12RootSignature::Dx12RootSignature()
{
}

Dx12RootSignature::~Dx12RootSignature()
{
}

/* ���[�g�V�O�l�`���̐��� */
std::uint32_t Dx12RootSignature::CreateRootSignature(ID3DBlob* shader)
{
	auto hr = Dx12Runtime::GetDevice()->CreateRootSignature(0, shader->GetBufferPointer(), shader->GetBufferSize(), IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("���s�F���[�g�V�O�l�`���������ł��܂���ł���\n");
	}

	return hr;
}

std::uint32_t Dx12RootSignature::CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler)
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
	if (FAILED(hr)) {
		OutputDebugStringA("���s�F���[�g�V�O�l�`���̃V���A���C�Y�����ł��܂���ł���\n");
		return hr;
	}

	return CreateRootSignature(sig.Get());
}
