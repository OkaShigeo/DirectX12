#pragma once
#include "BaseObject.h"
#include <string>

namespace Dx12
{
	class ShaderCompiler :
		public BaseObject<ID3DBlob>
	{
	public:
		/* �R���X�g���N�^ */
		ShaderCompiler(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);
		ShaderCompiler(ID3DBlob* blob);
		/* �f�X�g���N�^ */
		~ShaderCompiler();

	private:
		/* �V�F�[�_�̃R���p�C�� */
		ID3DBlob* Compile(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);
	};
}
