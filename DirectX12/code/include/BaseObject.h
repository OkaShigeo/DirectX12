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
		/** コンストラクタ */
		BaseObject() {}
		/** デストラクタ */
		virtual ~BaseObject() {
			Release();
		}

	public:
		/** オブジェクトの破棄 */
		void Release(void) {
			if (obj != nullptr) {
				obj->Release();
				obj = nullptr;
			}
		}
		/** オブジェクトの有効判定 
		 * @return true:有効/false:無効
		 */
		bool IsValied(void) const {
			return (obj != nullptr);
		}

	public:
		/*＊ オブジェクトの取得
		 * @return テンプレートオブジェクト
		 */
		T* Get(void) const {
			return obj;
		}
		/** オブジェクトの参照を取得 
		 * @return テンプレートオブジェクトのポインタ
		 */
		T** GetAddressOf(void) {
			return &obj;
		}

		/*----------オペレータオーバーロード関数----------*/
	public:
		void operator=(const BaseObject& base) {
			Release();
			obj = base.obj;
		}
		void operator=(const BaseObject* base) {
			Release();
			obj = base->obj;
		}
		void operator=(T* ptr) {
			Release();
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
