#pragma once
#include "Window.h"
#include "Device.h"
#include "CommandAllocator.h"
#include "CommandList.h"
#include "CommandQueue.h"
#include "Fence.h"
#include "SwapChain.h"
#include "Descriptor.h"
#include "Resource.h"
#include "ShaderCompiler.h"
#include "RootSignature.h"
#include "GraphicsPipeline.h"
#include "ComputePipeline.h"

namespace Dx12
{
	class Runtime
	{
	public:
		/* ������ */
		static void Initialize(const Dx12::Vec2& size, const Dx12::Vec2& pos = 0x80000000);
		/* �I�� */
		static void UnInitialize(void);
		/* �N���A */
		static void Clear(void);
		/* ���s */
		static void Execution(const std::vector<CommandList*>& lists = {});
		/* �f�B�X�N���v�^�q�[�v�̃Z�b�g */
		static void SetDescriptorHeap(const std::vector<Descriptor*>& heap);
		/* �O���t�B�b�N�X�p���[�g�V�O�l�`���̃Z�b�g */
		static void SetGraphicsRootSignature(const RootSignature* root);
		/* �v���O���}�u���p���[�g�V�O�l�`���̃Z�b�g */
		static void SetComputeRootSignature(const RootSignature* root);
		/* �O���t�B�b�N�X�p�Ƀ��\�[�X���Z�b�g */
		static void SetGraphicsResource(const Resource* rsc, const std::uint32_t& param_index);
		/* �v���O���}�u���p�Ƀ��\�[�X���Z�b�g */
		static void SetComputeResource(const Resource* rsc, const std::uint32_t& param_index);
		/* �O���t�B�b�N�X�p�p�C�v���C���̃Z�b�g */
		static void SetGraphicsPipeline(const GraphicsPipeline* pipe);
		/* �v���O���}�u���p�p�C�v���C���̃Z�b�g */
		static void SetComputePipeline(const ComputePipeline* pipe);
		/* ���\�[�X�o���A�̃Z�b�g */
		static void SetRscBarrier(const Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
		/* UAV���\�[�X�o���A�̃Z�b�g */
		static void SetUavRscBarrier(const Resource* rsc);
		/* ���_���\�[�X�ɂ��v���~�e�B�u�̕`�� */
		static void DrawVertexInstance(const Resource* rsc, const std::uint32_t& vertex_num, const std::uint32_t& instance_num = 1, 
			const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		/* �C���f�b�N�X���\�[�X�̂��v���~�e�B�u�̕`�� */
		static void DrawIndexInstance(const Resource* vertex, const std::uint32_t& vertex_num, const Resource* index, const std::uint32_t& index_num, const std::uint32_t& offset = 0,
			const std::uint32_t& instance_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		/* ���\�[�X�f�[�^�̃R�s�[ */
		static void CopyResource(const Resource* dst, const Resource* src);

		/* �v���O���}�u���V�F�[�_�̎��s */
		static void Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y = 1, const std::uint64_t& thread_z = 1);

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
		/* �R�}���h�A���P�[�^ */
		static CommandAllocator* allocator;
		/* �R�}���h���X�g */
		static CommandList* list;
		/* �R�}���h�L���[ */
		static CommandQueue* queue;
		/* �t�F���X */
		static Fence* fence;
		/* �X���b�v�`�F�C�� */
		static SwapChain* swap;
		/* �f�B�X�N���v�^�q�[�v */
		static Descriptor* heap;
		/* ���\�[�X */
		static std::vector<Resource*>rsc;
	};
}
