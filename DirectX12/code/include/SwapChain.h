#pragma once
#include "BaseObject.h"
#include "Vector.h"

class Window;

namespace Dx12
{
	class CommandQueue;
	class Resource;

	class SwapChain :
		public BaseObject<IDXGISwapChain4>
	{
	public:
		/* �X���b�v�`�F�C���̐��� */
		static IDXGISwapChain4* CreateSwapChain(const Window* window, const CommandQueue* queue, const std::uint32_t& buffer_num);

	public:
		/* �R���X�g���N�^ */
		SwapChain();
		SwapChain(const Window* window, const CommandQueue* queue, const std::uint32_t& buffer_num = 2);
		SwapChain(IDXGISwapChain4* swap);
		/* �f�X�g���N�^ */
		~SwapChain();

	public:
		/* ���\�[�X�̎擾 */
		ID3D12Resource2* GetResource(const std::uint32_t& index);
		/* �o�b�t�@�̐؂�ւ�*/
		void Present(void) const;

	public:
		/* �T�C�Y�̎擾 */
		Vec2 GetSize(void) const;
		/* �o�b�t�@���̎擾 */
		std::uint32_t GetBufferNum(void) const;
		/* ���݂̃o�b�t�@�ԍ����擾 */
		std::uint32_t GetBufferIndex(void) const;
	};
}
