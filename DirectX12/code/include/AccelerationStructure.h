#pragma once
#include "Resource.h"
#include <tuple>

namespace Dx12
{
	class VertexBuffer;
	class IndexBuffer;

	class AccelerationStructure : 
		public Resource
	{
	public:
		/*�� �{�g�����x���̐��� 
		 * @param vertex ���_�o�b�t�@
		 * @param index �C���f�b�N�X�o�b�t�@
		 * @param transform_matrix ���_�ϊ��s��̉��z�A�h���X
		 */
		static std::tuple<ID3D12Resource2*, ID3D12Resource2*, std::vector<D3D12_RAYTRACING_GEOMETRY_DESC>> CreateBottomLevel(const std::vector<VertexBuffer*>& vertex, const std::vector<IndexBuffer*>& index = {}, const std::uint64_t& transform_matrix_addr = 0);
		/** �g�b�v���x���̐��� 
		 * @instance_num �C���X�^���X��
		 */
		static std::tuple<ID3D12Resource2*, ID3D12Resource2*, ID3D12Resource2*> CreateTopLevel(const std::uint64_t& instance_num = 1);

	public:
		/** �R���X�g���N�^ 
		 * @param vertex ���_�o�b�t�@
		 * @param index �C���f�b�N�X�o�b�t�@
		 * @param transform_matrix ���_�ϊ��s��̉��z�A�h���X
		 */
		AccelerationStructure(const std::vector<VertexBuffer*>& vertex, const std::vector<IndexBuffer*>& index = {}, const std::uint64_t& transform_matrix_addr = 0);
		/** �R���X�g���N�^
		 * @instance_num �C���X�^���X��
		 */
		AccelerationStructure(const std::uint64_t& instance_num = 1);
		/** �R���X�g���N�^ 
		 * @param value �����\���̒l
		 */
		template<typename T>
		AccelerationStructure(const std::tuple<ID3D12Resource2*, ID3D12Resource2*, T>& value);
		/** �f�X�g���N�^ */
		~AccelerationStructure();

	public:
		/*�� �������̉�� */
		void Release(void);
		/*�� �m�ۂ����o�b�t�@�̉�� */
		void ReleaseBuffer(void) const;
		/** �{�g�����x���̊m�F
		 * @return true:�{�g�����x�� / false:���̑�
		 */
		bool IsBottomLevel(void) const;
		/** �g�b�v���x���̊m�F
		 * @return true:�g�b�v���x�� / false:���̑�
		 */
		bool IsTopLevel(void) const;

	public:
		/** �X�N���b�`���\�[�X�̎擾
		 * @return �X�N���b�`���\�[�X
		 */
		ID3D12Resource2* GetScratch(void) const;
		/** ���U���g���\�[�X�̎擾
		 * @return ���U���g���\�[�X
		 */
		ID3D12Resource2* GetResult(void) const;
		/** �C���X�^���X���\�[�X�̎擾
		 * @return �C���X�^���X���\�[�X
		 */
		ID3D12Resource2* GetInstance(void) const;
		/** �C���X�^���X���̎擾
		 * @return �C���X�^���X��
		 */
		std::uint64_t GetInstanceNum(void) const;
		/** �r���h���̎擾
		 * @return �r���h���
		 */
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS GetBuildInput(void) const;
		/** �m�ۂ����o�b�t�@�̎擾
		 * @return �m�ۂ����o�b�t�@�[�̃|�C���^
		 */
		D3D12_RAYTRACING_INSTANCE_DESC* GetBuffer(void) const;

		/*----------�I�y���[�^�I�[�o�[���[�h�֐�----------*/
	public:
		void operator=(const std::tuple<ID3D12Resource2*, ID3D12Resource2*, std::vector<D3D12_RAYTRACING_GEOMETRY_DESC>>& value)
		{
			Release();

			scratch   = std::get<0>(value);
			obj       = std::get<1>(value);
			geometory = std::get<2>(value);

			input.DescsLayout    = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
			input.Flags          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
			input.NumDescs       = std::uint32_t(geometory.size());
			input.pGeometryDescs = geometory.data();
			input.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
		}
		void operator=(const std::tuple<ID3D12Resource2*, ID3D12Resource2*, ID3D12Resource2*>& value)
		{
			scratch  = std::get<0>(value);
			obj      = std::get<1>(value);
			instance = std::get<2>(value);

			input.DescsLayout   = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
			input.Flags         = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
			input.NumDescs      = std::uint32_t(instance->GetDesc().Width / sizeof(D3D12_RAYTRACING_INSTANCE_DESC));
			input.InstanceDescs = instance->GetGPUVirtualAddress();
			input.Type          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
		}

	private:
		/* �X�N���b�`���\�[�X */
		ID3D12Resource2* scratch{ nullptr };
		/* �C���X�^���X���\�[�X */
		ID3D12Resource2* instance{ nullptr };
		/* �r���h���͏�� */
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
		/* �W�I���g����� */
		std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> geometory;
	};
}
