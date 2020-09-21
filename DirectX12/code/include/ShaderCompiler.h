#pragma once
#include "BaseSubObject.h"

namespace Dx12
{
	class SubObject;

	class ShaderCompiler :
		public BaseObject<ID3DBlob>, BaseSubObject<D3D12_DXIL_LIBRARY_DESC>
	{
	public:
		/*�� �V�F�[�_�̃R���p�C��
		 * @param file_path �t�@�C���p�X
		 * @param function �G���g���[�֐���
		 * @param shader_model �V�F�[�_���f��
		 * @return �V�F�[�_���
		 */
		static ID3DBlob* CompileFromFile(const Str::String& file_path, const Str::String& entry_name, const Str::String& shader_model);

	public:
		/** �R���X�g���N�^ */
		ShaderCompiler();
		/** �R���X�g���N�^
		 * @param file_path �t�@�C���p�X
		 * @param function �G���g���[�֐���
		 * @param shader_model �V�F�[�_���f��
		 */
		ShaderCompiler(const Str::String& file_path, const Str::String& entry_name, const Str::String& shader_model);
		/** �R���X�g���N�^
		 * @param blob �V�F�[�_���
		 */
		ShaderCompiler(ID3DBlob* blob);
		/** �f�X�g���N�^ */
		~ShaderCompiler();

	public:
		/** �T�u�I�u�W�F�N�g���̃Z�b�g  
		 * @param func_name �G�N�X�|�[�g�֐���
		 */
		void SetConfig(const std::vector<Str::String>& func_name);
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param func_name �G�N�X�|�[�g�֐���
		 */
		void AddSubObject(SubObject* sub, const std::vector<Str::String>& func_name);

	private:
		/* �G�N�X�|�[�g�� */
		std::vector<D3D12_EXPORT_DESC>export_name;
	};
}
