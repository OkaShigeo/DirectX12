#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace Tex
{
	/* 画像の情報 */
	struct TextureInfo
	{
		/* 識別ID */
		std::uint32_t id{ UINT_MAX };
		/* 横幅(ピクセル)*/
		std::uint32_t width;
		/* 縦幅(ピクセル) */
		std::uint32_t height;
		/* ビット深度 */
		std::uint32_t depth;
	};

	class TextureLoader
	{
	private:
		/** コンストラクタ */
		TextureLoader();
		/** デストラクタ */
		~TextureLoader();

	public:
		/*＊ ファイルの読み込み
		 * @param file_path ファイルパス
		 * @param information 画像情報の格納先
		 * @return true:読み込み成功 / false:読み込み失敗
		 */
		static bool LoadFromFile(const std::string& file_path, TextureInfo* information);
		/** 画像の削除 
		 * @param file_path ファイルパス
		 */
		static void Deleted(const std::string& file_path);
		/** 画像の削除
		 * @param id 識別番号
		 */
		static void Deleted(const std::uint32_t& id);

	public:
		/*＊ 画像情報の取得
		 * @param file_path ファイルパス
		 * @return 画像の情報
		 */
		static const TextureInfo GetInfo(const std::string& file_path);
		/*＊ 画像情報の取得
		 * @param id 識別番号
		 * @return 画像の情報
		 */
		static const TextureInfo GetInfo(const std::uint32_t id);
		/** 画像データの取得
		 * @param information 画像の情報
		 * @return 画像データ
		 */
		static const std::vector<std::uint8_t>* GetData(const TextureInfo& information);

	private:
		/* 画像の情報 */
		static std::unordered_map<std::string, TextureInfo> tex_info;
		/* 画像データ */
		static std::unordered_map<std::string, std::vector<std::uint8_t>> tex_data;
	};
}
