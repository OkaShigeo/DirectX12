#pragma once
#include "Resource.h"
#include <tuple>

namespace Dx12
{
	class AccelerationStructure
	{
	public:
		/*＊ ボトムレベルの生成 
		 * @param vertex 頂点リソース
		 * @param vertex_num 頂点数
		 * @param transform_matrix 頂点変換行列の仮想アドレス
		 * @param index インデックスリソース
		 * @param index_num インデックス数
		 */
		static std::tuple<ID3D12Resource2*, ID3D12Resource2*> CreateBottomLevel(const std::vector<Resource*>& vertex, const std::vector<std::uint64_t>& vertex_num, const std::uint64_t& transform_matrix_addr = 0, const std::vector<Resource*>& index = {}, const std::vector<std::uint64_t>& index_num = {});

	public:
		/** コンストラクタ */
		AccelerationStructure();
		/** コンストラクタ */
		AccelerationStructure(const std::tuple<ID3D12Resource2*, ID3D12Resource2*>& rsc);
		/** デストラクタ */
		~AccelerationStructure();

	public:
		/*＊ メモリの解放 */
		void Release(void);

		/*----------オペレータオーバーロード関数----------*/
	public:
		void operator=(const std::tuple<ID3D12Resource2*, ID3D12Resource2*>& rsc)
		{
			Release();

			scratch = std::get<0>(rsc);
			result  = std::get<1>(rsc);
		}

	private:
		/* スクラッチリソース */
		ID3D12Resource2* scratch{ nullptr };
		/* リザルトリソース */
		ID3D12Resource2* result{ nullptr };
		/* インスタンスリソース */
		ID3D12Resource2* instance{ nullptr };
	};
}
