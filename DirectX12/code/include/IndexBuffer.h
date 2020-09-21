#pragma once
#include "Resource.h"

namespace Dx12
{
	class IndexBuffer :
		public Resource
	{
	public:
		/** �C���f�b�N�X�o�b�t�@�̐���
		 * @param index �C���f�b�N�X�f�[�^
		 */
		static ID3D12Resource2* CreateIndexBuffer(void* index);

	public:
		/** �R���X�g���N�^ */
		IndexBuffer();
		/** �R���X�g���N�^
		 * @param index �C���f�b�N�X�f�[�^
		 * @param index_num �C���f�b�N�X��
		 */
		IndexBuffer(void* index, const std::uint64_t& index_num);
		/** �R���X�g���N�^
		 * @param resource ���\�[�X
		 */
		IndexBuffer(ID3D12Resource2* resource);
		/** �f�X�g���N�^ */
		~IndexBuffer();

	public:
		/** �C���f�b�N�X�̍X�V 
		 * @param index �C���f�b�N�X�f�[�^
		 */
		void UpdateIndex(const void* index) const;

	public:
		/** �C���f�b�N�X���̎擾 
		 * @return �C���f�b�N�X��
		 */
		std::uint64_t GetNum(void) const;
		/** �C���f�b�N�X���̃Z�b�g
		 * @param index_num �C���f�b�N�X��
		 */
		void SetNum(const std::uint64_t& index_num);

	private:
		/* �C���f�b�N�X�� */
		std::uint64_t num{ 0 };
	};
}
