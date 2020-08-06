#pragma once
#include "BaseObject.h"
#include <vector>

namespace Dx12
{
	class ShaderCompiler;

	class RootSignature :
		public BaseObject<ID3D12RootSignature>
	{
	public:
		/* �R���X�g���N�^ */
		RootSignature(const ShaderCompiler* shader);
		RootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler = {});
		RootSignature(ID3D12RootSignature* root);
		/* �f�X�g���N�^ */
		~RootSignature();

	private:
		/* ���[�g�V�O�l�`���̐��� */
		ID3D12RootSignature* CreateRootSignature(const ShaderCompiler* shader);
		/* ���[�g�V�O�l�`���̐��� */
		ID3D12RootSignature* CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param,
			const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler);
	};
}
