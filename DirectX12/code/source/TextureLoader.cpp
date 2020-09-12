#include "..\include\TextureLoader.h"
#include "..\include\Bitmap.h"
#include <functional>

std::unordered_map<std::string, Tex::TextureInfo> Tex::TextureLoader::tex_info;
std::unordered_map<std::string, std::vector<std::uint8_t>> Tex::TextureLoader::tex_data;

namespace
{
	std::unordered_map<std::string, std::function<bool(const std::string&, Tex::TextureInfo&, std::vector<std::uint8_t>&)>>func = {
		{".bmp", [&](const std::string& file_path, Tex::TextureInfo& information, std::vector<std::uint8_t>& data)->bool {
			Bitmap::InformationHeader info{};
			if (Bitmap::LoadFromFile(file_path, info, data) == true) {
				information.depth = std::uint32_t(info.depth);
				information.height = info.height;
				information.width = info.width;

				return true;
			}
			return false;
		} },
	};
}

Tex::TextureLoader::TextureLoader()
{
}

Tex::TextureLoader::~TextureLoader()
{
}

bool Tex::TextureLoader::LoadFromFile(const std::string& file_path, TextureInfo* information)
{
	std::string fmt = file_path.substr(file_path.find_last_of('.'));
	auto itr = tex_info.find(fmt);
	if (itr == tex_info.end()) {
		TextureInfo tmp_info{};
		std::vector<std::uint8_t> tmp_data;
		if (func[fmt](file_path, tmp_info, tmp_data) == true) {
			tex_info[file_path]    = tmp_info;
			tex_info[file_path].id = (std::uint32_t)((std::uint32_t*) & tex_info[fmt]);
			std::swap(tex_data[file_path], tmp_data);
		}
		else {
			return false;
		}
	}

	if (information != nullptr) {
		*information = tex_info[file_path];
	}

	return true;
}

void Tex::TextureLoader::Deleted(const std::string& file_path)
{
	auto itr = tex_info.find(file_path);
	if (itr != tex_info.end()) {
		tex_info.erase(itr);
		tex_data.erase(tex_data.find(file_path));
	}
}

void Tex::TextureLoader::Deleted(const std::uint32_t& id)
{
	if (id != UINT_MAX) {
		for (auto itr = tex_info.begin(); itr != tex_info.end(); ++itr) {
			if ((*itr).second.id == id) {
				tex_data.erase(tex_data.find(itr->first));
				tex_info.erase(itr);
				break;
			}
		}
	}
}

const Tex::TextureInfo Tex::TextureLoader::GetInfo(const std::string& file_path)
{
	auto itr = tex_info.find(file_path);
	if (itr != tex_info.end()) {
		return (*itr).second;
	}

	return TextureInfo();
}

const Tex::TextureInfo Tex::TextureLoader::GetInfo(const std::uint32_t id)
{
	if (id != UINT_MAX) {
		for (auto itr = tex_info.begin(); itr != tex_info.end(); ++itr) {
			if ((*itr).second.id == id) {
				return (*itr).second;
			}
		}
	}

	return TextureInfo();
}

const std::vector<std::uint8_t>* Tex::TextureLoader::GetData(const TextureInfo& information)
{
	if (information.id != UINT_MAX) {
		for (auto itr = tex_info.begin(); itr != tex_info.end(); ++itr) {
			if ((*itr).second.id == information.id) {
				return &tex_data[(*itr).first];
			}
		}
	}

	return nullptr;
}
