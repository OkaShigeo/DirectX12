#include <DxLib.h>
#include "PmdLoader/PmdLoader.h"
#include <fstream>
#include <algorithm>
#include <random>

namespace {
	/* ウィンドウXサイズ */
	const std::uint32_t window_x = 640;
	/* ウィンドウYサイズ */
	const std::uint32_t window_y = 480;
	/* カメラの視点 */
	const DxLib::FLOAT3 camera = { 0.0f, 10.0f, -20.0f };
	/* カメラの焦点 */
	const DxLib::FLOAT3 target = { 0.0f, 10.0f, 0.0f };

	/* AABB */
	struct AABB
	{
		/* コンストラクタ */
		AABB()
		{
			for (std::uint32_t i = 0; i < 3; ++i) {
				min[i] =  float(WINT_MAX);
				max[i] = -float(WINT_MAX);
			}
		}

		/* 最小位置 */
		float min[3];
		/* 最大位置 */
		float max[3];

		/* 中心座標の取得 */
		float GetCenter(const std::uint32_t& axis) const
		{
			return (min[axis] + max[axis]) / 2.0f;
		}
		/* 直方体の面積の取得 */
		float GetArea(void) const
		{
			return 2.0f * std::pow((max[0] - min[0]), 2.0f) + std::pow((max[1] - min[1]), 2.0f) + std::pow((max[2] - min[2]), 2.0f);
		}
		/* AABBのマージ */
		static AABB MergeAABB(const AABB& box1, const AABB& box2)
		{
			AABB aabb{};
			for (std::uint32_t i = 0; i < 3; ++i) {
				aabb.min[i] = std::fmin(box1.min[i], box2.min[i]);
				aabb.max[i] = std::fmax(box1.max[i], box2.max[i]);
			}

			return aabb;
		}
	};
	/* BVHノード */
	struct BVH_NODE
	{
		/* AABB */
		AABB aabb;
		/* 左ノード番号 */
		std::int32_t left{ 0 };
		/* 右ノード番号 */
		std::int32_t right{ 0 };
	};
	/* BVH */
	struct BVH
	{
		/* ノード */
		std::vector<BVH_NODE>node;

		/* 構築 */
		void Construction(const std::vector<AABB>& aabb, const bool& reset = true)
		{
			static std::uint32_t node_index = 0;
			if (reset == true) {
				node_index = 1;
			}

			BVH_NODE node{};
			for (std::uint32_t i = 0; i < aabb.size(); ++i) {
				node.aabb = AABB::MergeAABB(node.aabb, aabb[i]);
			}
			
			/* SAHの値 */
			float sah = float(aabb.size());
			/* 分割軸 */
			std::int32_t axis = -1;
			/* 分割数 */
			std::int32_t sprite = -1;

			/* ソート結果 */
			std::vector<AABB>sort = aabb;

			for (std::uint32_t i = 0; i < 3; ++i) {
				/* 各軸の中心座標でソート */
				std::sort(sort.begin(), sort.end(), [i](const AABB& a, const AABB& b) {
					return a.GetCenter(i) < b.GetCenter(i);
				});

				AABB tmp = {};
				std::vector<float>area1;
				for (std::uint32_t n = 0; n < aabb.size(); ++n) {
					tmp = AABB::MergeAABB(tmp, sort[n]);
					area1.push_back(std::abs(tmp.GetArea()));
				}
				
				tmp = {};
				std::vector<float>area2;
				for (std::int32_t n = std::int32_t(sort.size() - 1); n >= 0; --n) {
					tmp = AABB::MergeAABB(tmp, sort[n]);
					area2.push_back(std::abs(tmp.GetArea()));

					/* SAHの算出 */
					float cost = 2.0f + (area1[n] * n + (*area2.rbegin()) * (sort.size() - n)) / node.aabb.GetArea();
					if (sah > cost) {
						sah    = cost;
						axis   = i;
						sprite = n;
					}
				}
			}

			if (axis != -1) {
				std::vector<AABB>sort = aabb;
				std::sort(sort.begin(), sort.end(), [axis](const AABB& a, const AABB& b)->bool {
					return a.GetCenter(axis) < b.GetCenter(axis);
				});

				node.left  = node_index++;
				node.right = node_index++;

				this->node.push_back(node);

				Construction(std::vector<AABB>(sort.begin(), sort.begin() + sprite), false);
				Construction(std::vector<AABB>(sort.begin() + sprite, sort.end()), false);
			}
			else {
				node.left = node.right = -1;

				this->node.push_back(node);
			}
		}
	};
}

