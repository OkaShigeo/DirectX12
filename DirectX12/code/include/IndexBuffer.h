#pragma once
#include "Resource.h"

namespace Dx12
{
	class IndexBuffer :
		public Resource
	{
	public:
		/** インデックスバッファの生成
		 * @param index インデックスデータ
		 */
		static ID3D12Resource2* CreateIndexBuffer(void* index);

	public:
		/** コンストラクタ */
		IndexBuffer();
		/** コンストラクタ
		 * @param index インデックスデータ
		 * @param index_num インデックス数
		 */
		IndexBuffer(void* index, const std::uint64_t& index_num);
		/** コンストラクタ
		 * @param resource リソース
		 */
		IndexBuffer(ID3D12Resource2* resource);
		/** デストラクタ */
		~IndexBuffer();

	public:
		/** インデックスの更新 
		 * @param index インデックスデータ
		 */
		void UpdateIndex(const void* index) const;

	public:
		/** インデックス数の取得 
		 * @return インデックス数
		 */
		std::uint64_t GetNum(void) const;
		/** インデックス数のセット
		 * @param index_num インデックス数
		 */
		void SetNum(const std::uint64_t& index_num);

	private:
		/* インデックス数 */
		std::uint64_t num{ 0 };
	};
}
