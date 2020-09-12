#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace Bitmap
{
	/* �t�@�C���w�b�_ */
	struct Header
	{
		/* �t�@�C���^�C�v("BM") */
		unsigned char type[2];
		/* �t�@�C���T�C�Y */
		std::uint32_t size;
		/* �\��̈� */
		std::uint16_t reserved_1{ 0 };
		/* �\��̈� */
		std::uint16_t reserved_2{ 0 };
		/* �t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g */
		std::uint32_t offset;

		/** �f�[�^�T�C�Y�̎擾
		 * @return �f�[�^�T�C�Y
		 */
		std::uint32_t GetSize(void) const
		{
			return sizeof(type) + sizeof(size) 
				+ sizeof(reserved_1) + sizeof(reserved_2) + sizeof(offset);
		}
	};

	/* �F�r�b�g�^�C�v */
	enum class BitType : std::uint16_t
	{
		Bit_1  = 1,
		Bit_4  = 4,
		Bit_8  = 8,
		Bit_16 = 16,
		Bit_24 = 24,
		Bit_32 = 32,
	};

	/* ���k�^�C�v */
	enum class CompressedType : std::uint32_t
	{
		None,
		RunLength_8,
		RunLength_4,
		Fields
	};

	/* ���w�b�_ */
	struct InformationHeader
	{
#ifdef _WIN32
		/* ���w�b�_�T�C�Y */
		std::uint32_t size{ 40 };
		/* �摜�̉���(�s�N�Z��) */
		std::uint32_t width;
		/* �摜�̏c��(�s�N�Z��) */
		std::uint32_t height;
		/* �v���[���� */
		std::uint16_t plane{ 1 };
		/* �F�r�b�g�� */
		BitType depth;
		/* ���k�`�� */
		CompressedType compressed;
		/* �摜�f�[�^�T�C�Y */
		std::uint32_t texture_size;
		/* �����𑜓x(dot/m) */
		std::uint32_t horizontal_resolution{ 0 };
		/* �����𑜓x(dot/m) */
		std::uint32_t vertical_resolution{ 0 };
		/* �i�[�p���b�g�� */
		std::uint32_t pallet{ 0 };
		/* �d�v�F�� */
		std::uint32_t important_color{ 0 };
#else
		/* ���w�b�_�T�C�Y */
		std::uint32_t size{ 12 };
		/* �摜�̉���(�s�N�Z��) */
		std::uint16_t width;
		/* �摜�̏c��(�s�N�Z��) */
		std::uint16_t height;
		/* �v���[���� */
		std::uint16_t plane{ 1 };
		/* �F�r�b�g�� */
		BitType bit;
#endif
	};

	/* �F��ԃ^�C�v*/
	enum class ColorSpaceType : std::uint32_t
	{
		/* �F��Ԗ��� */
		None = 0,
		/* sRGB�F��� */
		sRGB = 0x73524742,
		/* Windows�W���̐F��� */
		Win = 0x57696E20,
		/* �F��ԃv���t�B�[���t�@�C���̃p�X��BMP�t�@�C�����Ɋ܂܂�Ă��� */
		LINK = 0x4C494E4B,
		/* �F��ԃv���t�B�[����BMP�t�@�C�����Ɋ܂܂�Ă��� */
		MBED = 0x4D424544
	};

	/* �J���[���f���̍��W */
	struct ColorModelPosition
	{
		/* X���W */
		std::int32_t x;
		/* Y���W */
		std::int32_t y;
		/* Z���W */
		std::int32_t z;
	};

	/* ���w�b�_�̊g���� */
	struct ExpansionInformationV4
	{
		/* �Ԃ̃J���[�}�X�N(���k�^�C�v��3�̂�) */
		std::uint32_t red_mask;
		/* �΂̃J���[�}�X�N(���k�^�C�v��3�̂�) */
		std::uint32_t green_mask;
		/* �̃J���[�}�X�N(���k�^�C�v��3�̂�) */
		std::uint32_t blue_mask;
		/* �����x�̃J���[�}�X�N */
		std::uint32_t alpha_mask;
		/* �F��� */
		ColorSpaceType color_space{ ColorSpaceType::None };
		/* �J���[���f���Ԃ̍��W(�F��Ԃ�0�̂�) */
		ColorModelPosition red_pos;
		/* �J���[���f���΂̍��W(�F��Ԃ�0�̂�) */
		ColorModelPosition green_pos;
		/* �J���[���f���̍��W(�F��Ԃ�0�̂�) */
		ColorModelPosition blue_pos;
		/* �Ԃ̃K���}�l(�F��Ԃ�0�̂�) */
		std::uint32_t red_gamma;
		/* �΂̃K���}�l(�F��Ԃ�0�̂�) */
		std::uint32_t green_gamma;
		/* �̃K���}�l(�F��Ԃ�0�̂�) */
		std::uint32_t blue_gamma;
	};

	/* sRGB�F��ԃ^�C�v */
	enum class sRGBSpaceType : std::uint32_t
	{
		/* �F���▾�x�̕ۑ��ɏd�_��u�����C���[�W */
		Saturation = 1,
		/* �J���[�A�s�A�����X�̈�v�ɏd�_��u�����C���[�W */
		RelativeColorimetric = 2,
		/* ��F���͂̐��m���ɏd�_��u�����C���[�W */
		Perceptual = 4,
		/* ��F�̐�Βl�̕ۑ����K�v�ȃC���[�W */
		AbsoluteColorimetric = 8
	};

	/* ���w�b�_�̊g���� */
	struct ExpansionInformationV5
	{
		/* sRGB�F��ԃ^�C�v */
		sRGBSpaceType sRGB_type;
		/* �F��ԃv���t�B�[���̃I�t�Z�b�g */
		union {
			/* �F��ԃv���t�B�[���t�@�C���̃p�X��\��"\0"�ŏI��镶���� */
			unsigned char link[4];
			/* �F��ԃv���t�B�[���T�C�Y���̖��ߍ��݃f�[�^ */
			std::uint32_t mbed;
		};
		/* �F��ԃv���t�B�[���T�C�Y */
		std::uint32_t profile_size;
		/* �\��̈� */
		std::uint32_t reserved{ 0 };
	};

	/* �p���b�g�f�[�^(�F�r�b�g��1,4,8�̏ꍇ�ɑ��݂���) */
	struct PalletData
	{
		union {
			struct {
				/* �� */
				std::uint8_t blue;
				/* �� */
				std::uint8_t green;
				/* �� */
				std::uint8_t red;
			};
			/* �F��� */
			std::uint8_t color[3];
		};
#ifdef _WIN32
		/* �\��̈� */
		std::uint8_t reserved{ 0 };
#endif
	};

	/** �t�@�C���̓ǂݍ���
	 * @param file_path �t�@�C���p�X
	 * @param information �摜���̊i�[��
	 * @param texture �摜�f�[�^�̊i�[��
	 * @return true:���� / false:���s
	 */
	bool LoadFromFile(const std::string& file_path, InformationHeader& information, std::vector<std::uint8_t>& texture)
	{
		/* �t�@�C���I�[�v�� */
		std::ifstream file(file_path.c_str(), std::ios::in | std::ios::binary);
		if (file.is_open() == false) {
			return false;
		}
		else {
			/* �t�@�C���w�b�_�̓ǂݍ��� */
			Header header{};
			{
				file.read((char*)&header.type[0], sizeof(header.type));
				if (std::string((char*)header.type).find("BM") == std::string::npos) {
					file.close();
					return false;
				}

				file.read((char*)&header.size, header.GetSize() - sizeof(header.type));
			}
			/* ���w�b�_�̓ǂݍ��� */
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

			/* �p���b�g�f�[�^�̓ǂݍ��� */
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
			/* �摜�f�[�^�̓ǂݍ��� */
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
								/* �p�f�B���O */
								texture[index++] = 0;
							}
							/* RGB�̃Z�b�g */
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

			/* �t�@�C���N���[�Y */
			file.close();
		}

		return true;
	}
}
