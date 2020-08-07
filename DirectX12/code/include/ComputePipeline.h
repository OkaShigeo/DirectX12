#pragma once
#include "BaseObject.h"

namespace Dx12
{
	class ShaderCompiler;
	class RootSignature;

	class ComputePipeline :
		public BaseObject<ID3D12PipelineState>
	{
	public:
		/* �R���X�g���N�^ */
		ComputePipeline(const RootSignature* root, const ShaderCompiler* shader);
		ComputePipeline(ID3D12PipelineState* pipe);
		/* �f�X�g���N�^ */
		~ComputePipeline();

	private:
		/* �p�C�v���C���̐��� */
		ID3D12PipelineState* CreatePipeline(const RootSignature* root, const ShaderCompiler* shader);
	};
}
