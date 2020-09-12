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
		/*�� �X���b�v�`�F�C���̐��� 
		 * @param window �E�B���h�E
		 * @param queue �R�}���h�L���[
		 * @param buffer_num �o�b�N�o�b�t�@�[��
		 * @return �X���b�v�`�F�C��
		 */
		static IDXGISwapChain4* CreateSwapChain(const Window* window, const CommandQueue* queue, const std::uint32_t& buffer_num);

	public:
		/** �R���X�g���N�^ */
		SwapChain();
		/** �R���X�g���N�^
		 * @param window �E�B���h�E
		 * @param queue �R�}���h�L���[
		 * @param buffer_num �o�b�N�o�b�t�@�[��
		 */
		SwapChain(const Window* window, const CommandQueue* queue, const std::uint32_t& buffer_num = 2);
		/** �R���X�g���N�^
		 * @param swap �X���b�v�`�F�C��
		 */
		SwapChain(IDXGISwapChain4* swap);
		/*�� �f�X�g���N�^ */
		~SwapChain();

	public:
		/** ���\�[�X�̎擾 
		 * @param index ���\�[�X�z��̃C���f�b�N�X
		 * @return ���\�[�X
		 */
		ID3D12Resource2* GetResource(const std::uint32_t& index);
		/*�� �o�b�t�@�̐؂�ւ�*/
		void Present(void) const;

	public:
		/*�� �T�C�Y�̎擾 
		 * @return ���\�[�X�̏c���T�C�Y
		 */
		Math::Vec2 GetSize(void) const;
		/** �o�b�t�@���̎擾 
		 * @return �o�b�N�o�b�t�@�[��
		 */
		std::uint32_t GetBufferNum(void) const;
		/*�� ���݂̃o�b�t�@�ԍ����擾 
		 * @return �o�b�N�o�b�t�@�[�̃C���f�b�N�X
		 */
		std::uint32_t GetBufferIndex(void) const;
	};
}
