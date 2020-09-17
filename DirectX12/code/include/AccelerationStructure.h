#pragma once
#include "BaseObject.h"
#include <tuple>
#include <vector>

namespace Dx12
{
	class Resource;

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
		static std::tuple<ID3D12Resource2*, ID3D12Resource2*, std::vector<D3D12_RAYTRACING_GEOMETRY_DESC>> CreateBottomLevel(const std::vector<Resource*>& vertex, const std::vector<std::uint64_t>& vertex_num, const std::uint64_t& transform_matrix_addr = 0, const std::vector<Resource*>& index = {}, const std::vector<std::uint64_t>& index_num = {});
		/** トップレベルの生成 */
		static std::tuple<ID3D12Resource2*, ID3D12Resource2*, ID3D12Resource2*> CreateTopLevel(const std::uint64_t& instance_num = 1);

	public:
		/** コンストラクタ */
		AccelerationStructure();
		/** コンストラクタ */
		AccelerationStructure(const std::tuple<ID3D12Resource2*, ID3D12Resource2*, std::vector<D3D12_RAYTRACING_GEOMETRY_DESC>>& value);
		/** デストラクタ */
		~AccelerationStructure();

	public:
		/*＊ メモリの解放 */
		void Release(void);
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
		/** ビルド情報の取得
		 * @return ビルド情報
		 */
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS GetBuildInfo(void) const;

		/*----------オペレータオーバーロード関数----------*/
	public:
		void operator=(const std::tuple<ID3D12Resource2*, ID3D12Resource2*, std::vector<D3D12_RAYTRACING_GEOMETRY_DESC>>& value)
		{
			Release();

			scratch   = std::get<0>(value);
			result    = std::get<1>(value);
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
			result   = std::get<1>(value);
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
		/* リザルトリソース */
		ID3D12Resource2* result{ nullptr };
		/* インスタンスリソース */
		ID3D12Resource2* instance{ nullptr };
		/* ジオメトリ情報 */
		std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> geometory;
		/* ビルド入力情報 */
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	};
}
