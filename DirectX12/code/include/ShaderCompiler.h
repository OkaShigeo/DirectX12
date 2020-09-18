#pragma once
#include "BaseObject.h"
#include "String.h"

namespace Dx12
{
	class SubObject;

	class ShaderCompiler :
		public BaseObject<ID3DBlob>
	{
	public:
		/*�� �V�F�[�_�̃R���p�C��
		 * @param file_path �t�@�C���p�X
		 * @param function �G���g���[�֐���
		 * @param shader_model �V�F�[�_���f��
		 * @return �V�F�[�_���
		 */
		static ID3DBlob* CompileFromFile(const std::string& file_path, const std::string& entry_name, const std::string& shader_model);

	public:
		/** �R���X�g���N�^ */
		ShaderCompiler();
		/** �R���X�g���N�^
		 * @param file_path �t�@�C���p�X
		 * @param function �G���g���[�֐���
		 * @param shader_model �V�F�[�_���f��
		 */
		ShaderCompiler(const std::string& file_path, const std::string& entry_name, const std::string& shader_model);
		/** �R���X�g���N�^
		 * @param blob �V�F�[�_���
		 */
		ShaderCompiler(ID3DBlob* blob);
		/** �f�X�g���N�^ */
		~ShaderCompiler();

	public:
		/** �T�u�I�u�W�F�N�g�̒ǉ� 
		 * @param sub �T�u�I�u�W�F�N�g�̒ǉ���
		 * @param �֐���
		 */
		void AddSubObject(SubObject* sub, const std::vector<std::string>& func_name);

	private:
		/* �G�N�X�|�[�g�� */
		std::vector<D3D12_EXPORT_DESC>export_name;
		/* �V�F�[�_��� */
		D3D12_DXIL_LIBRARY_DESC dxil_info{};
	};
}
