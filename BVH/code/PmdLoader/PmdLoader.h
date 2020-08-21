#pragma once
#include <vector>

namespace pmd
{
	/* ヘッダー情報 */
	struct PmdHeader
	{
		union {
			struct {
				/* マジック定義 */
				char magic[3];
				/* バージョン */
				float version;
				/* モデル名 */
				char name[20];
				/* コメント */
				char comment[256];
			};
			/* バイトデータ */
			std::uint8_t byte[283];
		};
	};
	/* 頂点情報 */
	struct PmdVertexInfo
	{
		union {
			struct {
				/* 座標 */
				float pos[3];
				/* 法線ベクトル */
				float normal[3];
				/* UV座標 */
				float uv[2];
				/* ボーン番号 */
				std::uint16_t bone_index[2];
				/* ボーンの影響度(0～100) */
				std::uint8_t bone_weight;
				/* 輪郭線フラグ(0:通常、1:エッジ無効) */
				std::uint8_t edge_flag;
			};
			/* バイトデータ */
			std::uint8_t byte[38];
		};
	};
	/* 頂点リスト */
	struct PmdVertexList
	{
		/* 頂点数 */
		std::uint32_t num;
		/* 頂点情報 */
		std::vector<PmdVertexInfo>vertex;
	};
	/* インデックスリスト */
	struct PmdIndexList
	{
		/* インデックス数 */
		std::uint32_t num;
		/* インデックス(3個で1面) */
		std::vector<std::uint16_t>index;
	};
	/* マテリアル情報 */
	struct PmdMaterialInfo
	{
		union {
			struct {
				/* 減衰色 */
				float diffuse[3];
				/* 減衰色の不透明度 */
				float alpha;
				/* 鏡面反射率 */
				float specularity;
				/* 光沢色 */
				float specular[3];
				/* 環境色 */
				float ambient[3];
				/* トゥーンテクスチャの使用番号 */
				std::uint8_t toon_index;
				/* 輪郭線フラグ(0:通常、1:エッジ無効) */
				std::uint8_t edge_flag;
				/* 使用するインデックス数 */
				std::uint32_t index_num;
				/* テクスチャファイル名(*.sphで乗算 *.spaで加算) */
				char texture_name[20];
			};
			/* バイトデータ */
			std::uint8_t byte[70];
		};
	};
	/* マテリアルリスト */
	struct PmdMaterialList
	{
		/* マテリアル数 */
		std::uint32_t num;
		/* マテリアル情報 */
		std::vector<PmdMaterialInfo>material;
	};
	/* ボーン情報 */
	struct PmdBoneInfo
	{
		/* ボーン名 */
		char name[20];
		/* 親ボーン番号(NONE=0xFFFF) */
		std::uint16_t parent_index;
		/* 子ボーン番号(NONE=0) */
		std::uint16_t child_index;
		/* 種別 */
		std::uint8_t type;
		/* IKボーン番号(NONE=0) */
		std::uint16_t ik_index;
		/* ヘッド座標 */
		float pos[3];
	};
	/* ボーンリスト */
	struct PmdBoneList
	{
		/* ボーン数 */
		std::uint16_t num;
		/* ボーン情報 */
		std::vector<PmdBoneInfo>bone;
	};
}
