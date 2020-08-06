#pragma once
#include "BaseObject.h"
#include <string>

namespace Dx12
{
	class ShaderCompiler :
		public BaseObject<ID3DBlob>
	{
	public:
		/* コンストラクタ */
		ShaderCompiler(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);
		ShaderCompiler(ID3DBlob* blob);
		/* デストラクタ */
		~ShaderCompiler();

	private:
		/* シェーダのコンパイル */
		ID3DBlob* Compile(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);
	};
}
