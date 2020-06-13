#include "..\include\Dx12CommandAllocator.h"
#include "..\include\Dx12Runtime.h"
#include <d3d12.h>

Dx12CommandAllocator::Dx12CommandAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	CreateCommandAllocator(type);
}

Dx12CommandAllocator::~Dx12CommandAllocator()
{
}

std::uint32_t Dx12CommandAllocator::CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	if (obj != nullptr) {
		OutputDebugStringA("�R�}���h�A���P�[�^�������ς݂ł�\n");
		return S_FALSE;
	}

	auto hr = Dx12Runtime::GetDevice()->CreateCommandAllocator(type, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("���s�F�R�}���h�A���P�[�^�������ł��܂���ł���\n");
	}

	return hr;
}

std::uint32_t Dx12CommandAllocator::Reset(void) const
{
	if (obj == nullptr) {
		OutputDebugStringA("�R�}���h�A���P�[�^����������Ă��܂���\n");
		return S_FALSE;
	}

	auto hr = obj->Reset();
	if (FAILED(hr)) {
		OutputDebugStringA("���s�F�R�}���h�A���P�[�^�̃��Z�b�g���ł��܂���ł���\n");
	}

	return hr;
}
