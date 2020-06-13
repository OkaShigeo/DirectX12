#pragma once
#include "DxBaseObject.h"
#include <vector>

class Dx12CommandList;
class Dx12Fence;

class Dx12CommandQueue :
	public DxBaseObject<ID3D12CommandQueue>
{
public:
	/* �R���X�g���N�^ */
	Dx12CommandQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE(0));
	/* �f�X�g���N�^ */
	~Dx12CommandQueue();
	/* �R�}���h�̎��s */
	void ExecuteCommand(const std::vector<Dx12CommandList*>& lists);

private:
	/* �R�}���h�L���[�̐��� */
	std::uint32_t CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type);


private:
	/* �t�F���X */
	Dx12Fence* fence;
};
