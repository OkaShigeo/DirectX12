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
		/* �p�C�v���C���̐��� */
		static ID3D12PipelineState* CreatePipeline(const RootSignature* root, const ShaderCompiler* shader);

	public:
		/* �R���X�g���N�^ */
		ComputePipeline();
		ComputePipeline(const RootSignature* root, const ShaderCompiler* shader);
		ComputePipeline(ID3D12PipelineState* pipe);
		/* �f�X�g���N�^ */
		~ComputePipeline();
	};
}
