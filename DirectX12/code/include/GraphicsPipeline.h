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
		/*�� �p�C�v���C���̐��� 
		 * @param input ���͗v�f�̏��
		 * @param root ���[�g�V�O�l�`��
		 * @param shader �V�F�[�_���
		 * @param vertex ���_�V�F�[�_���
		 * @param pixel �s�N�Z���V�F�[�_���
		 * @param geometory �W�I���g���[�V�F�[�_���
		 * @param topology �v���~�e�B�u�̃g�|���W�[�̎��
		 * @param depth �[�x�l�Z�b�g�̃t���O
		 * @return �p�C�v���C��
		 */
		static ID3D12PipelineState* CreatePipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const RootSignature* root, const ShaderCompiler* vertex, const ShaderCompiler* pixel, const ShaderCompiler* geometory, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology, const bool& depth);

	public:
		/*�� �R���X�g���N�^ */
		GraphicsPipeline();
		/** �R���X�g���N�^
		 * @param input ���͗v�f�̏��
		 * @param root ���[�g�V�O�l�`��
		 * @param shader �V�F�[�_���
		 * @param vertex ���_�V�F�[�_���
		 * @param pixel �s�N�Z���V�F�[�_���
		 * @param geometory �W�I���g���[�V�F�[�_���
		 * @param topology �v���~�e�B�u�̃g�|���W�[�̎��
		 * @param depth �[�x�l�Z�b�g�̃t���O
		 */
		GraphicsPipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const RootSignature* root, const ShaderCompiler* vertex, const ShaderCompiler* pixel, const ShaderCompiler* geometory = nullptr, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, const bool& depth = false);
		/** �R���X�g���N�^
		 * @param �p�C�v���C��
		 */
		GraphicsPipeline(ID3D12PipelineState* pipe);
		/*�� �f�X�g���N�^ */
		~GraphicsPipeline();
	};
}
