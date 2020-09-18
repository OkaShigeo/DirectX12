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
		/*＊ シェーダのコンパイル
		 * @param file_path ファイルパス
		 * @param function エントリー関数名
		 * @param shader_model シェーダモデル
		 * @return シェーダ情報
		 */
		static ID3DBlob* CompileFromFile(const std::string& file_path, const std::string& entry_name, const std::string& shader_model);

	public:
		/** コンストラクタ */
		ShaderCompiler();
		/** コンストラクタ
		 * @param file_path ファイルパス
		 * @param function エントリー関数名
		 * @param shader_model シェーダモデル
		 */
		ShaderCompiler(const std::string& file_path, const std::string& entry_name, const std::string& shader_model);
		/** コンストラクタ
		 * @param blob シェーダ情報
		 */
		ShaderCompiler(ID3DBlob* blob);
		/** デストラクタ */
		~ShaderCompiler();

	public:
		/** サブオブジェクトの追加 
		 * @param sub サブオブジェクトの追加先
		 * @param 関数名
		 */
		void AddSubObject(SubObject* sub, const std::vector<std::string>& func_name);

	private:
		/* エクスポート名 */
		std::vector<D3D12_EXPORT_DESC>export_name;
		/* シェーダ情報 */
		D3D12_DXIL_LIBRARY_DESC dxil_info{};
	};
}
