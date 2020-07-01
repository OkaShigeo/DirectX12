#include "include/Dx12Runtime.h"
#include "include/Dx12CommandAllocator.h"
#include "include/Dx12CommandList.h"
#include "include/ShaderCompiler.h"
#include "include/Dx12RootSignature.h"
#include "include/Dx12Pipeline.h"
#include "include/Dx12Heap.h"
#include "include/Dx12Resource.h"

namespace {
	/* ウィンドウサイズ */
	const st::Vec2 window_size(640, 480);
	/* クリアカラー */
	const float clear_color[] = {
		1.0f, 0.0f, 0.0f, 1.0f
	};
	/* 頂点情報 */
	struct VertexInfo {
		/* 座標 */
		st::Vec3f pos;
		/* UV */
		st::Vec2f uv;
	};
	const std::vector<VertexInfo>vertex = {
		/* 左から右 */
		/* 上から下 */
		{ st::Vec3f(-0.5f, -0.5f, 0.0f), st::Vec2f(0.0f, 0.0f) },
		{ st::Vec3f( 0.5f, -0.5f, 0.0f), st::Vec2f(1.0f, 0.0f) },
		{ st::Vec3f(-0.5f,  0.5f, 0.0f), st::Vec2f(0.0f, 1.0f) },
		{ st::Vec3f( 0.5f,  0.5f, 0.0f), st::Vec2f(1.0f, 1.0f) },
	};
	/* 入力属性 */
	const std::vector<D3D12_INPUT_ELEMENT_DESC>input = {
		{ "POSITION", 0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		/*{ "NORMAL", 0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BONENO", 0,
			DXGI_FORMAT::DXGI_FORMAT_R16G16_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "WEIGHT", 0,
			DXGI_FORMAT::DXGI_FORMAT_R8_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },*/
	};
	/* コンピュート */
	struct Compute {
		/* ヒープ */
		Dx12Heap heap;
		/* リソース */
		std::vector<Dx12Resource>rsc;
	};
	/* テクスチャリソース */
	struct Texture {
		/* ヒープ */
		Dx12Heap heap;
		/* リソース */
		Dx12Resource rsc;
	};
	/* テクスチャサイズ */
	const st::Vec2 texture_size = window_size;
	/* レイトレーシングパラメータ */
	struct RaytracingParam {
		/* 視線位置 */
		st::Vec3f eye_pos;
		/* レイの最大距離 */
		float distance;
	};
}

int main()
{
	void* buffer = nullptr;

	Dx12Runtime::Initialized(window_size);
	auto* allocator = new Dx12CommandAllocator();
	auto* list = new Dx12CommandList();
	auto* plane = new Dx12Resource();
	{
		plane->CreateResource(Dx12Resource::GetUploadProp(), D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, sizeof(vertex[0]) * vertex.size(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
		plane->Map(&buffer, plane->Get()->GetDesc().Width);
		std::memcpy(buffer, vertex.data(), plane->Get()->GetDesc().Width);
		plane->Unmap();
	}
	auto* vs_shader = new ShaderCompiler(L"../code/shader/VertexShader.hlsl", L"main", L"vs_6_4");
	auto* ps_shader = new ShaderCompiler(L"../code/shader/PixelShader.hlsl", L"main", L"ps_6_4");
	auto* root = new Dx12RootSignature();
	{
		std::vector<D3D12_DESCRIPTOR_RANGE1>range(1);
		range[0].BaseShaderRegister = 0;
		range[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAGS::D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
		range[0].NumDescriptors = 1;
		range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[0].RegisterSpace = 0;
		std::vector<D3D12_ROOT_PARAMETER1>param(1);
		param[0].DescriptorTable.NumDescriptorRanges = std::uint32_t(range.size());
		param[0].DescriptorTable.pDescriptorRanges = range.data();
		param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;

		D3D12_STATIC_SAMPLER_DESC sampler{};
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
		sampler.Filter = D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		sampler.MaxAnisotropy = 0;
		sampler.MaxLOD = float(UINT_MAX);
		sampler.MinLOD = 0.0f;
		sampler.MipLODBias = 0.0f;
		sampler.RegisterSpace = 0;
		sampler.ShaderRegister = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
		root->CreateRootSignature(D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, param, { sampler });
	}
	auto* pipe = new Dx12Pipeline();
	{
		pipe->CreatePipeline(input, D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, root, vs_shader, ps_shader);
	}
	auto* cs_shader = new ShaderCompiler(L"../code/shader/ComputeShader.hlsl", L"main", L"cs_6_4");
	auto* cs_root = new Dx12RootSignature();
	{
		cs_root->CreateRootSignature(cs_shader->Get());
	}
	auto* cs_pipe = new Dx12Pipeline();
	{
		cs_pipe->CreatePipeline(cs_shader, cs_root);
	}
	Compute* compute = new Compute();
	{
		compute->heap.CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2);
		compute->rsc.resize(compute->heap->GetDesc().NumDescriptors);
		std::uint32_t index = 0;
		compute->rsc[index].CreateResource(Dx12Resource::GetDefaultProp(), D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
			texture_size.x, texture_size.y, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
		Dx12Runtime::UAV(compute->heap.Get(), compute->rsc[index].Get(), index);
		++index;
		compute->rsc[index].CreateResource(Dx12Resource::GetUploadProp(), D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE,
			(sizeof(RaytracingParam) + 0xff) & ~0xff, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
		Dx12Runtime::CBV(compute->heap.Get(), compute->rsc[index].Get(), index);
		RaytracingParam param{};
		param.eye_pos.z = 1.0f;
		param.distance  = 1.0f;
		compute->rsc[index].Map(&buffer, sizeof(RaytracingParam));
		std::memcpy(buffer, &param, sizeof(param));
		compute->rsc[index].Unmap();
	}
	Texture* texture = new Texture();
	{
		texture->heap.CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
		texture->rsc.CreateResource(Dx12Resource::GetDefaultProp(), D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE,
			texture_size.x, texture_size.y, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
		Dx12Runtime::SRV(texture->heap.Get(), texture->rsc.Get());
	}

	while (Dx12Runtime::CheckMsg()) {
		allocator->Reset();
		list->Reset(allocator);

		list->SetComputeRootSignature(cs_root);
		list->SetPipeline(cs_pipe);
		list->SetBarrier(compute->rsc[0].Get(),
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		list->Get()->SetDescriptorHeaps(1, (ID3D12DescriptorHeap* const*)&compute->heap);
		for (std::uint32_t i = 0; i < compute->rsc.size(); ++i) {
			list->Get()->SetComputeRootDescriptorTable(i, compute->heap.GetGpuAddress(i));
		}
		list->Get()->Dispatch(texture_size.x, texture_size.y, 1);
		list->SetBarrier(compute->rsc[0].Get());
		list->SetBarrier(compute->rsc[0].Get(),
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
		list->Get()->CopyResource(texture->rsc.Get(), compute->rsc[0].Get());

		list->SetRect(Dx12Runtime::GetWindowSize());

		list->SetBarrier(Dx12Runtime::GetRenderTarget(),
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
		Dx12Runtime::ClearRenderTarget(list, clear_color);

		list->SetRootSignature(root);
		list->SetPipeline(pipe);
		list->SetBarrier(texture->rsc.Get(),
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
		list->Get()->SetDescriptorHeaps(1, (ID3D12DescriptorHeap* const*)&texture->heap);
		list->Get()->SetGraphicsRootDescriptorTable(0, texture->heap.GetGpuAddress());
		list->DrawVertexInstance(plane, vertex.size(), D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		list->SetBarrier(texture->rsc.Get(),
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);

		list->SetBarrier(Dx12Runtime::GetRenderTarget(),
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

		Dx12Runtime::ExecuteCommand({ list });
	}

	delete texture;
	delete compute;
	delete cs_pipe;
	delete cs_root;
	delete cs_shader;
	delete pipe;
	delete root;
	delete ps_shader;
	delete vs_shader;
	delete plane;
	delete list;
	delete allocator;
	Dx12Runtime::UnInitialized();

	return 0;
}
