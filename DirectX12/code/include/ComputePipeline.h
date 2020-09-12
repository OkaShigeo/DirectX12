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
		/** �p�C�v���C���̐��� 
		 * @param root ���[�g�V�O�l�`��
		 * @param shader �V�F�[�_���
		 * @return �p�C�v���C��
		 */
		static ID3D12PipelineState* CreatePipeline(const RootSignature* root, const ShaderCompiler* shader);

	public:
		/** �R���X�g���N�^ */
		ComputePipeline();
		/** �R���X�g���N�^ 
		 * @param ���[�g�V�O�l�`��
		 * @param shader �V�F�[�_���
		 */
		ComputePipeline(const RootSignature* root, const ShaderCompiler* shader);
		/** �R���X�g���N�^
		 * @param pipe �p�C�v���C��
		 */
		ComputePipeline(ID3D12PipelineState* pipe);
		/*�� �f�X�g���N�^ */
		~ComputePipeline();
	};
}
