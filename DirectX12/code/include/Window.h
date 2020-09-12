#pragma once
#include "Vector.h"

struct HWND__;

class Window
{
public:
	/*�� ���b�Z�[�W�̊m�F 
	 * @return ���b�Z�[�W�̃t���O true:���b�Z�[�W����/false:���b�Z�[�W���� 
	 */
	static bool CheckMsg(void);
	/*�� �E�B���h�E�̐��� 
	 * @param size �E�B���h�E�T�C�Y
	 * @param location �\������ʒu
	 * @return �E�B���h�E�n���h��
	 */
	static HWND__* CreateWindowHandle(const Math::Vec2& size, const Math::Vec2& location);

public:
	/** �R���X�g���N�^ */
	Window();
	/** �R���X�g���N�^
	 * @param size �E�B���h�E�T�C�Y
	 * @param location �\������ʒu
	 */
	Window(const Math::Vec2& size, const Math::Vec2& location = 0x80000000);
	/** �R���X�g���N�^
	 * @param �E�B���h�E�n���h��
	 */
	Window(HWND__* window);
	/** �f�X�g���N�^ */
	~Window();

public:
	/** �E�B���h�E�̕\�� */
	void Show(void) const;

public:
	/** �n���h���̎擾
	 * @return �E�B���h�E�n���h��
	 */
	HWND__* Get(void) const;
	/** �T�C�Y�̎擾 
	 * @return �E�B���h�E�T�C�Y
	 */
	Math::Vec2 GetSize(void) const;
	/*�� �\�����W�̎擾 
	 * @return �E�B���h�E�̕\������ʒu
	 */
	Math::Vec2 GetLocation(void) const;

private:
	/** �������̉�� */
	void Release(void);

private:
	/* �n���h�� */
	HWND__* handle{ nullptr };
};
