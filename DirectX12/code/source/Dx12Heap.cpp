#include "..\include\Dx12Heap.h"
#include "..\include\Dx12Runtime.h"

Dx12Heap::Dx12Heap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& view_num)
{
	CreateHeap(type, view_num);
}

Dx12Heap::~Dx12Heap()
{
}

std::uint32_t Dx12Heap::CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& view_num)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	if (type == D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV) {
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	}
	else {
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	}
	desc.NodeMask       = 0;
	desc.NumDescriptors = std::uint32_t(view_num);
	desc.Type           = type;

	auto hr = Dx12Runtime::GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&obj));
	if (FAILED(hr)) {
		OutputDebugStringA("¸”sFƒq[ƒv‚Ì¶¬‚ª‚Å‚«‚Ü‚¹‚ñ‚Å‚µ‚½\n");
	}

	return hr;
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Heap::GetCpuAddress(const std::uint32_t& index) const
{
	auto handle = obj->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += Dx12Runtime::GetDevice()->GetDescriptorHandleIncrementSize(obj->GetDesc().Type) * index;
	
	return handle;
}
