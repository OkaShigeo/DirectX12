#pragma once
#include "BaseObject.h"
#include <vector>

namespace Dx12
{
	class ShaderCompiler;
	class RootSignature;

	class GraphicsPipeline :
		public BaseObject<ID3D12PipelineState>
	{
	public:
		/* �p�C�v���C���̐���*/
		static ID3D12PipelineState* CreatePipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const RootSignature* root, const ShaderCompiler* vertex,
			const ShaderCompiler* pixel, const ShaderCompiler* geometory, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology, const bool& depth);

	public:
		/* �R���X�g���N�^ */
		GraphicsPipeline();
		GraphicsPipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const RootSignature* root, const ShaderCompiler* vertex, const ShaderCompiler* pixel, 
			const ShaderCompiler* geometory = nullptr, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, 
			const bool& depth = false);
		GraphicsPipeline(ID3D12PipelineState* pipe);
		/* �f�X�g���N�^ */
		~GraphicsPipeline();
	};
}
