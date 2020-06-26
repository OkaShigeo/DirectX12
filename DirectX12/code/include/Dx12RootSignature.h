#pragma once
#include "DxBaseObject.h"
#include <vector>

class ShaderCompiler;

class Dx12RootSignature :
	public DxBaseObject<ID3D12RootSignature>
{
public:
	/* コンストラクタ */
	Dx12RootSignature(const ShaderCompiler* shader);
	/* デストラクタ */
	~Dx12RootSignature();

private:
	/* ルートシグネチャの生成 */
	std::uint32_t CreateRootSignature(ID3DBlob* shader);
	/* ルートシグネチャの生成 */
	std::uint32_t CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler);
};
