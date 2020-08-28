#pragma once
#include <vector>

namespace Dx12
{
	class DescriptorHeap;
	class Resource;

	class AcceleratoinStructure
	{
	public:
		/* コンストラクタ */
		AcceleratoinStructure();
		/* デストラクタ */
		~AcceleratoinStructure();

	public:
		/* メモリの解放 */
		void Release(void);

	private:
		/* ヒープ */
		DescriptorHeap* heap{ nullptr };
		/* リソース */
		std::vector<Resource*>rsc;
	};
}
