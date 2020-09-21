#pragma once
#include "BaseSubObject.h"

namespace Dx12
{
	class ShaderConfig :
		public BaseSubObject<D3D12_RAYTRACING_SHADER_CONFIG>
	{
	public:
		/** �R���X�g���N�^ */
		ShaderConfig();
		/** �R���X�g���N�^
		 * @param payload �y�C���[�h�T�C�Y
		 * @param attributes �q�b�g�ڍ׃f�[�^�T�C�Y
		 */
		ShaderConfig(const std::uint64_t& payload, const std::uint64_t& attributes = sizeof(float) * 2);
		/** �R���X�g���N�^
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param payload �y�C���[�h�T�C�Y
		 * @param attributes �q�b�g�ڍ׃f�[�^�T�C�Y
		 */
		ShaderConfig(SubObject* sub, const std::uint64_t& payload, const std::uint64_t& attributes = sizeof(float) * 2);
		/** �R���X�g���N�^
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param payload �y�C���[�h�T�C�Y
		 * @param func_name �֘A�t���֐���
		 * @param attributes �q�b�g�ڍ׃f�[�^�T�C�Y
		 */
		ShaderConfig(SubObject* sub, const std::uint64_t& payload, const std::vector<Str::String>& func_name, const std::uint64_t& attributes = sizeof(float) * 2);
		/** �f�X�g���N�^ */
		~ShaderConfig();

	public:
		/** �T�u�I�u�W�F�N�g���̃Z�b�g
		 * @param payload �y�C���[�h�T�C�Y
		 * @param attributes �q�b�g�ڍ׃f�[�^�T�C�Y
		 */
		void SetConfig(const std::uint64_t& payload, const std::uint64_t& attributes = sizeof(float) * 2);
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param payload �y�C���[�h�T�C�Y
		 * @param attributes �q�b�g�ڍ׃f�[�^�T�C�Y
		 */
		void AddSubObject(SubObject* sub, const std::uint64_t& payload, const std::uint64_t& attributes = sizeof(float) * 2);
		/** �T�u�I�u�W�F�N�g�ɒǉ�
		 * @param sub �ǉ���T�u�I�u�W�F�N�g
		 * @param payload �y�C���[�h�T�C�Y
		 * @param func_name �֘A�t���֐���
		 * @param attributes �q�b�g�ڍ׃f�[�^�T�C�Y
		 */
		void AddSubObject(SubObject* sub, const std::uint64_t& payload, const std::vector<Str::String>& func_name, const std::uint64_t& attributes = sizeof(float) * 2);
	}; 
}
