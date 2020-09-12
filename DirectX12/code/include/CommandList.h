#pragma once
#include "BaseObject.h"
#include "Vector.h"
#include <vector>

namespace Dx12
{
	class CommandAllocator;
	class DescriptorHeap;
	class Resource;
	class RootSignature;
	class GraphicsPipeline;
	class ComputePipeline;

	class CommandList :
		public BaseObject<ID3D12GraphicsCommandList5>
	{
	public:
		/** �R�}���h���X�g�̐��� 
		 * @param type �R�}���h���X�g�̎��
		 * @return �R�}���h���X�g
		 */
		static ID3D12GraphicsCommandList5* CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);

	public:
		/** �R���X�g���N�^ 
		 * @param type �R�}���h���X�g�̎��
		 */
		CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		/** �R���X�g���N�^
		 * @param list �R�}���h���X�g
		 */
		CommandList(ID3D12GraphicsCommandList5* list);
		/*�� �f�X�g���N�^ */
		~CommandList();

	public:
		/** �R�}���h���X�g�̓��e�����Z�b�g
		 * @param allocator �R�}���h�A���P�[�^
		 */
		void Reset(const CommandAllocator* allocator) const;
		/** �R�}���h���X�g�̏������݂���� */
		void Close(void) const;
		/** �r���[�|�[�g�̃Z�b�g 
		 * @param size �E�B���h�E�T�C�Y
		 * @param depth �ő�[�x�l
		 */
		void SetViewport(const Math::Vec2& size, const float& depth = 1.0f) const;
		/** �V�U�[�̃Z�b�g
		 * @param size �E�B���h�E�T�C�Y
		 */
		void SetScissors(const Math::Vec2& size) const;
		/** ���\�[�X�o���A�̃Z�b�g 
		 * @param rsc ���\�[�X
		 * @param befor �J�ڑO�̃��\�[�X���
		 * @param after �J�ڌ�̃��\�[�X���
		 */
		void SetRscBarrier(const Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after) const;
		/** UAV���\�[�X�o���A�̃Z�b�g 
		 * @param ���\�[�X
		 */
		void SetUavRscBarrier(const Resource* rsc) const;
		/** �����_�[�^�[�Q�b�g�r���[�̃N���A 
		 * @param rsc ���\�[�X
		 * @param color �N���A�J���[
		 */
		void ClearRenderTargetView(const Resource* rsc, const float color[4]) const;
		/** �f�B�X�N���v�^�q�[�v�̃Z�b�g
		 * @param heap �f�B�X�N���v�^�q�[�v
		 */
		void SetDescriptorHeap(const std::vector<DescriptorHeap*>& heap);
		/** �O���t�B�b�N�X�p���[�g�V�O�l�`���̃Z�b�g 
		 * @param root ���[�g�V�O�l�`��
		 */
		void SetGraphicsRootSignature(const RootSignature* root) const;
		/** �v���O���}�u���p���[�g�V�O�l�`���̃Z�b�g
		 * @param root ���[�g�V�O�l�`��
		 */
		void SetComputeRootSignature(const RootSignature* root) const;
		/** �O���t�B�b�N�X�p�Ƀ��\�[�X���Z�b�g 
		 * @param rsc ���\�[�X
		 * @param param_index ���[�g�V�O�l�`���̃p�����[�^�ԍ�
		 */
		void SetGraphicsResource(const Resource* rsc, const std::uint32_t& param_index) const;
		/** �v���O���}�u���p�Ƀ��\�[�X���Z�b�g
		 * @param rsc ���\�[�X
		 * @param param_index ���[�g�V�O�l�`���̃p�����[�^�ԍ�
		 */
		void SetComputeResource(const Resource* rsc, const std::uint32_t& param_index) const;
		/** �O���t�B�b�N�X�p�p�C�v���C���̃Z�b�g 
		 * @param pipe �p�C�v���C��
		 */
		void SetGraphicsPipeline(const GraphicsPipeline* pipe) const;
		/** �v���O���}�u���p�p�C�v���C���̃Z�b�g 
		 * @param pipe �p�C�v���C��
		 */
		void SetComputePipeline(const ComputePipeline* pipe) const;
		/** ���_���\�[�X�ɂ��v���~�e�B�u�̕`�� 
		 * @param rsc ���_���\�[�X
		 * @param vertex_num ���_��
		 * @param instance_num �C���X�^���X��
		 * @param topology �v���~�e�B�u�̃g�|���W�[
		 */
		void DrawVertexInstance(const Resource* rsc, const std::uint32_t& vertex_num, const std::uint32_t& instance_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		/*�� �C���f�b�N�X���\�[�X�ɂ��v���~�e�B�u�̕`��
		 * @param vertex ���_���\�[�X
		 * @param vertex_num ���_��
		 * @param index �C���f�b�N�X���\�[�X
		 * @param index_num �C���f�b�N�X��
		 * @param instance_num �C���X�^���X��
		 * @param topology �v���~�e�B�u�̃g�|���W�[
		 */
		void DrawIndexInstance(const Resource* vertex, const std::uint32_t& vertex_num, const Resource* index, const std::uint32_t& index_num, const std::uint32_t& offset = 0, const std::uint32_t& instancex_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		/** ���\�[�X�f�[�^�̃R�s�[ 
		 * @param dst ���M��̃��\�[�X
		 * @param src ���M���̃��\�[�X
		 */
		void CopyResource(const Resource* dst, const Resource* src) const;
		/** ���\�[�X�f�[�^�̃R�s�[(�o�b�t�@)
		 * @param dst �R�s�[��̃��\�[�X
		 * @param src �R�s�[���̃��\�[�X
		 * @param size �R�s�[�T�C�Y
		 * @param dst_offset �R�s�[��̃f�[�^�̃I�t�Z�b�g
		 * @param src_offset �R�s�[���̃f�[�^�̃I�t�Z�b�g
		 */
		void CopyBufferRegion(const Resource* dst, const Resource* src, const std::uint64_t& size, const std::uint32_t& dst_offset = 0, const std::uint32_t& src_offset = 0);
		/** ���\�[�X�f�[�^�̃R�s�[(�e�N�X�`��)
		 * @param dst �R�s�[��̃��\�[�X
		 * @param src �R�s�[���̃��\�[�X
		 * @param information �T�u���\�[�X���̔z��
		 * @param offset �T�u���\�[�X�ԍ��̃I�t�Z�b�g
		 */
		void CopyTextureRegion(const Resource* dst, const Resource* src, const std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>& information, const std::uint32_t& offset = 0);
		/** �v���O���}�u���V�F�[�_�̎��s
		 * @param thread_x �X���b�h�O���[�v X
		 * @param thread_y �X���b�h�O���[�v Y
		 * @param thread_z �X���b�h�O���[�v Z
		 */
		void Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y = 1, const std::uint64_t& thread_z = 1) const;
	};
}
