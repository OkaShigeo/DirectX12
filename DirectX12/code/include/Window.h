#pragma once
#include "Vector.h"

struct HWND__;

class Window
{
public:
	/* �R���X�g���N�^ */
	Window(const st::Vec2& size, const st::Vec2& pos = 0x80000000);
	/* �f�X�g���N�^ */
	~Window();
	/* �E�B���h�E�\�� */
	void ShowWndow(void) const;

public:
	/* �E�B���h�E�n���h���̎擾 */
	HWND__* Get(void) const;
	/* �E�B���h�E�̃T�C�Y�����߂� */
	st::Vec2 GetWindowSize(void) const;
	/* �E�B���h�E�̍��W�����߂� */
	st::Vec2 GetWindowPos(void) const;

private:
	/* �E�B���h�E�n���h���̐��� */
	std::uint32_t CreateWindowHandle(const st::Vec2& size, const st::Vec2& pos = 0x80000000);


private:
	/* �E�B���h�E�n���h�� */
	HWND__* handle;
	/* �E�B���h�E�N���X�� */
	const wchar_t* name;
	/* �C���X�^���X */
	const void* instance;
};
