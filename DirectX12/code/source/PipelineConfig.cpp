#include "..\include\Runtime.h"

Dx12::PipelineConfig::PipelineConfig()
{
}

Dx12::PipelineConfig::PipelineConfig(const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag)
{
	SetConfig(max_depth, flag);
}

Dx12::PipelineConfig::PipelineConfig(SubObject* sub, const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag)
{
	AddSubObject(sub, max_depth, flag);
}

Dx12::PipelineConfig::~PipelineConfig()
{
}

void Dx12::PipelineConfig::SetConfig(const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag)
{
	config.Flags                  = flag;
	config.MaxTraceRecursionDepth = max_depth;
}

void Dx12::PipelineConfig::AddSubObject(SubObject* sub)
{
	sub->AddSubObject(D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG1, &config);
}

void Dx12::PipelineConfig::AddSubObject(SubObject* sub, const std::uint32_t& max_depth, const D3D12_RAYTRACING_PIPELINE_FLAGS& flag)
{
	SetConfig(max_depth, flag);
	AddSubObject(sub);
}
