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
		/* ルートシグネチャの生成 */
		static ID3D12RootSignature* CreateRootSignature(const ShaderCompiler* shader);
		/* ルートシグネチャの生成 */
		static ID3D12RootSignature* CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param,
			const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler);

	public:
		/* コンストラクタ */
		RootSignature();
		RootSignature(const ShaderCompiler* shader);
		RootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler = {});
		RootSignature(ID3D12RootSignature* root);
		/* デストラクタ */
		~RootSignature();

	public:
		/* サブオブジェクトの追加 */
		void AddSubObj(const D3D12_STATE_SUBOBJECT_TYPE& type, const std::vector<std::wstring>& func_name);

	private:
		/* サブオブジェクトの関連付け */
		D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association{};
	};
}
