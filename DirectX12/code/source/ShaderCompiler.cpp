#include "..\include\Runtime.h"
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

ID3DBlob* Dx12::ShaderCompiler::CompileFromFile(const std::string& file_path, const std::string& entry_name, const std::string& shader_model)
{
	auto path  = Str::String(file_path).GetUniCode();
	auto name  = Str::String(entry_name).GetUniCode();
	auto model = Str::String(shader_model).GetUniCode();

	Microsoft::WRL::ComPtr<IDxcLibrary>library = nullptr;
	auto hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcIncludeHandler>handle = nullptr;
	hr = library->CreateIncludeHandler(&handle);
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcBlobEncoding>encode = nullptr;
	hr = library->CreateBlobFromFile(path.c_str(), nullptr, &encode);
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcCompiler>compile = nullptr;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compile));
	assert(hr == S_OK);

	ID3DBlob* blob = nullptr;

	std::string fmt = file_path.substr(file_path.find_last_of(".") + 1, std::string::npos);
	if (fmt == "hlsl") {
		/* コンパイル */
		Microsoft::WRL::ComPtr<IDxcOperationResult>result = nullptr;
		hr = compile->Compile(encode.Get(), path.c_str(), name.c_str(), model.c_str(), nullptr, 0, nullptr, 0, handle.Get(), &result);
		assert(hr == S_OK);

		result->GetStatus(&hr);
		if (SUCCEEDED(hr))
		{
			hr = result->GetResult((IDxcBlob**)&blob);
		}
	}
	else {
		/* コンパイル済みデータの読み込み */
		hr = library->GetBlobAsUtf8((IDxcBlob*)blob, &encode);
		if (FAILED(hr)) {
			hr = library->GetBlobAsUtf16((IDxcBlob*)blob, &encode);
		}
	}

	assert(hr == S_OK);

	return blob;
}

Dx12::ShaderCompiler::ShaderCompiler()
{
}

Dx12::ShaderCompiler::ShaderCompiler(const std::string& file_path, const std::string& entry_name, const std::string& shader_model)
{
	obj = CompileFromFile(file_path, entry_name, shader_model);
}

Dx12::ShaderCompiler::ShaderCompiler(ID3DBlob* blob)
{
	obj = blob;
}

Dx12::ShaderCompiler::~ShaderCompiler()
{
}

void Dx12::ShaderCompiler::AddSubObject(SubObject* sub, const std::vector<std::string>& func_name)
{
	export_name.clear();

	for (auto& i : func_name) {
		auto name = Str::String(i).GetUniCode();
		export_name.push_back({ name.c_str(), nullptr, D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE });
	}

	dxil_info.DXILLibrary.BytecodeLength  = obj->GetBufferSize();
	dxil_info.DXILLibrary.pShaderBytecode = obj->GetBufferPointer();
	dxil_info.NumExports                  = std::uint32_t(export_name.size());
	dxil_info.pExports                    = export_name.data();

	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY, &dxil_info);
}
