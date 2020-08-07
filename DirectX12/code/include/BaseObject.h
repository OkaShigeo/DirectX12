#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>

namespace Dx12
{
	template <typename T>
	class BaseObject
	{
	public:
		/* コンストラクタ */
		BaseObject() {}
		/* デストラクタ */
		virtual ~BaseObject() {
			if (obj != nullptr) {
				obj->Release();
				obj = nullptr;
			}
		}

	public:
		/* オブジェクトの有効判定 */
		bool IsValied(void) const {
			return (obj != nullptr);
		}

	public:
		/* オブジェクトの取得 */
		T* Get(void) const {
			return obj;
		}
		/* オブジェクトの参照を取得 */
		T** GetAddressOf(void) {
			return &obj;
		}

		/* 演算子オーバーロード */
	public:
		void operator=(const BaseObject& base) {
			obj = base.obj;
		}
		void operator=(const BaseObject* base) {
			obj = base->obj;
		}
		void operator=(T* ptr) {
			obj = ptr;
		}
		T* operator->(void) {
			return obj;
		}

	protected:
		/* オブジェクト */
		T* obj{ nullptr };
	};
}
