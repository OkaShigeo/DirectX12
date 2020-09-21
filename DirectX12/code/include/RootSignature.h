#pragma once
#include "BaseObject.h"
#include "BaseSubObject.h"

namespace Dx12
{
	class ShaderCompiler;

	class RootSignature :
		public BaseObject<ID3D12RootSignature>, BaseSubObject<D3D12_STATE_SUBOBJECT>
	{
	public:
		/** �f�B�X�N���v�^�͈͂̎擾
		 * @param type �f�B�X�N���v�^�͈̓^�C�v
		 * @param index ���W�X�^�[�ԍ�
		 * @return �f�B�X�N���v�^�͈�
		 * @param descriptor_num �o�C���h�ł��鐔
		 * @param space ���W�X�^�[���
		 */
		static D3D12_DESCRIPTOR_RANGE1 GetRange(const D3D12_DESCRIPTOR_RANGE_TYPE& type, const std::uint32_t& index, const std::uint32_t& descriptor_num = 1, const std::uint32_t& space = 0);
		/* �p�����[�^�̎擾
		 * @param type �p�����[�^�^�C�v
		 * @param index ���W�X�^�[�ԍ�
		 * @param range �f�B�X�N���v�^�͈͂̔z��
		 * @param �萔�l
		 * @param space ���W�X�^�[���
		 * @return ���[�g�V�O�l�`���p�����[�^
		 */
		static D3D12_ROOT_PARAMETER1 GetParam(const D3D12_ROOT_PARAMETER_TYPE& type, const std::uint32_t& index, const std::vector<D3D12_DESCRIPTOR_RANGE1>& ranges = {}, const std::uint32_t& value = 0, const std::uint32_t& space = 0);
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
		static ID3D12RootSignature* CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler = {});

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
		/** �T�u�I�u�W�F�N�g���̃Z�b�g
		 * @param type �T�u�I�u�W�F�N�g�^�C�v
		 */
		void SetConfig(const D3D12_STATE_SUBOBJECT_TYPE& type);
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param type �T�u�I�u�W�F�N�g�^�C�v
		 */
		void AddSubObject(SubObject* sub, const D3D12_STATE_SUBOBJECT_TYPE& type);
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param type �T�u�I�u�W�F�N�g�^�C�v
		 * @param func_name �֘A�t���֐���
		 */
		void AddSubObject(SubObject* sub, const D3D12_STATE_SUBOBJECT_TYPE& type, const std::vector<Str::String>& func_name);
	};
}
