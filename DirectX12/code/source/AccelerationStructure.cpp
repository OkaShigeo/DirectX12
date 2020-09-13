#include "..\include\AccelerationStructure.h"
#include "..\include\Runtime.h"

std::tuple<ID3D12Resource2*, ID3D12Resource2*> Dx12::AccelerationStructure::CreateBottomLevel(const std::vector<Resource*>& vertex, const std::vector<std::uint64_t>& vertex_num, const std::uint64_t& transform_matrix_addr, const std::vector<Resource*>& index, const std::vector<std::uint64_t>& index_num)
{
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> geo(vertex.size());
	for (std::uint32_t i = 0; i < geo.size(); ++i) {
		geo[i].Flags = D3D12_RAYTRACING_GEOMETRY_FLAGS::D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
		if (index.empty() == true && index.size() > i) {
			geo[i].Triangles.IndexBuffer = index[i]->GetAddress();
			geo[i].Triangles.IndexCount  = std::uint32_t(index_num[i]);
			geo[i].Triangles.IndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
		}
		geo[i].Triangles.Transform3x4               = transform_matrix_addr;
		geo[i].Triangles.VertexBuffer.StartAddress  = vertex[i]->GetAddress();
		geo[i].Triangles.VertexBuffer.StrideInBytes = sizeof(float) * 3;
		geo[i].Triangles.VertexCount                = std::uint32_t(vertex_num[i]);
		geo[i].Triangles.VertexFormat               = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		geo[i].Type                                 = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
	}

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	input.DescsLayout    = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input.Flags          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	input.NumDescs       = std::uint32_t(geo.size());
	input.pGeometryDescs = geo.data();
	input.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
	
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	Runtime::GetDevice()->Get()->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

	auto* scratch = Resource::CreateBufferResource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, Resource::GetDefaultProp(), info.ScratchDataSizeInBytes, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	auto* result  = Resource::CreateBufferResource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, Resource::GetDefaultProp(), info.ResultDataMaxSizeInBytes, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	return std::tie(scratch, result);
}

Dx12::AccelerationStructure::AccelerationStructure()
{
}

Dx12::AccelerationStructure::AccelerationStructure(const std::tuple<ID3D12Resource2*, ID3D12Resource2*>& rsc)
{
	(*this) = rsc;
}

Dx12::AccelerationStructure::~AccelerationStructure()
{
	Release();
}

void Dx12::AccelerationStructure::Release(void)
{
	if (instance != nullptr) {
		instance->Release();
		instance = nullptr;
	}
	if (result != nullptr) {
		result->Release();
		result = nullptr;
	}
	if (scratch != nullptr) {
		scratch->Release();
		scratch = nullptr;
	}
}
