#pragma once
#include "Window.h"
#include "Device.h"
#include "CommandAllocator.h"
#include "CommandList.h"
#include "CommandQueue.h"
#include "Fence.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"
#include "Resource.h"
#include "SubObject.h"
#include "ShaderCompiler.h"
#include "RootSignature.h"
#include "GraphicsPipeline.h"
#include "ComputePipeline.h"
#include "RaytracingPipeline.h"
#include "AccelerationStructure.h"
#include "HitGroup.h"

namespace Dx12
{
	class Runtime
	{
	private:
		/** �R���X�g���N�^ */
		Runtime();
		/** �f�X�g���N�^ */
		~Runtime();
		/** �R�s�[�R���X�g���N�^ */
		Runtime(const Runtime&) = delete;

	public:
		/** �������ς݂��m�F
		 * @return true:�������ς� / false:��������
		 */
		static bool IsInitialized(void);
		/** ������ 
		 * @param size �E�B���h�E�T�C�Y
		 * @param pos �E�B���h�E���W
		 */
		static void Initialize(const Math::Vec2& size, const Math::Vec2& pos = 0x80000000);
		/*�� �I�� */
		static void UnInitialize(void);
		/** �N���A */
		static void Clear(void);
		/** ���s 
		 * @param lists �R�}���h���X�g�̔z��
		 */
		static void Execution(const std::vector<CommandList*>& lists = {});
		/** �f�B�X�N���v�^�q�[�v�̃Z�b�g 
		 * @param heaps �f�B�X�N���v�^�q�[�v�̔z��
		 */
		static void SetDescriptorHeap(const std::vector<DescriptorHeap*>& heaps);
		/** �O���t�B�b�N�X�p���[�g�V�O�l�`���̃Z�b�g 
		 * @param root ���[�g�V�O�l�`��
		 */
		static void SetGraphicsRootSignature(const RootSignature* root);
		/** �v���O���}�u���p���[�g�V�O�l�`���̃Z�b�g 
		 * @param root ���[�g�V�O�l�`��
		 */
		static void SetComputeRootSignature(const RootSignature* root);
		/** �O���t�B�b�N�X�p�Ƀ��\�[�X���Z�b�g
		 * @param rsc ���\�[�X
		 * @param param_index ���[�g�V�O�l�`���̃p�����[�^�ԍ�
		 */
		static void SetGraphicsResource(const Resource* rsc, const std::uint32_t& param_index);
		/** �v���O���}�u���p�Ƀ��\�[�X���Z�b�g 
		 * @param rsc ���\�[�X
		 * @param param_index ���[�g�V�O�l�`���̃p�����[�^�ԍ�
		 */
		static void SetComputeResource(const Resource* rsc, const std::uint32_t& param_index);
		/** �O���t�B�b�N�X�p�p�C�v���C���̃Z�b�g 
		 * @param pipe �`��p�p�C�v���C��
		 */
		static void SetGraphicsPipeline(const GraphicsPipeline* pipe);
		/*�� �v���O���}�u���p�p�C�v���C���̃Z�b�g 
		 * @param pipe �R���s���[�g�p�p�C�v���C��
		 */
		static void SetComputePipeline(const ComputePipeline* pipe);
		/*�� ���\�[�X�o���A�̃Z�b�g 
		 * @param rsc ���\�[�X
		 * @param befor �J�ڑO�̃��\�[�X���
		 * @param after �J�ڌ�̃��\�[�X���
		 */
		static void SetResourceBarrier(const Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
		/** UAV���\�[�X�o���A�̃Z�b�g 
		 * @param rsc ���\�[�X
		 */
		static void SetUavResourceBarrier(const Resource* rsc);
		/** ���_���\�[�X�ɂ��v���~�e�B�u�̕`�� 
		 * @param rsc ���_���\�[�X
		 * @param vertex_num ���_��
		 * @param instance_num �C���X�^���X��
		 * @param topology �g�|���W�[
		 */
		static void DrawVertexInstance(const Resource* rsc, const std::uint32_t& vertex_num, const std::uint32_t& instance_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		/** �C���f�b�N�X���\�[�X�̂��v���~�e�B�u�̕`�� 
		 * @param vertex ���_���\�[�X
		 * @param vertex_num ���_��
		 * @param index �C���f�b�N�X���\�[�X
		 * @param index_num �C���f�b�N�X��
		 * @param offset ���_�̃I�t�Z�b�g
		 * @param instance_num �C���X�^���X��
		 * @param topology �g�|���W�[
		 */
		static void DrawIndexInstance(const Resource* vertex, const std::uint32_t& vertex_num, const Resource* index, const std::uint32_t& index_num, const std::uint32_t& offset = 0, const std::uint32_t& instance_num = 1, const D3D12_PRIMITIVE_TOPOLOGY& topology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		/*�� ���\�[�X�f�[�^�̃R�s�[
		 * @param dst �R�s�[��̃��\�[�X
		 * @param src �R�s�[���̃��\�[�X
		 */
		static void CopyResource(const Resource* dst, const Resource* src);
		/** ���\�[�X�f�[�^�̃R�s�[(�o�b�t�@) 
		 * @param dst �R�s�[��̃��\�[�X
		 * @param src �R�s�[���̃��\�[�X
		 * @param size �R�s�[�T�C�Y
		 * @param dst_offset �R�s�[��̃f�[�^�̃I�t�Z�b�g
		 * @param src_offset �R�s�[���̃f�[�^�̃I�t�Z�b�g
		 */
		static void CopyBufferRegion(const Resource* dst, const Resource* src, const std::uint64_t& size, const std::uint32_t& dst_offset = 0, const std::uint32_t& src_offset = 0);
		/** ���\�[�X�f�[�^�̃R�s�[(�e�N�X�`��) 
		 * @param dst �R�s�[��̃��\�[�X
		 * @param src �R�s�[���̃��\�[�X
		 * @param information �T�u���\�[�X���̔z��
		 * @param offset �T�u���\�[�X�ԍ��̃I�t�Z�b�g
		 */
		static void CopyTextureRegion(const Resource* dst, const Resource* src, const std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>& information, const std::uint32_t& offset = 0);
		/*�� �v���O���}�u���V�F�[�_�̎��s
		 * @param thread_x �X���b�h�O���[�vX
		 * @param thread_x �X���b�h�O���[�vY
		 * @param thread_x �X���b�h�O���[�vZ
		 */
		static void Dispatch(const std::uint64_t& thread_x, const std::uint64_t& thread_y = 1, const std::uint64_t& thread_z = 1);

	public:
		/** �E�B���h�E�̎擾
		 * @return �E�B���h�E
		 */
		static Window* GetWindow(void);
		/** �f�o�C�X�̎擾
		 * @return �f�o�C�X
		 */
		static Device* GetDevice(void);
		/** �r���[�|�[�g�T�C�Y�̎擾
		 * @return �r���[�|�[�g�T�C�Y
		 */
		static Math::Vec2 GetViewportSize(void);

		/*----------�I�y���[�^�I�[�o�[���[�h�֐�----------*/
	private:
		void operator=(const Runtime&) = delete;


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
		static DescriptorHeap* heap;
		/* ���\�[�X */
		static std::vector<Resource*>render_resource;
	};
}
