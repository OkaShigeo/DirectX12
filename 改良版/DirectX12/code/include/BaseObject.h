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
		/* �R���X�g���N�^ */
		BaseObject() {}
		/* �f�X�g���N�^ */
		virtual ~BaseObject() {
			if (obj != nullptr) {
				obj->Release();
				obj = nullptr;
			}
		}

	public:
		/* �I�u�W�F�N�g�̗L������ */
		bool IsValied(void) const {
			return (obj != nullptr);
		}

	public:
		/* �I�u�W�F�N�g�̎擾 */
		T* Get(void) const {
			return obj;
		}
		/* �I�u�W�F�N�g�̎Q�Ƃ��擾 */
		T** GetAddressOf(void) {
			return &obj;
		}

		/* ���Z�q�I�[�o�[���[�h */
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
		/* �I�u�W�F�N�g */
		T* obj{ nullptr };
	};
}
