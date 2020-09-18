#include "..\include\Runtime.h"

ID3D12PipelineState * Dx12::GraphicsPipeline::CreatePipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const RootSignature* root, const ShaderCompiler* vertex,
	const ShaderCompiler* pixel, const ShaderCompiler* geometory, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology, const bool& depth)
{
	return Runtime::GetDevice()->CreateGraphicsPipeline(input, root, vertex, pixel, geometory, topology, depth);
}

Dx12::GraphicsPipeline::GraphicsPipeline()
{
}

Dx12::GraphicsPipeline::GraphicsPipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const RootSignature* root, const ShaderCompiler* vertex,
	const ShaderCompiler* pixel, const ShaderCompiler* geometory, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology, const bool& depth)
{
	obj = CreatePipeline(input, root, vertex, pixel, geometory, topology, depth);
}

Dx12::GraphicsPipeline::GraphicsPipeline(ID3D12PipelineState* pipe)
{
	obj = pipe;
}

Dx12::GraphicsPipeline::~GraphicsPipeline()
{
}
