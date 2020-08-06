#pragma once
#include "Vector.h"

struct HWND__;

class Window
{
public:
	/* �R���X�g���N�^ */
	Window(const Dx12::Vec2& size, const Dx12::Vec2& location = 0x80000000);
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

public:
	/* ���b�Z�[�W�̊m�F */
	static bool CheckMsg(void);

private:
	/* �E�B���h�E�̐��� */
	HWND__* CreateWindowHandle(const Dx12::Vec2& size, const Dx12::Vec2& location);

private:
	/* �n���h�� */
	HWND__* handle{ nullptr };
	/* �N���X�� */
	const wchar_t* name{ nullptr };
	/* �C���X�^���X */
	const void* instance{ nullptr };
};
