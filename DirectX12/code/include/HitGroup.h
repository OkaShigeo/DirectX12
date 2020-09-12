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
		/*＊ コンストラクタ */
		HitGroup();
		/** コンストラクタ
		 * @param hit_name ヒット名
		 * @param func_name 関数名
		 */
		HitGroup(const std::wstring& hit_name, const std::vector<std::wstring>& func_name);
		/** デストラクタ */
		~HitGroup();

	public:
		/** サブオブジェクトの追加 
		 * @param hit_name ヒット名
		 * @param func_name 関数名
		 */
		void AddSubObj(const std::wstring& hit_name, const std::vector<std::wstring>& func_name);

	private:
		/** コピーコンストラクタ */
		HitGroup(const HitGroup&) = delete;
	};
}
