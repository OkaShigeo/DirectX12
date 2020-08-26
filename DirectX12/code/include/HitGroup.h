#pragma once
#include "SubObject.h"
#include <string>
#include <vector>

namespace Dx12
{
	class HitGroup :
		public SubObject
	{
	public:
		/* コンストラクタ */
		HitGroup();
		HitGroup(const std::wstring& hit_name, const std::vector<std::wstring>& func_name);
		/* デストラクタ */
		~HitGroup();

	public:
		/* サブオブジェクトの追加 */
		void AddSubObj(const std::wstring& hit_name, const std::vector<std::wstring>& func_name);

	private:
		/* コピーコンストラクタ */
		HitGroup(const HitGroup&) = delete;
	};
}
