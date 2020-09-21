#pragma once
#include "Resource.h"

namespace Dx12
{
	class VertexBuffer :
		public Resource
	{
	public:
		/** 頂点バッファの生成 
		 * @param vertex 頂点データ
		 */
		static ID3D12Resource2* CreateVertexBuffer(void* vertex);

	public:
		/** コンストラクタ */
		VertexBuffer();
		/** コンストラクタ 
		 * @param vertex 頂点データ
		 * @param vertex_num 頂点数
		 */
		VertexBuffer(void* vertex, const std::uint64_t& vertex_num);
		/** コンストラクタ
		 * @param resource リソース
		 */
		VertexBuffer(ID3D12Resource2* resource);
		/** デストラクタ */
		~VertexBuffer();

	public:
		/** 頂点の更新  
		 * @param vertex 頂点データ
		 */
		void UpdateVertex(const void* vertex) const;

	public:
		/** 頂点数の取得 
		 * @return 頂点数
		 */
		std::uint64_t GetNum(void) const;
		/** 頂点数のセット
		 * @param vertex_num 頂点数
		 */
		void SetNum(const std::uint64_t& vertex_num);

	private:
		/* 頂点数 */
		std::uint64_t num{ 0 };
	};
}