/* 初期化 */
std::uint32_t Initialize(void)
{
	/* ウィンドウモードにする */
	DxLib::ChangeWindowMode(true);
	/* ウィンドウ名の設定 */
	DxLib::SetWindowText(_T("おかもん"));
	/* ウィンドウサイズ&カラービット&リフレッシュレートの設定 */
	DxLib::SetGraphMode(window_x, window_y, 32);

	/* ライブラリの初期化 */
	if (DxLib::DxLib_Init() != S_OK) {
		return S_FALSE;
	}

	/* 裏画面に描画 */
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	/* Zバッファの有効 */
	DxLib::SetUseZBuffer3D(true);
	/* Zバッファへの書き込み有効 */
	SetWriteZBuffer3D(true);

	/* カメラの設定 */
	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(camera.x, camera.y, camera.z), VGet(target.x, target.y, target.z));
	/* カメラの描画範囲設定 */
	DxLib::SetCameraNearFar(0.1f, 100.0f);

	return S_OK;
}

/* 更新 */
void Update(void)
{
	const float velocity = 0.1f;
	static DxLib::FLOAT3 offset = { 0.0f, 0.0f, 0.0f };
	if (DxLib::CheckHitKey(KEY_INPUT_RIGHT) == TRUE) {
		offset.x += velocity;
	}
	else if (DxLib::CheckHitKey(KEY_INPUT_LEFT) == TRUE) {
		offset.x -= velocity;
	}
	else if (DxLib::CheckHitKey(KEY_INPUT_UP) == TRUE) {
		offset.z += velocity;
	}
	else if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == TRUE) {
		offset.z -= velocity;
	}

	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(camera.x + offset.x, camera.y + offset.y, camera.z + offset.z),
		VGet(target.x, target.y, target.z));
}

