#include "..\include\Dx12Pipeline.h"
#include "..\include\ShaderCompiler.h"
#include "..\include\Dx12RootSignature.h"
#include "..\include\Dx12Runtime.h"

Dx12Pipeline::Dx12Pipeline()
{
}

Dx12Pipeline::~Dx12Pipeline()
{
}

std::uint32_t Dx12Pipeline::CreatePipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const Dx12RootSignature* root,
	const ShaderCompiler* vertex, const ShaderCompiler* pixel, const ShaderCompiler* geometory, const bool& depth_flag)
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
	desc.DepthStencilState.DepthEnable                  = depth_flag;
	desc.DepthStencilState.DepthWriteMask               = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc                    = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
	desc.DepthStencilState.StencilEnable                = depth_flag;
	desc.DepthStencilState.StencilReadMask              = 0xff;
	desc.DepthStencilState.StencilWriteMask             = 0xff;
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
	desc.PrimitiveTopologyType = type;
	desc.pRootSignature        = root->Get();
	desc.PS.pShaderBytecode    = pixel->Get()->GetBufferPointer();
	desc.PS.BytecodeLength     = pixel->Get()->GetBufferSize();
	desc.RasterizerState       = rasterizer;
	desc.RTVFormats[0]         = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleMask            = UINT_MAX;
	desc.SampleDesc            = { 1, 0 };
	desc.VS.pShaderBytecode    = vertex->Get()->GetBufferPointer();
	desc.VS.BytecodeLength     = vertex->Get()->GetBufferSize();

	auto hr = Dx12Runtime::GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：パイプラインが生成できませんでした\n");
	}

	return hr;
}

std::uint32_t Dx12Pipeline::CreatePipeline(const ShaderCompiler* shader, const Dx12RootSignature* root)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.BytecodeLength  = shader->Get()->GetBufferSize();
	desc.CS.pShaderBytecode = shader->Get()->GetBufferPointer();
	desc.Flags              = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask           = 0;
	desc.pRootSignature     = root->Get();

	auto hr = Dx12Runtime::GetDevice()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：コンピュート用のパイプラインが生成できませんでした\n");
	}

	return hr;
}
