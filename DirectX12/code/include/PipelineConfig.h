#pragma once
#include "BaseSubObject.h"

namespace Dx12
{
	class PipelineConfig :
		public BaseSubObject<D3D12_RAYTRACING_PIPELINE_CONFIG1>
	{
	public:
		/** �R���X�g���N�^ */
		PipelineConfig();
		/** �R���X�g���N�^ 
		 * @param max_depth �e�O���b�h��TraceRay()�Ăяo����
		 * @param flag �p�C�v���C���t���O
		 */
		PipelineConfig(const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag = D3D12_RAYTRACING_PIPELINE_FLAGS::D3D12_RAYTRACING_PIPELINE_FLAG_NONE);
		/** �R���X�g���N�^
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param max_depth �e�O���b�h��TraceRay()�Ăяo����
		 * @param flag �p�C�v���C���t���O
		 */
		PipelineConfig(SubObject* sub, const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag = D3D12_RAYTRACING_PIPELINE_FLAGS::D3D12_RAYTRACING_PIPELINE_FLAG_NONE);
		/** �f�X�g���N�^ */
		~PipelineConfig();

	public:
		/** �T�u�I�u�W�F�N�g���̃Z�b�g
		 * @param max_depth �e�O���b�h��TraceRay()�Ăяo����
		 * @param flag �p�C�v���C���t���O
		 */
		void SetConfig(const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag = D3D12_RAYTRACING_PIPELINE_FLAGS::D3D12_RAYTRACING_PIPELINE_FLAG_NONE);
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param max_depth �e�O���b�h��TraceRay()�Ăяo����
		 * @param flag �p�C�v���C���t���O
		 */
		void AddSubObject(SubObject* sub, const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag = D3D12_RAYTRACING_PIPELINE_FLAGS::D3D12_RAYTRACING_PIPELINE_FLAG_NONE);
	};
}
