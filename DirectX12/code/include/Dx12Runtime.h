#pragma once
#include "Vector.h"
#include <vector>

struct ID3D12Device6;
struct ID3D12DescriptorHeap;
struct ID3D12Resource1;
class Window;
class Dx12Device;
class Dx12CommandList;
class Dx12CommandQueue;
class Dx12SwapChain;
class Dx12Heap;
class Dx12Resource;
class Dx12RenderTarget;

class Dx12Runtime
{
public:
	/* �f�X�g���N�^ */
	~Dx12Runtime();
	/* ������ */
	static void Initialized(const st::Vec2& window_size);
	/* �I������ */
	static void UnInitialized(void);
	/* �E�B���h�E���b�Z�[�W�̊m�F */
	static bool CheckMsg(void);
	/* RTV�̐��� */
	static void RTV(const Dx12Heap* heap, const Dx12Resource* rsc, const std::uint32_t& index = 0);
	/* RTV�̐��� */
	static void RTV(ID3D12DescriptorHeap* heap, ID3D12Resource1* rsc, const std::uint32_t& index = 0);
	/* �����_�[�^�[�Q�b�g�̃N���A */
	static void ClearRenderTarget(const Dx12CommandList* list, const float color[4]);
	/* �R�}���h�̎��s */
	static void ExecuteCommand(const std::vector<Dx12CommandList*>& list);

public:
	/* �E�B���h�E�T�C�Y�̎擾 */
	static st::Vec2 GetWindowSize(void);
	/* �f�o�C�X�̎擾 */
	static ID3D12Device6* GetDevice(void);
	/* �����_�[�^�[�Q�b�g�̃��\�[�X�̎擾 */
	static ID3D12Resource1* GetRenderTarget(void);

private:
	/* �R���X�g���N�^ */
	Dx12Runtime();


private:
	/* �E�B���h�E */
	static Window* window;
	/* �f�o�C�X */
	static Dx12Device* device;
	/* �R�}���h�L���[ */
	static Dx12CommandQueue* queue;
	/* �X���b�v�`�F�C�� */
	static Dx12SwapChain* swap;
	/* �����_�[�^�[�Q�b�g */
	static Dx12RenderTarget* render;
};
