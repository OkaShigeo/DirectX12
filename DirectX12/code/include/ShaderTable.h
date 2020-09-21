#pragma once
#include "Resource.h"

namespace Dx12
{
	class ShaderTable :
		public Resource
	{
	public:
		/** シェーダテーブルリソースの生成
		 * @param recode_num シェーダレコード数
		 * @return リソース
		 */
		static ID3D12Resource2* CreateShaderTableResource(const std::uint64_t& recode_num);
		/** シェーダレコードサイズの取得
		 * @return シェーダレコードサイズ
		 */
		static std::uint64_t GetShaderRecodeSize(void);

	public:
		/*＊ コンストラクタ */
		ShaderTable();
		/** コンストラクタ
		 * @param recode_num シェーダレコード数
		 */
		ShaderTable(const std::uint64_t& recode_num);
		/** コンストラクタ 
		 * @param resource リソース
		 */
		ShaderTable(ID3D12Resource2* resource);
		/** デストラクタ */
		~ShaderTable();
	};
}
