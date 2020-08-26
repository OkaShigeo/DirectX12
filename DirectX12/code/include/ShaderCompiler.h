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
		/* シェーダのコンパイル */
		static ID3DBlob* Compile(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);

	public:
		/* コンストラクタ */
		ShaderCompiler();
		ShaderCompiler(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);
		ShaderCompiler(ID3DBlob* blob);
		/* デストラクタ */
		~ShaderCompiler();

	public:
		/* サブオブジェクトの追加 */
		void AddSubObj(const std::vector<std::wstring>& func_name);
	};
}
