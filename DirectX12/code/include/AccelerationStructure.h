#pragma once
#include "BaseObject.h"
#include <tuple>
#include <vector>

namespace Dx12
{
	class Resource;

	class AccelerationStructure
	{
	public:
		/*�� �{�g�����x���̐��� 
		 * @param vertex ���_���\�[�X
		 * @param vertex_num ���_��
		 * @param transform_matrix ���_�ϊ��s��̉��z�A�h���X
		 * @param index �C���f�b�N�X���\�[�X
		 * @param index_num �C���f�b�N�X��
		 */
		static std::tuple<ID3D12Resource2*, ID3D12Resource2*, std::vector<D3D12_RAYTRACING_GEOMETRY_DESC>> CreateBottomLevel(const std::vector<Resource*>& vertex, const std::vector<std::uint64_t>& vertex_num, const std::uint64_t& transform_matrix_addr = 0, const std::vector<Resource*>& index = {}, const std::vector<std::uint64_t>& index_num = {});
		/** �g�b�v���x���̐��� */
		static std::tuple<ID3D12Resource2*, ID3D12Resource2*, ID3D12Resource2*> CreateTopLevel(const std::uint64_t& instance_num = 1);

	public:
		/** �R���X�g���N�^ */
		AccelerationStructure();
		/** �R���X�g���N�^ */
		AccelerationStructure(const std::tuple<ID3D12Resource2*, ID3D12Resource2*, std::vector<D3D12_RAYTRACING_GEOMETRY_DESC>>& value);
		/** �f�X�g���N�^ */
		~AccelerationStructure();

	public:
		/*�� �������̉�� */
		void Release(void);
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
		/** �r���h���̎擾
		 * @return �r���h���
		 */
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS GetBuildInfo(void) const;

		/*----------�I�y���[�^�I�[�o�[���[�h�֐�----------*/
	public:
		void operator=(const std::tuple<ID3D12Resource2*, ID3D12Resource2*, std::vector<D3D12_RAYTRACING_GEOMETRY_DESC>>& value)
		{
			Release();

			scratch   = std::get<0>(value);
			result    = std::get<1>(value);
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
			result   = std::get<1>(value);
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
		/* ���U���g���\�[�X */
		ID3D12Resource2* result{ nullptr };
		/* �C���X�^���X���\�[�X */
		ID3D12Resource2* instance{ nullptr };
		/* �W�I���g����� */
		std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> geometory;
		/* �r���h���͏�� */
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	};
}
