#pragma once
#include "DxBaseObject.h"
#include <string>

class ShaderCompiler :
	public DxBaseObject<ID3DBlob>
{
public:
	/* コンストラクタ */
	ShaderCompiler(const std::wstring& file_path, const std::wstring& entry_func, const std::wstring& shader_model);
	/* デストラクタ */
	~ShaderCompiler();
	
private:
	/* シェーダのコンパイル */
	std::uint32_t Compile(const std::wstring& file_path, const std::wstring& entry_func, const std::wstring& shader_model);
};
