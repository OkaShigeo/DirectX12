#pragma once
#include "BaseObject.h"
#include "SubObject.h"
#include <string>
#include <vector>

namespace Dx12
{
	class ShaderCompiler;

	class RootSignature :
		public BaseObject<ID3D12RootSignature>, SubObject
	{
	public:
		/* ���[�g�V�O�l�`���̐��� */
		static ID3D12RootSignature* CreateRootSignature(const ShaderCompiler* shader);
		/* ���[�g�V�O�l�`���̐��� */
		static ID3D12RootSignature* CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param,
			const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler);

	public:
		/* �R���X�g���N�^ */
		RootSignature();
		RootSignature(const ShaderCompiler* shader);
		RootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler = {});
		RootSignature(ID3D12RootSignature* root);
		/* �f�X�g���N�^ */
		~RootSignature();

	public:
		/* �T�u�I�u�W�F�N�g�̒ǉ� */
		void AddSubObj(const D3D12_STATE_SUBOBJECT_TYPE& type, const std::vector<std::wstring>& func_name);

	private:
		/* �T�u�I�u�W�F�N�g�̊֘A�t�� */
		D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association{};
	};
}
