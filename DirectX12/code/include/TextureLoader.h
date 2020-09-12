#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace Tex
{
	/* �摜�̏�� */
	struct TextureInfo
	{
		/* ����ID */
		std::uint32_t id{ UINT_MAX };
		/* ����(�s�N�Z��)*/
		std::uint32_t width;
		/* �c��(�s�N�Z��) */
		std::uint32_t height;
		/* �r�b�g�[�x */
		std::uint32_t depth;
	};

	class TextureLoader
	{
	private:
		/** �R���X�g���N�^ */
		TextureLoader();
		/** �f�X�g���N�^ */
		~TextureLoader();

	public:
		/*�� �t�@�C���̓ǂݍ���
		 * @param file_path �t�@�C���p�X
		 * @param information �摜���̊i�[��
		 * @return true:�ǂݍ��ݐ��� / false:�ǂݍ��ݎ��s
		 */
		static bool LoadFromFile(const std::string& file_path, TextureInfo* information);
		/** �摜�̍폜 
		 * @param file_path �t�@�C���p�X
		 */
		static void Deleted(const std::string& file_path);
		/** �摜�̍폜
		 * @param id ���ʔԍ�
		 */
		static void Deleted(const std::uint32_t& id);

	public:
		/*�� �摜���̎擾
		 * @param file_path �t�@�C���p�X
		 * @return �摜�̏��
		 */
		static const TextureInfo GetInfo(const std::string& file_path);
		/*�� �摜���̎擾
		 * @param id ���ʔԍ�
		 * @return �摜�̏��
		 */
		static const TextureInfo GetInfo(const std::uint32_t id);
		/** �摜�f�[�^�̎擾
		 * @param information �摜�̏��
		 * @return �摜�f�[�^
		 */
		static const std::vector<std::uint8_t>* GetData(const TextureInfo& information);

	private:
		/* �摜�̏�� */
		static std::unordered_map<std::string, TextureInfo> tex_info;
		/* �摜�f�[�^ */
		static std::unordered_map<std::string, std::vector<std::uint8_t>> tex_data;
	};
}
