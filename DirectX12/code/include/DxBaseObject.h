#pragma once
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_6.h>

template <typename T>
class DxBaseObject
{
public:
	/* �R���X�g���N�^ */
	DxBaseObject() : obj(nullptr) {}

public:
	/* �I�u�W�F�N�g�̎擾 */
	T* Get(void) const {
		return obj;
	}

public:
	T* operator->(void) const {
		return obj;
	}
	T** operator&(void) {
		return &obj;
	}

protected:
	/* �f�X�g���N�^ */
	virtual ~DxBaseObject() {
#define RELEASE(X) { if ((X) != nullptr) (X)->Release(); (X) = nullptr; }
		RELEASE(obj);
	}


protected:
	/* �I�u�W�F�N�g */
	T* obj;
};
