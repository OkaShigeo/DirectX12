#pragma once
#include "Resource.h"

namespace Dx12
{
	class ShaderTable :
		public Resource
	{
	public:
		/** �V�F�[�_�e�[�u�����\�[�X�̐���
		 * @param recode_num �V�F�[�_���R�[�h��
		 * @return ���\�[�X
		 */
		static ID3D12Resource2* CreateShaderTableResource(const std::uint64_t& recode_num);
		/** �V�F�[�_���R�[�h�T�C�Y�̎擾
		 * @return �V�F�[�_���R�[�h�T�C�Y
		 */
		static std::uint64_t GetShaderRecodeSize(void);

	public:
		/*�� �R���X�g���N�^ */
		ShaderTable();
		/** �R���X�g���N�^
		 * @param recode_num �V�F�[�_���R�[�h��
		 */
		ShaderTable(const std::uint64_t& recode_num);
		/** �R���X�g���N�^ 
		 * @param resource ���\�[�X
		 */
		ShaderTable(ID3D12Resource2* resource);
		/** �f�X�g���N�^ */
		~ShaderTable();
	};
}
