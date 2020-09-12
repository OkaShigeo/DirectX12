#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace Bitmap
{
	/* ファイルヘッダ */
	struct Header
	{
		/* ファイルタイプ("BM") */
		unsigned char type[2];
		/* ファイルサイズ */
		std::uint32_t size;
		/* 予約領域 */
		std::uint16_t reserved_1{ 0 };
		/* 予約領域 */
		std::uint16_t reserved_2{ 0 };
		/* ファイル先頭から画像データまでのオフセット */
		std::uint32_t offset;

		/** データサイズの取得
		 * @return データサイズ
		 */
		std::uint32_t GetSize(void) const
		{
			return sizeof(type) + sizeof(size) 
				+ sizeof(reserved_1) + sizeof(reserved_2) + sizeof(offset);
		}
	};

	/* 色ビットタイプ */
	enum class BitType : std::uint16_t
	{
		Bit_1  = 1,
		Bit_4  = 4,
		Bit_8  = 8,
		Bit_16 = 16,
		Bit_24 = 24,
		Bit_32 = 32,
	};

	/* 圧縮タイプ */
	enum class CompressedType : std::uint32_t
	{
		None,
		RunLength_8,
		RunLength_4,
		Fields
	};

	/* 情報ヘッダ */
	struct InformationHeader
	{
#ifdef _WIN32
		/* 情報ヘッダサイズ */
		std::uint32_t size{ 40 };
		/* 画像の横幅(ピクセル) */
		std::uint32_t width;
		/* 画像の縦幅(ピクセル) */
		std::uint32_t height;
		/* プレーン数 */
		std::uint16_t plane{ 1 };
		/* 色ビット数 */
		BitType depth;
		/* 圧縮形式 */
		CompressedType compressed;
		/* 画像データサイズ */
		std::uint32_t texture_size;
		/* 水平解像度(dot/m) */
		std::uint32_t horizontal_resolution{ 0 };
		/* 垂直解像度(dot/m) */
		std::uint32_t vertical_resolution{ 0 };
		/* 格納パレット数 */
		std::uint32_t pallet{ 0 };
		/* 重要色数 */
		std::uint32_t important_color{ 0 };
#else
		/* 情報ヘッダサイズ */
		std::uint32_t size{ 12 };
		/* 画像の横幅(ピクセル) */
		std::uint16_t width;
		/* 画像の縦幅(ピクセル) */
		std::uint16_t height;
		/* プレーン数 */
		std::uint16_t plane{ 1 };
		/* 色ビット数 */
		BitType bit;
#endif
	};

	/* 色空間タイプ*/
	enum class ColorSpaceType : std::uint32_t
	{
		/* 色空間無し */
		None = 0,
		/* sRGB色空間 */
		sRGB = 0x73524742,
		/* Windows標準の色空間 */
		Win = 0x57696E20,
		/* 色空間プロフィールファイルのパスがBMPファイル中に含まれている */
		LINK = 0x4C494E4B,
		/* 色空間プロフィールがBMPファイル中に含まれている */
		MBED = 0x4D424544
	};

	/* カラーモデルの座標 */
	struct ColorModelPosition
	{
		/* X座標 */
		std::int32_t x;
		/* Y座標 */
		std::int32_t y;
		/* Z座標 */
		std::int32_t z;
	};

	/* 情報ヘッダの拡張部 */
	struct ExpansionInformationV4
	{
		/* 赤のカラーマスク(圧縮タイプが3のみ) */
		std::uint32_t red_mask;
		/* 緑のカラーマスク(圧縮タイプが3のみ) */
		std::uint32_t green_mask;
		/* 青のカラーマスク(圧縮タイプが3のみ) */
		std::uint32_t blue_mask;
		/* 透明度のカラーマスク */
		std::uint32_t alpha_mask;
		/* 色空間 */
		ColorSpaceType color_space{ ColorSpaceType::None };
		/* カラーモデル赤の座標(色空間が0のみ) */
		ColorModelPosition red_pos;
		/* カラーモデル緑の座標(色空間が0のみ) */
		ColorModelPosition green_pos;
		/* カラーモデル青の座標(色空間が0のみ) */
		ColorModelPosition blue_pos;
		/* 赤のガンマ値(色空間が0のみ) */
		std::uint32_t red_gamma;
		/* 緑のガンマ値(色空間が0のみ) */
		std::uint32_t green_gamma;
		/* 青のガンマ値(色空間が0のみ) */
		std::uint32_t blue_gamma;
	};

	/* sRGB色空間タイプ */
	enum class sRGBSpaceType : std::uint32_t
	{
		/* 色相や明度の保存に重点を置いたイメージ */
		Saturation = 1,
		/* カラーアピアランスの一致に重点を置いたイメージ */
		RelativeColorimetric = 2,
		/* 比色分析の正確さに重点を置いたイメージ */
		Perceptual = 4,
		/* 比色の絶対値の保存が必要なイメージ */
		AbsoluteColorimetric = 8
	};

	/* 情報ヘッダの拡張部 */
	struct ExpansionInformationV5
	{
		/* sRGB色空間タイプ */
		sRGBSpaceType sRGB_type;
		/* 色空間プロフィールのオフセット */
		union {
			/* 色空間プロフィールファイルのパスを表す"\0"で終わる文字列 */
			unsigned char link[4];
			/* 色空間プロフィールサイズ分の埋め込みデータ */
			std::uint32_t mbed;
		};
		/* 色空間プロフィールサイズ */
		std::uint32_t profile_size;
		/* 予約領域 */
		std::uint32_t reserved{ 0 };
	};

	/* パレットデータ(色ビットが1,4,8の場合に存在する) */
	struct PalletData
	{
		union {
			struct {
				/* 青 */
				std::uint8_t blue;
				/* 緑 */
				std::uint8_t green;
				/* 赤 */
				std::uint8_t red;
			};
			/* 色情報 */
			std::uint8_t color[3];
		};
#ifdef _WIN32
		/* 予約領域 */
		std::uint8_t reserved{ 0 };
#endif
	};

	/** ファイルの読み込み
	 * @param file_path ファイルパス
	 * @param information 画像情報の格納先
	 * @param texture 画像データの格納先
	 * @return true:成功 / false:失敗
	 */
	bool LoadFromFile(const std::string& file_path, InformationHeader& information, std::vector<std::uint8_t>& texture)
	{
		/* ファイルオープン */
		std::ifstream file(file_path.c_str(), std::ios::in | std::ios::binary);
		if (file.is_open() == false) {
			return false;
		}
		else {
			/* ファイルヘッダの読み込み */
			Header header{};
			{
				file.read((char*)&header.type[0], sizeof(header.type));
				if (std::string((char*)header.type).find("BM") == std::string::npos) {
					file.close();
					return false;
				}

				file.read((char*)&header.size, header.GetSize() - sizeof(header.type));
			}
			/* 情報ヘッダの読み込み */
			{
				file.read((char*)&information, sizeof(information));
				if (information.size > sizeof(information)) {
					ExpansionInformationV4 expansion_v4{};
					file.read((char*)&expansion_v4, sizeof(expansion_v4));
					if (information.size > sizeof(information) + sizeof(expansion_v4)) {
						ExpansionInformationV5 expansion_v5{};
						file.read((char*)&expansion_v5, sizeof(expansion_v5));
					}
				}
			}

			/* パレットデータの読み込み */
			std::vector<PalletData>pallet;
			{
				if (information.depth < BitType::Bit_16) {
					pallet.resize(std::uint32_t(std::pow(2, std::uint32_t(information.depth))));
					for (auto& data : pallet) {
						for (auto& i : data.color) {
							file.read((char*)&i, sizeof(i));
						}
#ifdef _WIN32
						file.read((char*)&data.reserved, sizeof(data.reserved));
#endif
					}
				}
			}
			/* 画像データの読み込み */
			{
				texture.resize(4 * information.width * information.height);
				switch (information.depth)
				{
				case BitType::Bit_1:
					break;
				case BitType::Bit_4:
					break;
				case BitType::Bit_8:
					break;
				case BitType::Bit_16:
					break;
				case BitType::Bit_24:
				case BitType::Bit_32:
					for (std::uint32_t y = 0; y < information.height; ++y) {
						for (std::uint32_t x = 0; x < information.width; ++x) {
							std::uint32_t index = (4 * y * information.width) + (4 * x);
							if (information.depth == BitType::Bit_24) {
								/* パディング */
								texture[index++] = 0;
							}
							/* RGBのセット */
							file.read((char*)&texture[index], std::uint32_t(information.depth) / 8);
						}
					}
					break;
				default:
					file.close();
					return false;
					break;
				}
			}

			/* ファイルクローズ */
			file.close();
		}

		return true;
	}
}
