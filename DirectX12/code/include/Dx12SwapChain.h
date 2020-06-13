#pragma once
#include "DxBaseObject.h"

class Window;
class Dx12CommandQueue;

class Dx12SwapChain :
	public DxBaseObject<IDXGISwapChain4>
{
public:
	/* �R���X�g���N�^ */
	Dx12SwapChain(const Window* window, const Dx12CommandQueue* queue, const std::uint32_t& back_buffer_num = 2);
	/* �f�X�g���N�^ */
	~Dx12SwapChain();
	/* �o�b�t�@��؂�ւ��� */
	std::uint32_t SwapBuffer(void) const;

public:
	/* ���݂̃o�b�N�o�b�t�@�C���f�b�N�X�̎擾 */
	std::uint32_t GetBackBufferIndex(void) const;

private:
	/* �X���b�v�`�F�C���̐��� */
	std::uint32_t CreateSwapChain(const Window* window, const Dx12CommandQueue* queue, const std::uint32_t& back_buffer_num);
};