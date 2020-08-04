#pragma once
#include "Window.h"
#include "Device.h"

namespace Dx12
{
	class Runtime
	{
	public:
		/* ������ */
		static void Initialize(const Dx12::Vec2& size, const Dx12::Vec2& pos = 0x80000000);
		/* �I�� */
		static void UnInitialize(void);

	public:
		/* �E�B���h�E�̎擾 */
		static Window* GetWindow(void);
		/* �f�o�C�X�̎擾 */
		static Device* GetDevice(void);

	private:
		/* �R���X�g���N�^ */
		Runtime();
		/* �f�X�g���N�^ */
		~Runtime();

	private:
		/* �E�B���h�E */
		static Window* window;
		/* �f�o�C�X */
		static Device* device;
	};
}
