#pragma once
#include "BaseObject.h"
#include "Vector.h"
#include <vector>

namespace Dx12
{
	class CommandAllocator;
	class Descriptor;
	class Resource;
	class RootSignature;
	class GraphicsPipeline;
	class ComputePipeline;

	class CommandList :
		public BaseObject<ID3D12GraphicsCommandList5>
	{
	public:
		/* �R���X�g���N�^ */
		CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		CommandList(ID3D12GraphicsCommandList5* list);
		/* �f�X�g���N�^ */
		~CommandList();

	public:
		/* ���Z�b�g */
		void Reset(const CommandAllocator* allocator) const;
		/* ���� */
		void Close(void) const;
		/* �r���[�|�[�g�̃Z�b�g */
		void SetViewport(const Vec2& size, const float& depth = 1.0f) const;
		/* �V�U�[�̃Z�b�g */
		void SetScissors(const Vec2& size) const;
		/* ���\�[�X�o���A�̃Z�b�g */
		void SetRscBarrier(const Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after) const;
		/* UAV���\�[�X�o���A�̃Z�b�g */
		void SetUavRscBarrier(const Resource* rsc) const;
		/* �����_�[�^�[�Q�b�g�r���[�̃N���A */
		void ClearRenderTargetView(const Resource* rsc, const float color[4]) const;
		/* �f�B�X�N���v�^�q�[�v�̃Z�b�g */
		void SetDescriptorHeap(const std::vector<Descriptor*>& heap);
		/* �O���t�B�b�N�X�p���[�g�V�O�l�`���̃Z�b�g */
		void SetGraphicsRootSignature(const RootSignature* root) const;
		/* �v���O���}�u���p���[�g�V�O�l�`���̃Z�b�g */
		void SetComputeRootSignature(const RootSignature* root) const;
		/* �O���t�B�b�N�X�p�Ƀ��\�[�X���Z�b�g */
		void SetGraphicsResource(const Resource* rsc, const std::uint32_t& param_index) const;
		/* �v���O���}�u���p�Ƀ��\�[�X���Z�b�g */
		void SetComputeResource(const Resource* rsc, const std::uint32_t& param_index) const;
		/* �O���t�B�b�N�X�p�p�C�v���C���̃Z�b�g */
		void SetGraphicsPipeline(const GraphicsPipeline* pipe) const;
		/* �v���O���}�u���p�p�C�v���C���̃Z�b�g */
		void SetComputePipeline(const ComputePipeline* pipe) const;
		/* ���_���\�[�X�ɂ��v���~�e�B�u�̕`�� */
		void DrawVertexInstance(const Resource* rsc, const std::uint32_t& vertex_num, const std::uint32_t& instance_num = 1,
			const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		/* �C���f�b�N�X���\�[�X�ɂ��v���~�e�B�u�̕`�� */
		void DrawIndexInstance(const Resource* vertex, const std::uint32_t& vertex_num, const Resource* index, const std::uint32_t& index_num, const std::uint32_t& offset = 0,
			const std::uint32_t& instancex_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		/* ���\�[�X�f�[�^�̃R�s�[ */
		void CopyResource(const Resource* dst, const Resource* src) const;
		/* �v���O���}�u���V�F�[�_�̎��s */
		void Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y = 1, const std::uint64_t& thread_z = 1) const;

	private:
		/* �R�}���h���X�g�̐��� */
		ID3D12GraphicsCommandList5* CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);
	};
}
