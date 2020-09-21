#pragma once
#include "BaseSubObject.h"

namespace Dx12
{
	class PipelineConfig :
		public BaseSubObject<D3D12_RAYTRACING_PIPELINE_CONFIG1>
	{
	public:
		/** コンストラクタ */
		PipelineConfig();
		/** コンストラクタ 
		 * @param max_depth 各グリッドでTraceRay()呼び出し回数
		 * @param flag パイプラインフラグ
		 */
		PipelineConfig(const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag = D3D12_RAYTRACING_PIPELINE_FLAGS::D3D12_RAYTRACING_PIPELINE_FLAG_NONE);
		/** コンストラクタ
		 * @param sub 追加先サブオブジェクト
		 * @param max_depth 各グリッドでTraceRay()呼び出し回数
		 * @param flag パイプラインフラグ
		 */
		PipelineConfig(SubObject* sub, const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag = D3D12_RAYTRACING_PIPELINE_FLAGS::D3D12_RAYTRACING_PIPELINE_FLAG_NONE);
		/** デストラクタ */
		~PipelineConfig();

	public:
		/** サブオブジェクト情報のセット
		 * @param max_depth 各グリッドでTraceRay()呼び出し回数
		 * @param flag パイプラインフラグ
		 */
		void SetConfig(const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag = D3D12_RAYTRACING_PIPELINE_FLAGS::D3D12_RAYTRACING_PIPELINE_FLAG_NONE);
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 * @param max_depth 各グリッドでTraceRay()呼び出し回数
		 * @param flag パイプラインフラグ
		 */
		void AddSubObject(SubObject* sub, const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag = D3D12_RAYTRACING_PIPELINE_FLAGS::D3D12_RAYTRACING_PIPELINE_FLAG_NONE);
	};
}
