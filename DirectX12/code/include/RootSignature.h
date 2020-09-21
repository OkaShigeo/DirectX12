#pragma once
#include "BaseObject.h"
#include "BaseSubObject.h"

namespace Dx12
{
	class ShaderCompiler;

	class RootSignature :
		public BaseObject<ID3D12RootSignature>, BaseSubObject<D3D12_STATE_SUBOBJECT>
	{
	public:
		/** ディスクリプタ範囲の取得
		 * @param type ディスクリプタ範囲タイプ
		 * @param index レジスター番号
		 * @return ディスクリプタ範囲
		 * @param descriptor_num バインドできる数
		 * @param space レジスター空間
		 */
		static D3D12_DESCRIPTOR_RANGE1 GetRange(const D3D12_DESCRIPTOR_RANGE_TYPE& type, const std::uint32_t& index, const std::uint32_t& descriptor_num = 1, const std::uint32_t& space = 0);
		/* パラメータの取得
		 * @param type パラメータタイプ
		 * @param index レジスター番号
		 * @param range ディスクリプタ範囲の配列
		 * @param 定数値
		 * @param space レジスター空間
		 * @return ルートシグネチャパラメータ
		 */
		static D3D12_ROOT_PARAMETER1 GetParam(const D3D12_ROOT_PARAMETER_TYPE& type, const std::uint32_t& index, const std::vector<D3D12_DESCRIPTOR_RANGE1>& ranges = {}, const std::uint32_t& value = 0, const std::uint32_t& space = 0);
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
		static ID3D12RootSignature* CreateRootSignature(const D3D12_ROOT_SIGNATURE_FLAGS& flag, const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler = {});

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
		/** サブオブジェクト情報のセット
		 * @param type サブオブジェクトタイプ
		 */
		void SetConfig(const D3D12_STATE_SUBOBJECT_TYPE& type);
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 */
		virtual void AddSubObject(SubObject* sub) override;
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 * @param type サブオブジェクトタイプ
		 */
		void AddSubObject(SubObject* sub, const D3D12_STATE_SUBOBJECT_TYPE& type);
		/** サブオブジェクトに追加
		 * @param sub 追加先サブオブジェクト
		 * @param type サブオブジェクトタイプ
		 * @param func_name 関連付け関数名
		 */
		void AddSubObject(SubObject* sub, const D3D12_STATE_SUBOBJECT_TYPE& type, const std::vector<Str::String>& func_name);
	};
}
