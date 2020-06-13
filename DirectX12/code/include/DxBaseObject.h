#pragma once
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_6.h>

template <typename T>
class DxBaseObject
{
public:
	/* コンストラクタ */
	DxBaseObject() : obj(nullptr) {}

public:
	/* オブジェクトの取得 */
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
	/* デストラクタ */
	virtual ~DxBaseObject() {
#define RELEASE(X) { if ((X) != nullptr) (X)->Release(); (X) = nullptr; }
		RELEASE(obj);
	}


protected:
	/* オブジェクト */
	T* obj;
};
