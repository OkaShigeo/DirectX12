#include "..\include\ComputePipeline.h"
#include "..\include\Runtime.h"

ID3D12PipelineState * Dx12::ComputePipeline::CreatePipeline(const RootSignature * root, const ShaderCompiler * shader)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.BytecodeLength  = shader->Get()->GetBufferSize();
	desc.CS.pShaderBytecode = shader->Get()->GetBufferPointer();
	desc.Flags              = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask           = 0;
	desc.pRootSignature     = root->Get();

	ID3D12PipelineState* pipe = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipe));
	assert(hr == S_OK);

	return pipe;
}

Dx12::ComputePipeline::ComputePipeline()
{
}

Dx12::ComputePipeline::ComputePipeline(const RootSignature* root, const ShaderCompiler* shader)
{
	obj = CreatePipeline(root, shader);
}

Dx12::ComputePipeline::ComputePipeline(ID3D12PipelineState* pipe)
{
	obj = pipe;
}

Dx12::ComputePipeline::~ComputePipeline()
{
}
