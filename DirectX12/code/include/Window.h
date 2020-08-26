#pragma once
#include "Vector.h"

struct HWND__;

class Window
{
public:
	/* ���b�Z�[�W�̊m�F */
	static bool CheckMsg(void);
	/* �E�B���h�E�̐��� */
	static HWND__* CreateWindowHandle(const Dx12::Vec2& size, const Dx12::Vec2& location);

public:
	/* �R���X�g���N�^ */
	Window();
	Window(const Dx12::Vec2& size, const Dx12::Vec2& location = 0x80000000);
	Window(HWND__* window);
	/* �f�X�g���N�^ */
	~Window();

public:
	/* �E�B���h�E�̕\�� */
	void Show(void) const;

public:
	/* �n���h���̎擾 */
	HWND__* Get(void) const;
	/* �T�C�Y�̎擾 */
	Dx12::Vec2 GetSize(void) const;
	/* �\�����W�̎擾 */
	Dx12::Vec2 GetLocation(void) const;

private:
	/* �������̉�� */
	void Release(void);

private:
	/* �n���h�� */
	HWND__* handle{ nullptr };
};
