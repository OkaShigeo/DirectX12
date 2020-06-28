#pragma once
#include "DxBaseObject.h"
#include <vector>

class ShaderCompiler;

class Dx12RootSignature :
	public DxBaseObject<ID3D12RootSignature>
{
public:
	/* �R���X�g���N�^ */
	Dx12RootSignature();
	/* �f�X�g���N�^ */
	~Dx12RootSignature();

	/* ���[�g�V�O�l�`���̐��� */
	std::uint32_t CreateRootSignature(ID3DBlob* shader);
	/* ���[�g�V�O�l�`���̐��� */
	std::uint32_t CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler);
};
