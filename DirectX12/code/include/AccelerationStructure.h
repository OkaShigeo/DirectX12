#pragma once
#include "Resource.h"
#include <tuple>

namespace Dx12
{
	class VertexBuffer;
	class IndexBuffer;

	class AccelerationStructure : 
		public Resource
	{
	public:
		/*＊ ボトムレベルの生成 
		 * @param vertex 頂点バッファ
		 * @param index インデックスバッファ
		 * @param transform_matrix 頂点変換行列の仮想アドレス
		 */
		static std::tuple<ID3D12Resource2*, ID3D12Resource2*, std::vector<D3D12_RAYTRACING_GEOMETRY_DESC>> CreateBottomLevel(const std::vector<VertexBuffer*>& vertex, const std::vector<IndexBuffer*>& index = {}, const std::uint64_t& transform_matrix_addr = 0);
		/** トップレベルの生成 
		 * @instance_num インスタンス数
		 */
		static std::tuple<ID3D12Resource2*, ID3D12Resource2*, ID3D12Resource2*> CreateTopLevel(const std::uint64_t& instance_num = 1);

	public:
		/** コンストラクタ 
		 * @param vertex 頂点バッファ
		 * @param index インデックスバッファ
		 * @param transform_matrix 頂点変換行列の仮想アドレス
		 */
		AccelerationStructure(const std::vector<VertexBuffer*>& vertex, const std::vector<IndexBuffer*>& index = {}, const std::uint64_t& transform_matrix_addr = 0);
		/** コンストラクタ
		 * @instance_num インスタンス数
		 */
		AccelerationStructure(const std::uint64_t& instance_num = 1);
		/** コンストラクタ 
		 * @param value 加速構造の値
		 */
		template<typename T>
		AccelerationStructure(const std::tuple<ID3D12Resource2*, ID3D12Resource2*, T>& value);
		/** デストラクタ */
		~AccelerationStructure();

	public:
		/*＊ メモリの解放 */
		void Release(void);
		/*＊ 確保したバッファの解放 */
		void ReleaseBuffer(void) const;
		/** ボトムレベルの確認
		 * @return true:ボトムレベル / false:その他
		 */
		bool IsBottomLevel(void) const;
		/** トップレベルの確認
		 * @return true:トップレベル / false:その他
		 */
		bool IsTopLevel(void) const;

	public:
		/** スクラッチリソースの取得
		 * @return スクラッチリソース
		 */
		ID3D12Resource2* GetScratch(void) const;
		/** リザルトリソースの取得
		 * @return リザルトリソース
		 */
		ID3D12Resource2* GetResult(void) const;
		/** インスタンスリソースの取得
		 * @return インスタンスリソース
		 */
		ID3D12Resource2* GetInstance(void) const;
		/** インスタンス数の取得
		 * @return インスタンス数
		 */
		std::uint64_t GetInstanceNum(void) const;
		/** ビルド情報の取得
		 * @return ビルド情報
		 */
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS GetBuildInput(void) const;
		/** 確保したバッファの取得
		 * @return 確保したバッファーのポインタ
		 */
		D3D12_RAYTRACING_INSTANCE_DESC* GetBuffer(void) const;

		/*----------オペレータオーバーロード関数----------*/
	public:
		void operator=(const std::tuple<ID3D12Resource2*, ID3D12Resource2*, std::vector<D3D12_RAYTRACING_GEOMETRY_DESC>>& value)
		{
			Release();

			scratch   = std::get<0>(value);
			obj       = std::get<1>(value);
			geometory = std::get<2>(value);

			input.DescsLayout    = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
			input.Flags          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
			input.NumDescs       = std::uint32_t(geometory.size());
			input.pGeometryDescs = geometory.data();
			input.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
		}
		void operator=(const std::tuple<ID3D12Resource2*, ID3D12Resource2*, ID3D12Resource2*>& value)
		{
			scratch  = std::get<0>(value);
			obj      = std::get<1>(value);
			instance = std::get<2>(value);

			input.DescsLayout   = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
			input.Flags         = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
			input.NumDescs      = std::uint32_t(instance->GetDesc().Width / sizeof(D3D12_RAYTRACING_INSTANCE_DESC));
			input.InstanceDescs = instance->GetGPUVirtualAddress();
			input.Type          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
		}

	private:
		/* スクラッチリソース */
		ID3D12Resource2* scratch{ nullptr };
		/* インスタンスリソース */
		ID3D12Resource2* instance{ nullptr };
		/* ビルド入力情報 */
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
		/* ジオメトリ情報 */
		std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> geometory;
	};
}
