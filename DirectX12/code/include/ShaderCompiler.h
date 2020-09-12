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
		/*＊ シェーダのコンパイル
		 * @param file_path ファイルパス
		 * @param function エントリー関数名
		 * @param shader_model シェーダモデル
		 * @return シェーダ情報
		 */
		static ID3DBlob* CompileFromFile(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);

	public:
		/** コンストラクタ */
		ShaderCompiler();
		/** コンストラクタ
		 * @param file_path ファイルパス
		 * @param function エントリー関数名
		 * @param shader_model シェーダモデル
		 */
		ShaderCompiler(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model);
		/** コンストラクタ
		 * @param blob シェーダ情報
		 */
		ShaderCompiler(ID3DBlob* blob);
		/** デストラクタ */
		~ShaderCompiler();

	public:
		/** サブオブジェクトの追加 
		 * @param 関数名
		 */
		void AddSubObj(const std::vector<std::wstring>& func_name);
	};
}