/* エントリーポイント */
int main()
{
	if (FAILED(Initialize())) {
		return S_FALSE;
	}

	BVH bvh{};

	/* モデル読み込み */
	std::uint32_t model = DxLib::MV1LoadModel(_T("../material/Model/初音ミク.pmd"));

	/* ファイルオープン */
	std::ifstream file("../material/Model/初音ミク.pmd", std::ios::in | std::ios::binary);
	if (file.is_open() == true) {
		pmd::PmdHeader header{};
		file.read((char*)&header, sizeof(header.byte));

		pmd::PmdVertexList vertex{};
		file.read((char*)&vertex.num, sizeof(vertex.num));
		if (vertex.num > 0) {
			vertex.vertex.resize(vertex.num);
			for (std::uint32_t i = 0; i < vertex.num; ++i) {
				file.read((char*)&vertex.vertex[i].byte, sizeof(vertex.vertex[i].byte));
			}
		}

		pmd::PmdIndexList index{};
		file.read((char*)&index.num, sizeof(index.num));
		if (index.num > 0) {
			index.index.resize(index.num);
			file.read((char*)&index.index[0], sizeof(index.index[0]) * index.num);
		}

		pmd::PmdMaterialList material{};
		file.read((char*)&material.num, sizeof(material.num));
		if (material.num > 0) {
			material.material.resize(material.num);
			for (std::uint32_t i = 0; i < material.num; ++i) {
				file.read((char*)&material.material[i].byte, sizeof(material.material[i].byte));
			}
		}

		file.close();

		/* BVHの構築 */
		std::vector<AABB>aabb;
		for (std::uint32_t i = 0; i < index.index.size(); i += 3) {

			AABB tmp{};
			for (std::uint32_t n = 0; n < 3; ++n) {
				for (std::uint32_t m = 0; m < 3; ++m) {
					tmp.min[m] = std::fmin(tmp.min[m], vertex.vertex[index.index[i + n]].pos[m]);
					tmp.max[m] = std::fmax(tmp.max[m], vertex.vertex[index.index[i + n]].pos[m]);
				}
			}

			aabb.push_back(tmp);
		}

		bvh.Construction(aabb);
	}

	while (DxLib::ProcessMessage() == S_OK && DxLib::CheckHitKey(KEY_INPUT_ESCAPE) == FALSE) {
		/* 表画面をクリア */
		DxLib::ClearDrawScreen();

		/* モデルの描画 */
		MV1DrawModel(model);

		for (std::uint32_t i = 0; i < bvh.node.size(); ++i) {
			if (bvh.node[i].left == -1 && bvh.node[i].right == -1) {
				AABB* aabb = &bvh.node[i].aabb;

				std::random_device seed;
				std::mt19937_64 mt(seed());
				std::uniform_int_distribution<std::uint32_t>random(0, 0xFFFFFFFF);

				std::uint32_t color = random(mt);

				/* 上の4辺 */
				DxLib::DrawLine3D(VGet(aabb->min[0], aabb->max[1], aabb->min[2]), VGet(aabb->max[0], aabb->max[1], aabb->min[2]), color);
				DxLib::DrawLine3D(VGet(aabb->min[0], aabb->max[1], aabb->min[2]), VGet(aabb->min[0], aabb->max[1], aabb->max[2]), color);
				DxLib::DrawLine3D(VGet(aabb->max[0], aabb->max[1], aabb->min[2]), VGet(aabb->max[0], aabb->max[1], aabb->max[2]), color);
				DxLib::DrawLine3D(VGet(aabb->min[0], aabb->max[1], aabb->max[2]), VGet(aabb->max[0], aabb->max[1], aabb->max[2]), color);

				/* 柱の4本 */
				DxLib::DrawLine3D(VGet(aabb->min[0], aabb->min[1], aabb->min[2]), VGet(aabb->min[0], aabb->max[1], aabb->min[2]), color);
				DxLib::DrawLine3D(VGet(aabb->max[0], aabb->min[1], aabb->min[2]), VGet(aabb->max[0], aabb->max[1], aabb->min[2]), color);
				DxLib::DrawLine3D(VGet(aabb->min[0], aabb->min[1], aabb->max[2]), VGet(aabb->min[0], aabb->max[1], aabb->max[2]), color);
				DxLib::DrawLine3D(VGet(aabb->max[0], aabb->min[1], aabb->max[2]), VGet(aabb->max[0], aabb->max[1], aabb->max[2]), color);

				/* 下の4辺 */
				DxLib::DrawLine3D(VGet(aabb->min[0], aabb->min[1], aabb->min[2]), VGet(aabb->max[0], aabb->min[1], aabb->min[2]), color);
				DxLib::DrawLine3D(VGet(aabb->min[0], aabb->min[1], aabb->min[2]), VGet(aabb->min[0], aabb->min[1], aabb->max[2]), color);
				DxLib::DrawLine3D(VGet(aabb->max[0], aabb->min[1], aabb->min[2]), VGet(aabb->max[0], aabb->min[1], aabb->max[2]), color);
				DxLib::DrawLine3D(VGet(aabb->min[0], aabb->min[1], aabb->max[2]), VGet(aabb->max[0], aabb->min[1], aabb->max[2]), color);
			}
		}

		/* 裏・表画面の入れ替え */
		DxLib::ScreenFlip();

		Update();
	}

	DxLib::DxLib_End();
	return S_OK;
}