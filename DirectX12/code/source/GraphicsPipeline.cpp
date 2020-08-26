#include "..\include\GraphicsPipeline.h"
#include "..\include\Runtime.h"

ID3D12PipelineState * Dx12::GraphicsPipeline::CreatePipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const RootSignature* root, const ShaderCompiler* vertex,
	const ShaderCompiler* pixel, const ShaderCompiler* geometory, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology, const bool& depth)
{
	D3D12_RASTERIZER_DESC rasterizer{};
	rasterizer.CullMode        = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
	rasterizer.DepthClipEnable = true;
	rasterizer.FillMode        = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;

	D3D12_RENDER_TARGET_BLEND_DESC renderBlend{};
	renderBlend.BlendEnable           = true;
	renderBlend.BlendOp               = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	renderBlend.BlendOpAlpha          = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	renderBlend.DestBlend             = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	renderBlend.DestBlendAlpha        = D3D12_BLEND::D3D12_BLEND_ZERO;
	renderBlend.LogicOp               = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	renderBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;
	renderBlend.SrcBlend              = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
	renderBlend.SrcBlendAlpha         = D3D12_BLEND::D3D12_BLEND_ONE;

	D3D12_BLEND_DESC blend{};
	for (int i = 0; i < 2; ++i)
	{
		blend.RenderTarget[i] = renderBlend;
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
	desc.BlendState                                     = blend;
	desc.DepthStencilState.DepthEnable                  = depth;
	desc.DepthStencilState.DepthWriteMask               = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc                    = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
	desc.DepthStencilState.StencilEnable                = depth;
	desc.DepthStencilState.StencilReadMask              = 0xFF;
	desc.DepthStencilState.StencilWriteMask             = 0xFF;
	desc.DepthStencilState.FrontFace.StencilFailOp      = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.FrontFace.StencilFunc        = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
	desc.DepthStencilState.FrontFace.StencilPassOp      = D3D12_STENCIL_OP::D3D12_STENCIL_OP_INCR;
	desc.DepthStencilState.BackFace.StencilFailOp       = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.BackFace.StencilDepthFailOp  = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.BackFace.StencilPassOp       = D3D12_STENCIL_OP::D3D12_STENCIL_OP_DECR;
	desc.DepthStencilState.BackFace.StencilFunc         = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
	desc.DSVFormat                                      = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	if (geometory != nullptr)
	{
		desc.GS.pShaderBytecode = geometory->Get()->GetBufferPointer();
		desc.GS.BytecodeLength  = geometory->Get()->GetBufferSize();
	}
	desc.InputLayout           = { input.data(), std::uint32_t(input.size()) };
	desc.NumRenderTargets      = 1;
	desc.PrimitiveTopologyType = topology;
	desc.pRootSignature        = root->Get();
	desc.PS.pShaderBytecode    = pixel->Get()->GetBufferPointer();
	desc.PS.BytecodeLength     = pixel->Get()->GetBufferSize();
	desc.RasterizerState       = rasterizer;
	desc.RTVFormats[0]         = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleMask            = UINT_MAX;
	desc.SampleDesc            = { 1, 0 };
	desc.VS.pShaderBytecode    = vertex->Get()->GetBufferPointer();
	desc.VS.BytecodeLength     = vertex->Get()->GetBufferSize();

	ID3D12PipelineState* pipe = nullptr;
	auto hr = Runtime::GetDevice()->Get()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipe));
	assert(hr == S_OK);

	return pipe;
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
	Release();
	obj = pipe;
}

Dx12::GraphicsPipeline::~GraphicsPipeline()
{
}
