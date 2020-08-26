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
		/* �V�F�[�_�̃R���p�C�� */
		static ID3DBlob* Compile(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);

	public:
		/* �R���X�g���N�^ */
		ShaderCompiler();
		ShaderCompiler(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);
		ShaderCompiler(ID3DBlob* blob);
		/* �f�X�g���N�^ */
		~ShaderCompiler();

	public:
		/* �T�u�I�u�W�F�N�g�̒ǉ� */
		void AddSubObj(const std::vector<std::wstring>& func_name);
	};
}
