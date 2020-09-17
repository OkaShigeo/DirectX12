#pragma once
#include "BaseObject.h"
#include <vector>

namespace Dx12
{
	class ShaderCompiler;
	class RootSignature;
	class DescriptorHeap;
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
		ID3D12DescriptorHeap* CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) const;
		/** ���\�[�X�̐���(�o�b�t�@)
		 * @param state ���\�[�X���
		 * @param prop �q�[�v�v���p�e�B
		 * @param size �o�b�t�@�[�T�C�Y
		 * @param flag ���\�[�X�t���O
		 * @param clear �N���A���
		 * @return �o�b�t�@���\�[�X
		 */
		ID3D12Resource2* CreateBufferResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr) const;
		/** ���\�[�X�̐���(�e�N�X�`��)
		 * @param state ���\�[�X���
		 * @param prop �q�[�v�v���p�e�B
		 * @param format �e�N�X�`���t�H�[�}�b�g
		 * @param width �e�N�X�`���̉���
		 * @param height �e�N�X�`���̗���
		 * @param flag ���\�[�X�t���O
		 * @param clear �N���A���
		 * @return �e�N�X�`�����\�[�X
		 */
		ID3D12Resource2* CreateTextureResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format, const std::uint64_t& width, const std::uint32_t& height, const D3D12_RESOURCE_FLAGS& flag = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr) const;
		/** ���[�g�V�O�l�`���̐���
		 * @param shader �V�F�[�_���
		 * @return ���[�g�V�O�l�`��
		 */
		ID3D12RootSignature* CreateRootSignature(const ShaderCompiler* shader) const;
		/** �O���t�B�b�N�X�p�C�v���C���̐���
		 * @param input ���͗v�f�̏��
		 * @param root ���[�g�V�O�l�`��
		 * @param shader �V�F�[�_���
		 * @param vertex ���_�V�F�[�_���
		 * @param pixel �s�N�Z���V�F�[�_���
		 * @param geometory �W�I���g���[�V�F�[�_���
		 * @param topology �v���~�e�B�u�̃g�|���W�[�̎��
		 * @param depth �[�x�l�Z�b�g�̃t���O
		 * @return �p�C�v���C��
		 */
		ID3D12PipelineState* CreateGraphicsPipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const RootSignature* root, const ShaderCompiler* vertex, const ShaderCompiler* pixel, const ShaderCompiler* geometory = nullptr, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, const bool& depth = false) const;
		/** �R���s���[�g�p�C�v���C���̐���
		 * @param root ���[�g�V�O�l�`��
		 * @param shader �V�F�[�_���
		 * @return �p�C�v���C��
		 */
		ID3D12PipelineState* CreateComputePipeline(const RootSignature* root, const ShaderCompiler* shader) const;
		/** ���C�g���[�V���O�p�C�v���C���̐���
		 * @return �p�C�v���C��
		 */
		ID3D12StateObject* CreateRaytracingPipeline(void) const;
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
		/** �f�B�X�N���v�^�q�[�v�̃T�C�Y�̎擾
		 * @param heap �f�B�X�N���v�^�q�[�v
		 * @return �q�[�v�T�C�Y
		 */
		std::uint64_t GetHeapSize(const DescriptorHeap* heap) const;
		/** �T�u���\�[�X���̃R�s�[
		 * @param resource ���\�[�X
		 * @param information �T�u���\�[�X���̃R�s�[��
		 * @param offset �T�u���\�[�X�ԍ��̃I�t�Z�b�g
		 * @return �R�s�[�T�C�Y
		 */
		std::uint64_t CopySubResourceInfo(const Resource* resource, D3D12_PLACED_SUBRESOURCE_FOOTPRINT* information = nullptr, const std::uint32_t& offset = 0) const;
		/** �����\���̃v���r���h���̎擾
		 * @param input �r���h���͏�� 
		 * @return �v���r���h���
		 */
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO GetAccelerationStructurePrebuildInfo(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input) const;
	};
}
