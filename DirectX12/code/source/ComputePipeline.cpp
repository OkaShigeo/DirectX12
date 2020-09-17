#include "..\include\ComputePipeline.h"
#include "..\include\Runtime.h"

ID3D12PipelineState * Dx12::ComputePipeline::CreatePipeline(const RootSignature * root, const ShaderCompiler * shader)
{
	return Runtime::GetDevice()->CreateComputePipeline(root, shader);
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
