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
		/* コンストラクタ */
		ComputePipeline(const RootSignature* root, const ShaderCompiler* shader);
		ComputePipeline(ID3D12PipelineState* pipe);
		/* デストラクタ */
		~ComputePipeline();

	private:
		/* パイプラインの生成 */
		ID3D12PipelineState* CreatePipeline(const RootSignature* root, const ShaderCompiler* shader);
	};
}
