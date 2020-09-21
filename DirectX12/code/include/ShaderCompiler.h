#pragma once
#include "BaseSubObject.h"

namespace Dx12
{
	class SubObject;

	class ShaderCompiler :
		public BaseObject<ID3DBlob>, BaseSubObject<D3D12_DXIL_LIBRARY_DESC>
	{
	public:
		/*＊ シェーダのコンパイル
		 * @param file_path ファイルパス
		 * @param function エントリー関数名
		 * @param shader_model シェーダモデル
		 * @return シェーダ情報
		 */
		static ID3DBlob* CompileFromFile(const Str::String& file_path, const Str::String& entry_name, const Str::String& shader_model);

	public:
		/** コンストラクタ */
		ShaderCompiler();
		/** コンストラクタ
		 * @param file_path ファイルパス
		 * @param function エントリー関数名
		 * @param shader_model シェーダモデル
		 */
		ShaderCompiler(const Str::String& file_path, const Str::String& entry_name, const Str::String& shader_model);
		/** コンストラクタ
		 * @param blob シェーダ情報
		 */
		ShaderCompiler(ID3DBlob* blob);
		/** デストラクタ */
		~ShaderCompiler();

	public:
		/** サブオブジェクト情報のセット  
		 * @param func_name エクスポート関数名
		 */
		void SetConfig(const std::vector<Str::String>& func_name);
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 * @param func_name エクスポート関数名
		 */
		void AddSubObject(SubObject* sub, const std::vector<Str::String>& func_name);

	private:
		/* エクスポート名 */
		std::vector<D3D12_EXPORT_DESC>export_name;
	};
}
