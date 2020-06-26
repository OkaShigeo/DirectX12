#pragma once
#include "DxBaseObject.h"
#include "Vector.h"

class Dx12CommandAllocator;

class Dx12CommandList :
	public DxBaseObject<ID3D12GraphicsCommandList4>
{
public:
	/* �R���X�g���N�^ */
	Dx12CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE(0));
	/* �f�X�g���N�^ */
	~Dx12CommandList();
	/* �R�}���h���X�g�̃��Z�b�g */
	std::uint32_t Reset(const Dx12CommandAllocator* allocator);
	/* �o���A�̃Z�b�g */
	void SetBarrier(ID3D12Resource* rsc,
		const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
	/* �͈͂̃Z�b�g */
	void SetRect(const st::Vec2& size);

private:
	/* �R�}���h���X�g�̐��� */
	std::uint32_t CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);
};
