#include "..\include\Runtime.h"

namespace
{
	/* シェーダレコードサイズ */
	const std::uint32_t recode_size = (((D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + 8) + (D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT - 1)) / D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT) * D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT;
}

ID3D12Resource2* Dx12::ShaderTable::CreateShaderTableResource(const std::uint64_t& recode_num)
{
	return Runtime::GetDevice()->CreateBufferResource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Resource::GetUploadProp(), recode_size * recode_num);
}

std::uint64_t Dx12::ShaderTable::GetShaderRecodeSize(void)
{
	return recode_size;
}

Dx12::ShaderTable::ShaderTable()
{
}

Dx12::ShaderTable::ShaderTable(const std::uint64_t& recode_num)
{
	obj = CreateShaderTableResource(recode_num);
}

Dx12::ShaderTable::ShaderTable(ID3D12Resource2* resource)
{
	obj = resource;
}

Dx12::ShaderTable::~ShaderTable()
{
}
