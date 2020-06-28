#pragma once
#include "DxBaseObject.h"
#include "Vector.h"
#include <vector>

class Dx12CommandAllocator;
class Dx12Heap;
class Dx12Resource;
class Dx12RootSignature;
class Dx12Pipeline;

class Dx12CommandList :
	public DxBaseObject<ID3D12GraphicsCommandList4>
{
public:
	/* �R���X�g���N�^ */
	Dx12CommandList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE(0));
	/* �f�X�g���N�^ */
	~Dx12CommandList();

public:
	/* �R�}���h���X�g�̃��Z�b�g */
	std::uint32_t Reset(const Dx12CommandAllocator* allocator);
	/* �o���A�̃Z�b�g */
	void SetBarrier(ID3D12Resource* rsc,
		const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
	/* UAV�o���A�̃Z�b�g */
	void SetBarrier(ID3D12Resource* rsc);
	/* �`��͈͂̃Z�b�g */
	void SetRect(const st::Vec2& size);
	/* ���[�g�V�O�l�`���̃Z�b�g */
	void SetRootSignature(const Dx12RootSignature* root);
	/* �R���s���[�g�p�̃��[�g�V�O�l�`���̃Z�b�g */
	void SetComputeRootSignature(const Dx12RootSignature* root);
	/* �p�C�v���C���̃Z�b�g */
	void SetPipeline(const Dx12Pipeline* pipe);
	/* ���_�`�� */
	void DrawVertexInstance(const Dx12Resource* vertex, const std::uint64_t& vertex_num, 
		const D3D12_PRIMITIVE_TOPOLOGY& topology, const std::uint32_t& instance_num = 1);

private:
	/* �R�}���h���X�g�̐��� */
	std::uint32_t CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type);
};
