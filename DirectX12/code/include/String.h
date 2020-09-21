#pragma once
#include <string>
#include <vector>

namespace Str
{
	class String
	{
	public:
		/** コンストラクタ */
		String() {}
		/** コンストラクタ
		 * @param str string型のデータ
		 */
		String(const std::string& str)
		{
			num = str.size();
			buffer = new std::uint8_t[sizeof(char) * (num + 1)];
			std::memcpy(buffer, str.c_str(), _msize(buffer));
		}
		/** コンストラクタ
		 * @param str wstring型のデータ
		 */
		String(const std::wstring& str)
		{
			num = str.size();
			unicode = true;
			buffer = new std::uint8_t[sizeof(wchar_t) * (num + 1)];
			std::memcpy(buffer, str.c_str(), _msize(buffer));

		}
		/** コンストラクタ
		 * @param str マルチバイト文字データ
		 */
		String(const char* str)
		{
			if (str != nullptr) {
				num = strlen(str) + 1;
				buffer = new std::uint8_t[sizeof(char) * num];
				std::memcpy(buffer, str, _msize(buffer));
			}
		}
		/** コンストラクタ
		 * @param str ユニコード文字データ
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
		/** コピーコンストラクタ */
		String(const String& str)
		{
			num = str.num;
			unicode = str.unicode;
			if (str.buffer != nullptr) {
				buffer = new std::uint8_t[_msize(str.buffer)];
				std::memcpy(buffer, str.buffer, _msize(buffer));
			}
		}
		/** デストラクタ */
		~String() {
			Release();
		}

	public:
		/*＊ マルチバイト文字コードの取得
		 * @return stringの型データ
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
		/*＊ ユニコード文字の取得
		 * @return wstring型のデータ
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
		/** マルチバイト文字列ポインタの取得
		 * @return マルチバイト文字列のポインタ
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
		/** ユニコード文字列ポインタの取得
		 * @return ユニコード文字列ポインタ
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
		/** 文字数の取得
		 * @return 文字数(終端文字含めない)
		 */
		std::uint64_t GetNum(void) const
		{
			return num;
		}
		/** ユニコード文字が格納されているかチェック */
		bool IsUniCode(void) const
		{
			return unicode;
		}

		/*----------オペレータのオーバーロード関数----------*/
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
		/** メモリの解放 */
		void Release(void)
		{
			if (buffer != nullptr) {
				delete buffer;
				buffer = nullptr;
			}
		}

	private:
		/* 格納バッファ */
		std::uint8_t* buffer{ nullptr };
		/* 文字数 */
		std::uint64_t num{ 0 };
		/* ユニコードフラグ */
		bool unicode{ false };
	};
}
