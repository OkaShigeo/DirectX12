#include "..\include/Runtime.h"
#include <wrl.h>

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

ID3D12DescriptorHeap* Dx12::Device::CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag) const
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

ID3D12Resource2* Dx12::Device::CreateBufferResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const std::uint64_t& size, const D3D12_RESOURCE_FLAGS& flag, const D3D12_CLEAR_VALUE* clear) const
{
	D3D12_RESOURCE_DESC1 desc{};
	desc.Dimension                = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.DepthOrArraySize         = 1;
	desc.Flags                    = flag;
	desc.Format                   = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height                   = 1;
	desc.Layout                   = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels                = 1;
	desc.SampleDesc               = { 1, 0 };
	desc.SamplerFeedbackMipRegion = {};
	desc.Width                    = size;

	ID3D12Resource2* rsc = nullptr;
	auto hr = obj->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, (D3D12_RESOURCE_DESC*)&desc, state, clear, nullptr, IID_PPV_ARGS(&rsc));
	assert(hr == S_OK);

	return rsc;
}

ID3D12Resource2* Dx12::Device::CreateTextureResource(const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_PROPERTIES& prop, const DXGI_FORMAT& format, const Math::Vec2& size, const D3D12_RESOURCE_FLAGS& flag, const D3D12_CLEAR_VALUE* clear) const
{
	D3D12_RESOURCE_DESC1 desc{};
	desc.DepthOrArraySize         = 1;
	desc.Dimension                = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags                    = flag;
	desc.Format                   = format;
	desc.Height                   = size.y;
	desc.Layout                   = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.MipLevels                = 1;
	desc.SampleDesc               = { 1, 0 };
	desc.SamplerFeedbackMipRegion = {};
	desc.Width                    = size.x;

	ID3D12Resource2* rsc = nullptr;
	auto hr = obj->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, (D3D12_RESOURCE_DESC*)&desc, state, clear, nullptr, IID_PPV_ARGS(&rsc));
	assert(hr == S_OK);

	return rsc;
}

ID3D12RootSignature* Dx12::Device::CreateRootSignature(const ShaderCompiler* shader) const
{
	ID3D12RootSignature* root = nullptr;
	auto hr = obj->CreateRootSignature(0, shader->Get()->GetBufferPointer(), shader->Get()->GetBufferSize(), IID_PPV_ARGS(&root));
	assert(hr == S_OK);

	return root;
}

ID3D12PipelineState* Dx12::Device::CreateGraphicsPipeline(const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const RootSignature* root, const ShaderCompiler* vertex, const ShaderCompiler* pixel, const ShaderCompiler* geometory, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology, const bool& depth) const
{
	D3D12_RASTERIZER_DESC rasterizer{};
	rasterizer.CullMode        = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
	rasterizer.DepthClipEnable = true;
	rasterizer.FillMode        = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;

	D3D12_RENDER_TARGET_BLEND_DESC renderBlend{};
	renderBlend.BlendEnable           = true;
	renderBlend.BlendOp               = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	renderBlend.BlendOpAlpha          = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	renderBlend.DestBlend             = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	renderBlend.DestBlendAlpha        = D3D12_BLEND::D3D12_BLEND_ZERO;
	renderBlend.LogicOp               = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	renderBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;
	renderBlend.SrcBlend              = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
	renderBlend.SrcBlendAlpha         = D3D12_BLEND::D3D12_BLEND_ONE;

	D3D12_BLEND_DESC blend{};
	for (int i = 0; i < 2; ++i)
	{
		blend.RenderTarget[i] = renderBlend;
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
	desc.BlendState                                     = blend;
	desc.DepthStencilState.DepthEnable                  = depth;
	desc.DepthStencilState.DepthWriteMask               = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc                    = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
	desc.DepthStencilState.StencilEnable                = depth;
	desc.DepthStencilState.StencilReadMask              = UCHAR_MAX;
	desc.DepthStencilState.StencilWriteMask             = UCHAR_MAX;
	desc.DepthStencilState.FrontFace.StencilFailOp      = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.FrontFace.StencilFunc        = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
	desc.DepthStencilState.FrontFace.StencilPassOp      = D3D12_STENCIL_OP::D3D12_STENCIL_OP_INCR;
	desc.DepthStencilState.BackFace.StencilFailOp       = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.BackFace.StencilDepthFailOp  = D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
	desc.DepthStencilState.BackFace.StencilPassOp       = D3D12_STENCIL_OP::D3D12_STENCIL_OP_DECR;
	desc.DepthStencilState.BackFace.StencilFunc         = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
	desc.DSVFormat                                      = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	if (geometory != nullptr)
	{
		desc.GS.pShaderBytecode = geometory->Get()->GetBufferPointer();
		desc.GS.BytecodeLength  = geometory->Get()->GetBufferSize();
	}
	desc.InputLayout           = { input.data(), std::uint32_t(input.size()) };
	desc.NumRenderTargets      = 1;
	desc.PrimitiveTopologyType = topology;
	desc.pRootSignature        = root->Get();
	desc.PS.pShaderBytecode    = pixel->Get()->GetBufferPointer();
	desc.PS.BytecodeLength     = pixel->Get()->GetBufferSize();
	desc.RasterizerState       = rasterizer;
	desc.RTVFormats[0]         = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleMask            = UINT_MAX;
	desc.SampleDesc            = { 1, 0 };
	desc.VS.pShaderBytecode    = vertex->Get()->GetBufferPointer();
	desc.VS.BytecodeLength     = vertex->Get()->GetBufferSize();

	ID3D12PipelineState* pipe = nullptr;
	auto hr = obj->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipe));
	assert(hr == S_OK);

	return pipe;
}

ID3D12PipelineState* Dx12::Device::CreateComputePipeline(const RootSignature* root, const ShaderCompiler* shader) const
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

ID3D12StateObject* Dx12::Device::CreateStateObject(const D3D12_STATE_OBJECT_TYPE& type, const SubObject* sub) const
{
	D3D12_STATE_OBJECT_DESC desc{};
	desc.NumSubobjects = std::uint32_t(sub->GetSubObjNum());
	desc.pSubobjects   = sub->GetSubObjects().data();
	desc.Type          = type;

	ID3D12StateObject* pipe = nullptr;
	auto hr = obj->CreateStateObject(&desc, IID_PPV_ARGS(&pipe));
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

void Dx12::Device::CreateShaderResourceView(const AccelerationStructure* acceleration) const
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.RaytracingAccelerationStructure.Location = acceleration->GetAddress();
	desc.Shader4ComponentMapping                  = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension                            = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	
	obj->CreateShaderResourceView(nullptr, &desc, acceleration->GetCpuHandle());
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

std::uint64_t Dx12::Device::GetHeapSize(const DescriptorHeap* heap) const
{
	return obj->GetDescriptorHandleIncrementSize(heap->GetType());
}

std::uint64_t Dx12::Device::CopySubResourceInfo(const Resource* resource, D3D12_PLACED_SUBRESOURCE_FOOTPRINT* information, const std::uint32_t& offset) const
{
	std::uint32_t num = 1;
	if (information != nullptr) {
		num = std::uint32_t(_msize(information) / sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT));
	}

	auto desc = resource->Get()->GetDesc();
	std::uint64_t size = 0;
	obj->GetCopyableFootprints(&desc, offset, num, 0, information, nullptr, nullptr, &size);

	return size;
}

D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO Dx12::Device::GetAccelerationStructurePrebuildInfo(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input) const
{
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	obj->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

	return info;
}
