#include "..\include\ShaderCompiler.h"
#include <wrl.h>
#include <dxcapi.h>
#include <cassert>

#pragma comment(lib, "dxcompiler.lib")

Dx12::ShaderCompiler::ShaderCompiler(const std::wstring & file_path, const std::wstring & function, const std::wstring & shader_model)
{
	obj = Compile(file_path, function, shader_model);
}

Dx12::ShaderCompiler::ShaderCompiler(ID3DBlob * blob)
{
	obj = blob;
}

Dx12::ShaderCompiler::~ShaderCompiler()
{
}

ID3DBlob* Dx12::ShaderCompiler::Compile(const std::wstring & file_path, const std::wstring & function, const std::wstring & shader_model)
{
	Microsoft::WRL::ComPtr<IDxcLibrary>library = nullptr;
	auto hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcIncludeHandler>handle = nullptr;
	hr = library->CreateIncludeHandler(&handle);
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcBlobEncoding>encode = nullptr;
	hr = library->CreateBlobFromFile(file_path.c_str(), nullptr, &encode);
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcCompiler>compile = nullptr;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compile));
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcOperationResult>result = nullptr;
	hr = compile->Compile(encode.Get(), file_path.c_str(), function.c_str(), shader_model.c_str(), nullptr, 0, nullptr, 0, handle.Get(), &result);
	assert(hr == S_OK);

	ID3DBlob* blob = nullptr;
	result->GetStatus(&hr);
	if (SUCCEEDED(hr))
	{
		hr = result->GetResult((IDxcBlob**)&blob);
	}

	assert(hr == S_OK);

	return blob;
}
