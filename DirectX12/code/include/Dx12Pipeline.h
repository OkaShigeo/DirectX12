#pragma once
#include "DxBaseObject.h"
#include <vector>

class ShaderCompiler;
class Dx12RootSignature;

class Dx12Pipeline :
	public DxBaseObject<ID3D12PipelineState>
{
public:
	/* �R���X�g���N�^ */
	Dx12Pipeline();
	/* �f�X�g���N�^ */
	~Dx12Pipeline();

public:
	/* �p�C�v���C���̐��� */
	std::uint32_t CreatePipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const Dx12RootSignature* root,
		const ShaderCompiler* vertex, const ShaderCompiler* pixel, const ShaderCompiler* geometory = nullptr, const bool& depth_flag = false);
	/* �R���s���[�g�p�̃p�C�v���C���̐��� */
	std::uint32_t CreatePipeline(const ShaderCompiler* shader, const Dx12RootSignature* root);
};
