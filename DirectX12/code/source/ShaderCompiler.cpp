#include "..\include\ShaderCompiler.h"
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

ShaderCompiler::ShaderCompiler(const std::wstring & file_path, const std::wstring & entry_func, const std::wstring & shader_model)
{
}

ShaderCompiler::~ShaderCompiler()
{
}

std::uint32_t ShaderCompiler::Compile(const std::wstring & file_path, const std::wstring & entry_func, const std::wstring & shader_model)
{
	Microsoft::WRL::ComPtr<IDxcLibrary>library = nullptr;
	auto hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：ライブラリハンドルが取得できませんでした\n");
		return hr;
	}

	Microsoft::WRL::ComPtr<IDxcIncludeHandler>handle = nullptr;
	hr = library->CreateIncludeHandler(&handle);
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：インクルードハンドルが取得できませんでした\n");
		return hr;
	}

	Microsoft::WRL::ComPtr<IDxcBlobEncoding>encode = nullptr;
	hr = library->CreateBlobFromFile(file_path.c_str(), nullptr, &encode);
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：エンコーダーが取得できませんでした\n");
		return hr;
	}

	Microsoft::WRL::ComPtr<IDxcCompiler>compile = nullptr;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compile));
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：コンパイラーが取得できませんでした\n");
		return hr;
	}

	Microsoft::WRL::ComPtr<IDxcOperationResult>result = nullptr;
	hr = compile->Compile(encode.Get(), file_path.c_str(), entry_func.c_str(), shader_model.c_str(), nullptr, 0, nullptr, 0, handle.Get(), &result);
	if (FAILED(hr)) {
		OutputDebugStringA("失敗：シェーダのコンパイルができませんでした\n");
		return hr;
	}

	result->GetStatus(&hr);
	if (SUCCEEDED(hr))
	{
		hr = result->GetResult((IDxcBlob**)&obj);
		if (FAILED(hr)) {
			OutputDebugStringA("失敗：シェーダのコンパイル結果を取得できませんでした\n");
			return hr;
		}
	}
	else
	{
		Microsoft::WRL::ComPtr<IDxcBlobEncoding>print = nullptr;
		Microsoft::WRL::ComPtr<IDxcBlobEncoding>print16 = nullptr;

		hr = result->GetErrorBuffer(&print);
		_ASSERT(hr == S_OK);

		hr = library->GetBlobAsUtf16(print.Get(), &print16);
		_ASSERT(hr == S_OK);

		wprintf(L"%*s", (int)print16->GetBufferSize() / 2, (LPCWSTR)print16->GetBufferPointer());
	}

	return hr;
}
