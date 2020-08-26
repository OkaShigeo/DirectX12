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
		/* パイプラインの生成 */
		static ID3D12PipelineState* CreatePipeline(const RootSignature* root, const ShaderCompiler* shader);

	public:
		/* コンストラクタ */
		ComputePipeline();
		ComputePipeline(const RootSignature* root, const ShaderCompiler* shader);
		ComputePipeline(ID3D12PipelineState* pipe);
		/* デストラクタ */
		~ComputePipeline();
	};
}
