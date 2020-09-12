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
		/** ルートシグネチャの生成 
		 * @param shader シェーダ情報
		 * @return ルートシグネチャ
		 */
		static ID3D12RootSignature* CreateRootSignature(const ShaderCompiler* shader);
		/** ルートシグネチャの生成 
		 * @param flag ルートシグネチャのフラグ
		 * @param param ルートシグネチャのパラメータ
		 * @param sampler サンプラー
		 * @return ルートシグネチャ
		 */
		static ID3D12RootSignature* CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param,
			const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler);

	public:
		/** コンストラクタ */
		RootSignature();
		/** コンストラクタ
		 * @param shader シェーダ情報
		 */
		RootSignature(const ShaderCompiler* shader);
		/** コンストラクタ
		 * @param flag ルートシグネチャのフラグ
		 * @param param ルートシグネチャのパラメータ
		 * @param sampler サンプラー
		 */
		RootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler = {});
		/** コンストラクタ
		 * @param root ルートシグネチャ
		 */
		RootSignature(ID3D12RootSignature* root);
		/** デストラクタ */
		~RootSignature();

	public:
		/** サブオブジェクトの追加
		 * @param type サブオブジェクトの種別
		 * @param func_name 関数名
		 */
		void AddSubObj(const D3D12_STATE_SUBOBJECT_TYPE& type, const std::vector<std::wstring>& func_name);

	private:
		/* サブオブジェクトの関連付け */
		D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association{};
	};
}
