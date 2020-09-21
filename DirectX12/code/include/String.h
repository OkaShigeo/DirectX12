#pragma once
#include <string>
#include <vector>

namespace Str
{
	class String
	{
	public:
		/** �R���X�g���N�^ */
		String() {}
		/** �R���X�g���N�^
		 * @param str string�^�̃f�[�^
		 */
		String(const std::string& str)
		{
			num = str.size();
			buffer = new std::uint8_t[sizeof(char) * (num + 1)];
			std::memcpy(buffer, str.c_str(), _msize(buffer));
		}
		/** �R���X�g���N�^
		 * @param str wstring�^�̃f�[�^
		 */
		String(const std::wstring& str)
		{
			num = str.size();
			unicode = true;
			buffer = new std::uint8_t[sizeof(wchar_t) * (num + 1)];
			std::memcpy(buffer, str.c_str(), _msize(buffer));

		}
		/** �R���X�g���N�^
		 * @param str �}���`�o�C�g�����f�[�^
		 */
		String(const char* str)
		{
			if (str != nullptr) {
				num = strlen(str) + 1;
				buffer = new std::uint8_t[sizeof(char) * num];
				std::memcpy(buffer, str, _msize(buffer));
			}
		}
		/** �R���X�g���N�^
		 * @param str ���j�R�[�h�����f�[�^
		 */
		String(const wchar_t* str)
		{
			if (str != nullptr) {
				num = wcslen(str) + 1;
				unicode = true;
				buffer = new std::uint8_t[sizeof(wchar_t) * num];
				std::memcpy(buffer, str, _msize(buffer));
			}
		}
		/** �R�s�[�R���X�g���N�^ */
		String(const String& str)
		{
			num = str.num;
			unicode = str.unicode;
			if (str.buffer != nullptr) {
				buffer = new std::uint8_t[_msize(str.buffer)];
				std::memcpy(buffer, str.buffer, _msize(buffer));
			}
		}
		/** �f�X�g���N�^ */
		~String() {
			Release();
		}

	public:
		/*�� �}���`�o�C�g�����R�[�h�̎擾
		 * @return string�̌^�f�[�^
		 */
		std::string GetMultibyteCode(void) const
		{
			if (buffer != nullptr) {
				if (unicode == false) {
					return std::string((char*)buffer);
				}
				else {
					std::vector<std::uint8_t>tmp(sizeof(char) * (num + 1));
					size_t convert_num = 0;
					if (wcstombs_s(&convert_num, (char*)tmp.data(), num + 1, (const wchar_t*)buffer, _TRUNCATE) == 0) {
						return std::string((char*)tmp.data());
					}
				}
			}

			return std::string();
		}
		/*�� ���j�R�[�h�����̎擾
		 * @return wstring�^�̃f�[�^
		 */
		std::wstring GetUniCode(void) const
		{
			if (buffer != nullptr) {
				if (unicode == true) {
					return std::wstring((wchar_t*)buffer);
				}
				else {
					std::vector<std::uint8_t>tmp(sizeof(wchar_t) * (num + 1));
					size_t convert_num = 0;
					if (mbstowcs_s(&convert_num, (wchar_t*)tmp.data(), num + 1, (const char*)buffer, _TRUNCATE) == 0) {
						return std::wstring((wchar_t*)tmp.data());
					}
				}
			}

			return std::wstring();
		}
		/** �}���`�o�C�g������|�C���^�̎擾
		 * @return �}���`�o�C�g������̃|�C���^
		 */
		const char* GetMultibyteCodePtr(void) const
		{
			if (buffer != nullptr) {
				if (unicode == false) {
					char* tmp = new char[num + 1];
					std::memcpy(tmp, buffer, sizeof(buffer[0]) * (num + 1));
					return tmp;
				}
				else {
					char* tmp = new char[num + 1];
					size_t convert_num = 0;
					if (wcstombs_s(&convert_num, tmp, num + 1, (const wchar_t*)buffer, _TRUNCATE) == 0) {
						return tmp;
					}
				}
			}

			return nullptr;
		}
		/** ���j�R�[�h������|�C���^�̎擾
		 * @return ���j�R�[�h������|�C���^
		 */
		const wchar_t* GetUniCodePtr(void) const
		{
			if (buffer != nullptr) {
				if (unicode == true) {
					wchar_t* tmp = new wchar_t[num + 1];
					std::memcpy(tmp, buffer, sizeof(buffer[0]) * (num + 1));
					return tmp;
				}
				else {
					wchar_t* tmp = new wchar_t[num + 1];
					size_t convert_num = 0;
					if (mbstowcs_s(&convert_num, tmp, num + 1, (const char*)buffer, _TRUNCATE) == 0) {
						return tmp;
					}
				}
			}

			return nullptr;
		}
		/** �������̎擾
		 * @return ������(�I�[�����܂߂Ȃ�)
		 */
		std::uint64_t GetNum(void) const
		{
			return num;
		}
		/** ���j�R�[�h�������i�[����Ă��邩�`�F�b�N */
		bool IsUniCode(void) const
		{
			return unicode;
		}

		/*----------�I�y���[�^�̃I�[�o�[���[�h�֐�----------*/
	public:
		void operator=(const std::string& str)
		{
			Release();

			num = str.size();
			unicode = false;
			buffer = new std::uint8_t[sizeof(char) * (num + 1)];
			std::memcpy(buffer, str.c_str(), _msize(buffer));
		}
		void operator=(const std::wstring& str)
		{
			Release();

			num = str.size();
			unicode = true;
			buffer = new std::uint8_t[sizeof(wchar_t) * (num + 1)];
			std::memcpy(buffer, str.c_str(), _msize(buffer));
		}
		void operator=(const char* str)
		{
			Release();

			if (str != nullptr) {
				num = std::string(str).size();
				unicode = false;
				buffer = new std::uint8_t[sizeof(char) * (num + 1)];
				std::memcpy(buffer, str, _msize(buffer));
			}
		}
		void operator=(const wchar_t* str)
		{
			Release();

			if (str != nullptr) {
				num = std::wstring(str).size();
				unicode = true;
				buffer = new std::uint8_t[sizeof(wchar_t) * (num + 1)];
				std::memcpy(buffer, str, _msize(buffer));
			}
		}

	private:
		/** �������̉�� */
		void Release(void)
		{
			if (buffer != nullptr) {
				delete buffer;
				buffer = nullptr;
			}
		}

	private:
		/* �i�[�o�b�t�@ */
		std::uint8_t* buffer{ nullptr };
		/* ������ */
		std::uint64_t num{ 0 };
		/* ���j�R�[�h�t���O */
		bool unicode{ false };
	};
}
