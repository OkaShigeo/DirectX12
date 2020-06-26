#pragma once
#include "DxBaseObject.h"
#include <string>

class ShaderCompiler :
	public DxBaseObject<ID3DBlob>
{
public:
	/* �R���X�g���N�^ */
	ShaderCompiler(const std::wstring& file_path, const std::wstring& entry_func, const std::wstring& shader_model);
	/* �f�X�g���N�^ */
	~ShaderCompiler();
	
private:
	/* �V�F�[�_�̃R���p�C�� */
	std::uint32_t Compile(const std::wstring& file_path, const std::wstring& entry_func, const std::wstring& shader_model);
};
