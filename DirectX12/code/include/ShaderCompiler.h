#pragma once
#include "BaseObject.h"
#include "SubObject.h"
#include <string>
#include <vector>

namespace Dx12
{
	class ShaderCompiler :
		public BaseObject<ID3DBlob>, SubObject
	{
	public:
		/*�� �V�F�[�_�̃R���p�C��
		 * @param file_path �t�@�C���p�X
		 * @param function �G���g���[�֐���
		 * @param shader_model �V�F�[�_���f��
		 * @return �V�F�[�_���
		 */
		static ID3DBlob* CompileFromFile(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);

	public:
		/** �R���X�g���N�^ */
		ShaderCompiler();
		/** �R���X�g���N�^
		 * @param file_path �t�@�C���p�X
		 * @param function �G���g���[�֐���
		 * @param shader_model �V�F�[�_���f��
		 */
		ShaderCompiler(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);
		/** �R���X�g���N�^
		 * @param blob �V�F�[�_���
		 */
		ShaderCompiler(ID3DBlob* blob);
		/** �f�X�g���N�^ */
		~ShaderCompiler();

	public:
		/** �T�u�I�u�W�F�N�g�̒ǉ� 
		 * @param �֐���
		 */
		void AddSubObj(const std::vector<std::wstring>& func_name);
	};
}
