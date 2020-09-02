#include "include/Runtime.h"
#include <random>

namespace
{
	/* ウィンドウサイズ */
	const Dx12::Vec2 window_size(640, 480);

	/* 頂点情報 */
	struct VertexInfo
	{
		/* 座標 */
		Dx12::Vec3f pos{ 0.0f };
		/* テクスチャ座標 */
		Dx12::Vec2f uv{ 0.0f };
	};
	VertexInfo vertex_info[] = 
	{
		/* 左から右 */
		/* 上から下 */
		{ Dx12::Vec3f(-1.0f, -1.0f, 0.0f), Dx12::Vec2f(0.0f, 0.0f) },
		{ Dx12::Vec3f( 1.0f, -1.0f, 0.0f), Dx12::Vec2f(1.0f, 0.0f) },
		{ Dx12::Vec3f(-1.0f,  1.0f, 0.0f), Dx12::Vec2f(0.0f, 1.0f) },
		{ Dx12::Vec3f( 1.0f,  1.0f, 0.0f), Dx12::Vec2f(1.0f, 1.0f) },
	};
	/* インデックス情報 */
	std::uint16_t index_info[] = 
	{
		0, 1, 2,
		1, 2, 3,
	};

	/* シェーダーファイルディレクトリ */
	const std::wstring shader_dir = L"../code/hlsl/";
	/* シェーダエントリーポイント */
	const std::wstring shader_func = L"main";
	/* シェーダーモデル */
	const std::wstring shader_model = L"_6_3";

	/* 入力属性 */
	const std::vector<D3D12_INPUT_ELEMENT_DESC>input = {
		{ "POSITION", 0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	/* カメラ情報 */
	struct Camera
	{
		/* 座標 */
		Dx12::Vec3f pos{ 0.0f };
		/* 視野角 */
		float fov{ 90.0f };
		/* 焦点座標 */
		Dx12::Vec3f target{ 0.0f };
		/* レンズの大きさ */
		float lens_aperture{ 0.0f };
		/* アップベクトル */
		Dx12::Vec3f up{ Dx12::Vec3f(0.0f, 1.0f, 0.0f) };
		/* アライメント */
		float alignment{ 0.0f };
	};
	/* マテリアル種別 */
	enum class Material : std::uint32_t
	{
		/* マテリアル無し */
		None,
		/* ランバート反射 */
		Lambert,
		/* 鏡面反射 */
		reflect,
		/* 屈折 */
		Refract,
	};
	/* 球体情報 */
	struct Sphere
	{
		/* 中心座標 */
		Dx12::Vec3f center{ 0.0f };
		/* 半径 */
		float radius{ 0.0f };
		/* 色 */
		Dx12::Vec3f color{ 1.0f };
		/* マテリアルタイプ */
		Material material{ Material::None };
		/* 移動量 */
		Dx12::Vec3f velocity{ 0.0f };
		/* アライメント */
		float alignment{ 0.0f };
	};
	/* 球体の最大数 */
	const std::uint32_t sphere_max = 2;
}

int main()
{
	Dx12::Runtime::Initialize(window_size);

	/*==========テクスチャ関連==========*/
	Dx12::Resource* texture_vertex              = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(vertex_info));
	Dx12::Resource* texture_index               = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(index_info));
	Dx12::Resource* texture                     = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetDefaultProp(), DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, window_size.x, window_size.y, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	Dx12::DescriptorHeap* texture_heap          = new Dx12::DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
	Dx12::ShaderCompiler* texture_shader_vertex = new Dx12::ShaderCompiler(shader_dir + L"Texture/Vertex.hlsl", shader_func, L"vs" + shader_model);
	Dx12::ShaderCompiler* texture_shader_pixel  = new Dx12::ShaderCompiler(shader_dir + L"Texture/Pixel.hlsl", shader_func, L"ps" + shader_model);
	Dx12::RootSignature* texture_root           = new Dx12::RootSignature(texture_shader_vertex);
	Dx12::GraphicsPipeline* texture_pipe        = new Dx12::GraphicsPipeline(input, texture_root, texture_shader_vertex, texture_shader_pixel);
	/* GPUへ頂点情報を送信 */
	{
		auto* buffer = texture_vertex->GetBuffer();
		std::memcpy(buffer, &vertex_info[0], sizeof(vertex_info));
		texture_vertex->ReleaseBuffer();
	}
	/* GPUへインデックス情報を送信 */
	{
		auto* buffer = texture_index->GetBuffer();
		std::memcpy(buffer, &index_info[0], sizeof(index_info));
		texture_index->ReleaseBuffer();
	}
	/* ビューの生成 */
	{
		texture_heap->CreateShaderResourceView(texture);
	}

