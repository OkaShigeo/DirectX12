#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class ShaderCompiler;
	class RootSignature;
	class Resource;

	class Device :
		public BaseObject<ID3D12Device6>
	{
	public:
		/** �f�o�C�X�̐��� 
		 * @return �f�o�C�X
		 */
		static ID3D12Device6* CreateDevice(void);

	public:
		/*�� �R���X�g���N�^ */
		Device();
		/** �R���X�g���N�^
		 * @param device �f�o�C�X
		 */
		Device(ID3D12Device6* device);
		/** �f�X�g���N�^ */
		~Device();

	public:
		/** �R�}���h�A���P�[�^�̐���
		 * @param type �R�}���h�^�C�v
		 * @return �R�}���h�A���P�[�^
		 */
		ID3D12CommandAllocator* CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT) const;
		/** �R�}���h���X�g�̐���
		 * @param type �R�}���h�^�C�v
		 * @return �R�}���h���X�g
		 */
		ID3D12GraphicsCommandList5* CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT) const;
		/** �R�}���h�L���[�̐���
		 * @param type �R�}���h�^�C�v
		 * @return �R�}���h�L���[
		 */
		ID3D12CommandQueue* CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT) const;
		/** �t�F���X�̐���
		 * @param count �t�F���X�J�E���g
		 * @return �t�F���X
		 */
		ID3D12Fence1* CreateFence(const std::uint64_t& count = 0) const;
		/** �f�B�X�N���v�^�q�[�v�̐���
		 * @param type �f�B�X�N���v�^�q�[�v�^�C�v
		 * @param rsc_num ���\�[�X��
		 * @param flag �f�B�X�N���v�^�q�[�v�̃t���O
		 * @return �f�B�X�N���v�^�q�[�v
		 */
		ID3D12DescriptorHeap* CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		/** �R���s���[�g�p�C�v���C���̐���
		 * @param root ���[�g�V�O�l�`��
		 * @param shader �V�F�[�_���
		 * @return �p�C�v���C��
		 */
		ID3D12PipelineState* CreateComputePipeline(const RootSignature* root, const ShaderCompiler* shader);
		/** �����_�[�^�[�Q�b�g�̐���
		 * @param resource ���\�[�X
		 */
		void CreateRenderTarget(const Resource* resource) const;
		/** �R���X�^���g�o�b�t�@�r���[�̐���
		 * @param resource ���\�[�X
		 */
		void CreateConstantBufferView(const Resource* resource) const;
		/** �V�F�[�_���\�[�X�r���[�̐���
		 * @param resource ���\�[�X
		 */
		void CreateShaderResourceView(const Resource* resource) const;
		/** �A���I�[�_�[�A�N�Z�X�r���[�̐���(�o�b�t�@)
		 * @param resource ���\�[�X
		 * @param element_num �v�f��
		 */
		void CreateUnorderAccessView(Resource* resource, const std::uint64_t& element_num) const;
		/** �A���I�[�_�[�A�N�Z�X�r���[�̐���(�e�N�X�`��)
		 * @param resource ���\�[�X
		 */
		void CreateUnorderAccessView(Resource* resource) const;
		/** �����\���̃v���r���h���̎擾
		 * @param input �r���h���͏�� 
		 * @return �v���r���h���
		 */
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO GetAccelerationStructurePrebuildInfo(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input) const;
	};
}
