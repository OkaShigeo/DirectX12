#include "..\include\Runtime.h"
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

ID3DBlob* Dx12::ShaderCompiler::CompileFromFile(const Str::String& file_path, const Str::String& entry_name, const Str::String& shader_model)
{
	auto path  = file_path.GetUniCode();
	auto name  = entry_name.GetUniCode();
	auto model = shader_model.GetUniCode();

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

	std::wstring fmt = path.substr(path.find_last_of(L".") + 1, std::string::npos);
	if (fmt == L"hlsl") {
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

Dx12::ShaderCompiler::ShaderCompiler(const Str::String& file_path, const Str::String& entry_name, const Str::String& shader_model)
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

void Dx12::ShaderCompiler::SetConfig(const std::vector<Str::String>& func_name)
{
	export_name.clear();
	for (auto& i : func_name) {
		export_name.push_back({ i.GetUniCodePtr(), nullptr, D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE });
	}

	config.DXILLibrary.BytecodeLength  = obj->GetBufferSize();
	config.DXILLibrary.pShaderBytecode = obj->GetBufferPointer();
	config.NumExports                  = std::uint32_t(export_name.size());
	config.pExports                    = export_name.data();
}

void Dx12::ShaderCompiler::AddSubObject(SubObject* sub)
{
	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY, &config);
}

void Dx12::ShaderCompiler::AddSubObject(SubObject* sub, const std::vector<Str::String>& func_name)
{
	SetConfig(func_name);
	AddSubObject(sub);
}
