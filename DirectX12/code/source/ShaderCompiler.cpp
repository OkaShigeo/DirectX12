#include "..\include\ShaderCompiler.h"
#include <wrl.h>
#include <dxcapi.h>
#include <cassert>

#pragma comment(lib, "dxcompiler.lib")

ID3DBlob* Dx12::ShaderCompiler::CompileFromFile(const std::wstring & file_path, const std::wstring & function, const std::wstring & shader_model)
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

	ID3DBlob* blob = nullptr;

	std::wstring fmt = file_path.substr(file_path.find_last_of(L".") + 1, std::wstring::npos);
	if (fmt == L"hlsl") {
		/* コンパイル */
		Microsoft::WRL::ComPtr<IDxcOperationResult>result = nullptr;
		hr = compile->Compile(encode.Get(), file_path.c_str(), function.c_str(), shader_model.c_str(), nullptr, 0, nullptr, 0, handle.Get(), &result);
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

Dx12::ShaderCompiler::ShaderCompiler(const std::wstring& file_path, const std::wstring& function, const std::wstring& shader_model)
{
	obj = CompileFromFile(file_path, function, shader_model);
}

Dx12::ShaderCompiler::ShaderCompiler(ID3DBlob* blob)
{
	Release();
	obj = blob;
}

Dx12::ShaderCompiler::~ShaderCompiler()
{
}

void Dx12::ShaderCompiler::AddSubObj(const std::vector<std::wstring>& func_name)
{
	std::vector<D3D12_EXPORT_DESC>expo;
	for (auto& i : func_name) {
		expo.push_back({ i.c_str(), nullptr, D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE });
	}

	D3D12_DXIL_LIBRARY_DESC desc{};
	desc.DXILLibrary.BytecodeLength  = obj->GetBufferSize();
	desc.DXILLibrary.pShaderBytecode = obj->GetBufferPointer();
	desc.NumExports                  = std::uint32_t(expo.size());
	desc.pExports                    = expo.data();

	sub.push_back({ D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY, &desc });
}
