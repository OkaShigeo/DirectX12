#pragma once
#include <vector>
#include <memory>

struct ID3D12Resource1;
class Dx12SwapChain;
class Dx12Heap;

class Dx12RenderTarget
{
public:
	/* �R���X�g���N�^ */
	Dx12RenderTarget(const Dx12SwapChain* swap);
	/* �f�X�g���N�^ */
	~Dx12RenderTarget();

public:
	/* ���\�[�X�̎擾 */
	ID3D12Resource1* Get(void) const;
	/* CPU�A�h���X�̎擾 */
	std::uint64_t GetCpuAddress(void) const;
	
private:
	/* �����_�[�^�[�Q�b�g�̐��� */
	std::uint32_t CreateRenderTarget(void);

private:
	/* �X���b�v�`�F�C�� */
	const Dx12SwapChain* swap;
	/* �q�[�v */
	std::unique_ptr<Dx12Heap>heap;
	/* ���\�[�X */
	std::vector<ID3D12Resource1*>rsc;
};

