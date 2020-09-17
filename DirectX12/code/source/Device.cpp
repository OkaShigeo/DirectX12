#include "..\include\Device.h"
#include "..\include\ShaderCompiler.h"
#include "..\include\RootSignature.h"
#include "..\include\Resource.h"
#include <wrl.h>
#include <cstdint>

namespace {
	/* ‹@”\ƒŒƒxƒ‹ˆê—— */
	const D3D_FEATURE_LEVEL levels[]{
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
	};
}

ID3D12Device6* Dx12::Device::CreateDevice(void)
{
	Microsoft::WRL::ComPtr<IDXGIFactory7>factory;
	std::uint32_t debug_flag = 0;
#ifdef _DEBUG
	debug_flag = DXGI_CREATE_FACTORY_DEBUG;
#endif
	auto hr = CreateDXGIFactory2(debug_flag, IID_PPV_ARGS(&factory));
	assert(hr == S_OK);

	ID3D12Device6* device = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter1>adaptor = nullptr;
	DXGI_ADAPTER_DESC1 desc{};
	for (std::uint32_t i = 0U; factory->EnumAdapters1(i, (IDXGIAdapter1**)&adaptor) != DXGI_ERROR_NOT_FOUND; ++i) {
		hr = adaptor->GetDesc1(&desc);
		assert(hr == S_OK);

		for (const D3D_FEATURE_LEVEL& level : levels) {
			hr = D3D12CreateDevice(adaptor.Get(), level, IID_PPV_ARGS(&device));
			if (SUCCEEDED(hr)) {
				D3D12_FEATURE_DATA_D3D12_OPTIONS5 option{};
				if (SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_D3D12_OPTIONS5, &option, sizeof(option)))) {
					if (option.RaytracingTier != D3D12_RAYTRACING_TIER::D3D12_RAYTRACING_TIER_NOT_SUPPORTED) {
						return device;
					}
					else {
						device->Release();
					}
				}
			}
		}
	}

	hr = factory->EnumWarpAdapter(IID_PPV_ARGS(&adaptor));
	assert(hr == S_OK);

	for (const D3D_FEATURE_LEVEL& level : levels) {
		hr = D3D12CreateDevice(adaptor.Get(), level, IID_PPV_ARGS(&device));
		if (SUCCEEDED(hr)) {
			return device;
		}
	}

	return nullptr;
}

Dx12::Device::Device()
{
	obj = CreateDevice();
	assert(obj != nullptr);
}

Dx12::Device::Device(ID3D12Device6* device)
{
	obj = device;
}

Dx12::Device::~Device()
{
}

ID3D12CommandAllocator* Dx12::Device::CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type) const
{
	ID3D12CommandAllocator* allocator = nullptr;
	auto hr = obj->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator));
	assert(hr == S_OK);

	return allocator;
}

ID3D12GraphicsCommandList5* Dx12::Device::CreateCommandList(const D3D12_COMMAND_LIST_TYPE& type) const
{
	ID3D12GraphicsCommandList5* list = nullptr;
	auto hr = obj->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&list));
	assert(hr == S_OK);

	return list;
}

ID3D12CommandQueue* Dx12::Device::CreateCommandQueue(const D3D12_COMMAND_LIST_TYPE& type) const
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = type;

	ID3D12CommandQueue* queue = nullptr;
	auto hr = obj->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
	assert(hr == S_OK);

	return queue;
}

ID3D12Fence1* Dx12::Device::CreateFence(const std::uint64_t& count) const
{
	ID3D12Fence1* fence = nullptr;
	auto hr = obj->CreateFence(count, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(hr == S_OK);

	return fence;
}

ID3D12DescriptorHeap* Dx12::Device::CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = flag;
	desc.NodeMask       = 0;
	desc.NumDescriptors = std::uint32_t(rsc_num);
	desc.Type           = type;

	ID3D12DescriptorHeap* heap = nullptr;
	auto hr = obj->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	assert(hr == S_OK);

	return heap;
}

ID3D12PipelineState* Dx12::Device::CreateComputePipeline(const RootSignature* root, const ShaderCompiler* shader)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.BytecodeLength  = shader->Get()->GetBufferSize();
	desc.CS.pShaderBytecode = shader->Get()->GetBufferPointer();
	desc.Flags              = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask           = 0;
	desc.pRootSignature     = root->Get();

	ID3D12PipelineState* pipe = nullptr;
	auto hr = obj->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipe));
	assert(hr == S_OK);

	return pipe;
}

void Dx12::Device::CreateRenderTarget(const Resource* resource) const
{
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format        = resource->Get()->GetDesc().Format;
	desc.Texture2D     = {};
	desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;

	obj->CreateRenderTargetView(resource->Get(), &desc, resource->GetCpuHandle());
}

void Dx12::Device::CreateConstantBufferView(const Resource* resource) const
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = resource->Get()->GetGPUVirtualAddress();
	desc.SizeInBytes    = std::uint32_t(resource->Get()->GetDesc().Width);

	obj->CreateConstantBufferView(&desc, resource->GetCpuHandle());
}

void Dx12::Device::CreateShaderResourceView(const Resource* resource) const
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format                  = resource->Get()->GetDesc().Format;
	desc.ViewDimension           = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels     = resource->Get()->GetDesc().MipLevels;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	obj->CreateShaderResourceView(resource->Get(), &desc, resource->GetCpuHandle());
}

void Dx12::Device::CreateUnorderAccessView(Resource* resource, const std::uint64_t& element_num) const
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.Buffer.CounterOffsetInBytes = 0;
	desc.Buffer.FirstElement         = 0;
	desc.Buffer.Flags                = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
	desc.Buffer.NumElements          = std::uint32_t(element_num);
	desc.Buffer.StructureByteStride  = std::uint32_t(resource->Get()->GetDesc1().Width) / desc.Buffer.NumElements;
	desc.Format                      = resource->Get()->GetDesc().Format;
	desc.ViewDimension               = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;

	obj->CreateUnorderedAccessView(resource->Get(), nullptr, &desc, resource->GetCpuHandle());
}

void Dx12::Device::CreateUnorderAccessView(Resource* resource) const
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.Format        = resource->Get()->GetDesc().Format;
	desc.Texture2D     = {};
	desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;

	obj->CreateUnorderedAccessView(resource->Get(), nullptr, &desc, resource->GetCpuHandle());
}

D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO Dx12::Device::GetAccelerationStructurePrebuildInfo(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input) const
{
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	obj->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

	return info;
}
