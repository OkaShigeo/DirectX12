#pragma once
#include "Resource.h"

namespace Dx12
{
	class VertexBuffer :
		public Resource
	{
	public:
		/** ���_�o�b�t�@�̐��� 
		 * @param vertex ���_�f�[�^
		 */
		static ID3D12Resource2* CreateVertexBuffer(void* vertex);

	public:
		/** �R���X�g���N�^ */
		VertexBuffer();
		/** �R���X�g���N�^ 
		 * @param vertex ���_�f�[�^
		 * @param vertex_num ���_��
		 */
		VertexBuffer(void* vertex, const std::uint64_t& vertex_num);
		/** �R���X�g���N�^
		 * @param resource ���\�[�X
		 */
		VertexBuffer(ID3D12Resource2* resource);
		/** �f�X�g���N�^ */
		~VertexBuffer();

	public:
		/** ���_�̍X�V  
		 * @param vertex ���_�f�[�^
		 */
		void UpdateVertex(const void* vertex) const;

	public:
		/** ���_���̎擾 
		 * @return ���_��
		 */
		std::uint64_t GetNum(void) const;
		/** ���_���̃Z�b�g
		 * @param vertex_num ���_��
		 */
		void SetNum(const std::uint64_t& vertex_num);

	private:
		/* ���_�� */
		std::uint64_t num{ 0 };
	};
}
