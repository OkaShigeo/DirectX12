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
		/** パイプラインの生成 
		 * @param root ルートシグネチャ
		 * @param shader シェーダ情報
		 * @return パイプライン
		 */
		static ID3D12PipelineState* CreatePipeline(const RootSignature* root, const ShaderCompiler* shader);

	public:
		/** コンストラクタ */
		ComputePipeline();
		/** コンストラクタ 
		 * @param ルートシグネチャ
		 * @param shader シェーダ情報
		 */
		ComputePipeline(const RootSignature* root, const ShaderCompiler* shader);
		/** コンストラクタ
		 * @param pipe パイプライン
		 */
		ComputePipeline(ID3D12PipelineState* pipe);
		/*＊ デストラクタ */
		~ComputePipeline();
	};
}