	/*==========レイトレーシング関連==========*/
	Dx12::Resource* camera                  = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), (sizeof(Camera) + 0xff) & ~0xff);
	Dx12::Resource* sphere                  = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, Dx12::Resource::GetDefaultProp(), (sizeof(Sphere) * sphere_max), D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	Dx12::Resource* result                  = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, Dx12::Resource::GetDefaultProp(), DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, window_size.x, window_size.y, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	Dx12::DescriptorHeap* raytracing_heap   = new Dx12::DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3);
	Dx12::ShaderCompiler* sphere_shader     = new Dx12::ShaderCompiler(shader_dir + L"Raytracing/Sphere.hlsl", shader_func, L"cs" + shader_model);
	Dx12::RootSignature* sphere_root        = new Dx12::RootSignature(sphere_shader);
	Dx12::ComputePipeline* sphere_pipe      = new Dx12::ComputePipeline(sphere_root, sphere_shader);
	Dx12::ShaderCompiler* raytracing_shader = new Dx12::ShaderCompiler(shader_dir + L"Raytracing/Raytracing.hlsl", shader_func, L"cs" + shader_model);
	Dx12::RootSignature* raytracing_root    = new Dx12::RootSignature(raytracing_shader);
	Dx12::ComputePipeline* raytracing_pipe  = new Dx12::ComputePipeline(raytracing_root, raytracing_shader);

	/* ビューの生成 */
	{
		raytracing_heap->CreateConstantBufferView(camera);
		raytracing_heap->CreateUnorderAccessView(sphere, sphere_max);
		raytracing_heap->CreateUnorderAccessView(result);
	}
	/* 球体の生成 */
	{
		Dx12::Runtime::Clear();

		Dx12::Runtime::SetComputeRootSignature(sphere_root);
		Dx12::Runtime::SetComputePipeline(sphere_pipe);
		Dx12::Runtime::SetDescriptorHeap({ raytracing_heap });
		Dx12::Runtime::SetComputeResource(sphere, 0);

		Dx12::Runtime::Dispatch(sphere_max);
		Dx12::Runtime::SetUavRscBarrier(sphere);
		
		Dx12::Runtime::Execution();
	}
	/* GPUへカメラ情報を送信 */
	{
		Camera camera_info{};
		camera_info.pos           = Dx12::Vec3f(13.0f, 2.0f, 3.0f);
		camera_info.fov           = 20.0f;
		camera_info.target        = Dx12::Vec3f(0.0f, 0.0f, 0.0f);
		camera_info.lens_aperture = 0.0f;

		auto* buffer = camera->GetBuffer();
		std::memcpy(buffer, &camera_info, sizeof(Camera));
		camera->ReleaseBuffer();
	}
	/* レイトレーシングの実行 */
	{
		Dx12::Runtime::Clear();

		std::uint32_t param_index = 0;

		Dx12::Runtime::SetComputeRootSignature(raytracing_root);
		Dx12::Runtime::SetComputePipeline(raytracing_pipe);
		Dx12::Runtime::SetDescriptorHeap({ raytracing_heap });
		Dx12::Runtime::SetComputeResource(camera, param_index++);
		Dx12::Runtime::SetComputeResource(sphere, param_index++);
		Dx12::Runtime::SetComputeResource(result, param_index++);

		Dx12::Runtime::SetRscBarrier(result, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		Dx12::Runtime::Dispatch(window_size.x, window_size.y);
		Dx12::Runtime::SetUavRscBarrier(result);
		Dx12::Runtime::SetRscBarrier(result, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
		
		Dx12::Runtime::Execution();
	}

	/*==========メインループ==========*/
	while (Window::CheckMsg() == true) {
		Dx12::Runtime::Clear();

		/* レイトレーシング結果をレンダーターゲットにコピー*/
		{
			Dx12::Runtime::SetRscBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
			Dx12::Runtime::CopyResource(texture, result);
			Dx12::Runtime::SetRscBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
		}
		/* テクスチャ描画 */
		{
			std::uint32_t param_index = 0;

			Dx12::Runtime::SetGraphicsRootSignature(texture_root);
			Dx12::Runtime::SetGraphicsPipeline(texture_pipe);
			Dx12::Runtime::SetDescriptorHeap({ texture_heap });
			Dx12::Runtime::SetGraphicsResource(texture, param_index++);
			Dx12::Runtime::DrawIndexInstance(texture_vertex, _countof(vertex_info), texture_index, _countof(index_info));
		}

		Dx12::Runtime::Execution();
	}

	/*==========メモリ解放==========*/
	delete raytracing_pipe;
	delete raytracing_root;
	delete raytracing_shader;
	delete raytracing_heap;
	delete result;
	delete camera;

	delete sphere_pipe;
	delete sphere_root;
	delete sphere_shader;
	delete sphere;

	delete texture_pipe;
	delete texture_root;
	delete texture_shader_pixel;
	delete texture_shader_vertex;
	delete texture_heap;
	delete texture;
	delete texture_index;
	delete texture_vertex;
	Dx12::Runtime::UnInitialize();
	return 0;
}