#pragma once
#include "BaseSubObject.h"

namespace Dx12
{
	class ShaderConfig :
		public BaseSubObject<D3D12_RAYTRACING_SHADER_CONFIG>
	{
	public:
		/** コンストラクタ */
		ShaderConfig();
		/** コンストラクタ
		 * @param payload ペイロードサイズ
		 * @param attributes ヒット詳細データサイズ
		 */
		ShaderConfig(const std::uint64_t& payload, const std::uint64_t& attributes = sizeof(float) * 2);
		/** コンストラクタ
		 * @param sub 追加先サブオブジェクト
		 * @param payload ペイロードサイズ
		 * @param attributes ヒット詳細データサイズ
		 */
		ShaderConfig(SubObject* sub, const std::uint64_t& payload, const std::uint64_t& attributes = sizeof(float) * 2);
		/** コンストラクタ
		 * @param sub 追加先サブオブジェクト
		 * @param payload ペイロードサイズ
		 * @param func_name 関連付け関数名
		 * @param attributes ヒット詳細データサイズ
		 */
		ShaderConfig(SubObject* sub, const std::uint64_t& payload, const std::vector<Str::String>& func_name, const std::uint64_t& attributes = sizeof(float) * 2);
		/** デストラクタ */
		~ShaderConfig();

	public:
		/** サブオブジェクト情報のセット
		 * @param payload ペイロードサイズ
		 * @param attributes ヒット詳細データサイズ
		 */
		void SetConfig(const std::uint64_t& payload, const std::uint64_t& attributes = sizeof(float) * 2);
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 * @param payload ペイロードサイズ
		 * @param attributes ヒット詳細データサイズ
		 */
		void AddSubObject(SubObject* sub, const std::uint64_t& payload, const std::uint64_t& attributes = sizeof(float) * 2);
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 * @param payload ペイロードサイズ
		 * @param func_name 関連付け関数名
		 * @param attributes ヒット詳細データサイズ
		 */
		void AddSubObject(SubObject* sub, const std::uint64_t& payload, const std::vector<Str::String>& func_name, const std::uint64_t& attributes = sizeof(float) * 2);
	}; 
}
