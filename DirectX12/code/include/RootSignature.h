#pragma once
#include "BaseObject.h"
#include "SubObject.h"
#include <string>
#include <vector>

namespace Dx12
{
	class ShaderCompiler;

	class RootSignature :
		public BaseObject<ID3D12RootSignature>, SubObject
	{
	public:
		/** ���[�g�V�O�l�`���̐��� 
		 * @param shader �V�F�[�_���
		 * @return ���[�g�V�O�l�`��
		 */
		static ID3D12RootSignature* CreateRootSignature(const ShaderCompiler* shader);
		/** ���[�g�V�O�l�`���̐��� 
		 * @param flag ���[�g�V�O�l�`���̃t���O
		 * @param param ���[�g�V�O�l�`���̃p�����[�^
		 * @param sampler �T���v���[
		 * @return ���[�g�V�O�l�`��
		 */
		static ID3D12RootSignature* CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param,
			const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler);

	public:
		/** �R���X�g���N�^ */
		RootSignature();
		/** �R���X�g���N�^
		 * @param shader �V�F�[�_���
		 */
		RootSignature(const ShaderCompiler* shader);
		/** �R���X�g���N�^
		 * @param flag ���[�g�V�O�l�`���̃t���O
		 * @param param ���[�g�V�O�l�`���̃p�����[�^
		 * @param sampler �T���v���[
		 */
		RootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler = {});
		/** �R���X�g���N�^
		 * @param root ���[�g�V�O�l�`��
		 */
		RootSignature(ID3D12RootSignature* root);
		/** �f�X�g���N�^ */
		~RootSignature();

	public:
		/** �T�u�I�u�W�F�N�g�̒ǉ�
		 * @param type �T�u�I�u�W�F�N�g�̎��
		 * @param func_name �֐���
		 */
		void AddSubObj(const D3D12_STATE_SUBOBJECT_TYPE& type, const std::vector<std::wstring>& func_name);

	private:
		/* �T�u�I�u�W�F�N�g�̊֘A�t�� */
		D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association{};
	};
}
