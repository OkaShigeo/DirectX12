#include "include/Runtime.h"

namespace
{
	/* ウィンドウサイズ */
	const Math::Vec2 win_size = { 640, 480 };

	/* シェーダーファイルディレクトリ */
	const std::string shader_dir = "../code/hlsl/";
	/* シェーダエントリーポイント */
	const std::string shader_func = "main";
	/* シェーダーモデル */
	const std::string shader_model = "_6_4";

	/* ペイロード */
	struct Payload
	{
		/* 出力カラー */
		float color[4];
	};

	/* レイトレーシングの情報 */
	struct RaytracingInfo
	{
		/* 視点 */
		Math::Vec3f eye_pos{ 0.0f, 0.0f, -2.0f };
		/* レイの方向 */
		Math::Vec3f direction{ 0.0f, 0.0f, 1.0f };
		/* ヒット判定開始距離 */
		float tmin{ 0.01f };
		/* ヒット判定終了処理 */
		float tmax{ 10000.0f };
	};

	/* 頂点リソース */
	Dx12::VertexBuffer* vertex = nullptr;
	
	/* インデックスリソース */
	Dx12::IndexBuffer* index = nullptr;

	/* ボトムレベル加速構造 */
	Dx12::AccelerationStructure* bottom = nullptr;
	/* トップレベル加速構造 */
	Dx12::AccelerationStructure* top = nullptr;

	/* レイ生成シェーダ関数名 */
	Str::String ray_gen_name = "RayGen";
	/* レイ生成シェーダ */
	Dx12::ShaderCompiler* ray_gen = nullptr;
	/* レイ生成コレクション */
	Dx12::RaytracingPipeline* ray_gen_collection = nullptr;
	/* レイ生成シェーダの定数リソース */
	Dx12::Resource* ray_gen_constant = nullptr;
	/* レイ生成シェーダのシェーダテーブル */
	Dx12::ShaderTable* ray_gen_table = nullptr;

	/* 最も近いヒットシェーダ関数名 */
	Str::String closesthit_name = "ClosestHit";
	/* ヒットグループ名 */
	Str::String group_name = "Triangle";
	/* 最も近いヒットシェーダ */
	Dx12::ShaderCompiler* closest_hit = nullptr;
	/* 最も近いヒットコレクション */
	Dx12::RaytracingPipeline* closesthit_collection = nullptr;
	/* 最も近いヒットシェーダテーブル */
	Dx12::ShaderTable* closesthit_table = nullptr;

	/* ミスシェーダ名 */
	Str::String miss_name = "Miss";
	/* ミスシェーダ */
	Dx12::ShaderCompiler* miss = nullptr;
	/* ミスコレクション */
	Dx12::RaytracingPipeline* miss_collection = nullptr;
	/* ミスシェーダテーブル */
	Dx12::ShaderTable* miss_table = nullptr;

	/* グローバルルートシグネチャ */
	Dx12::RootSignature* global = nullptr;
	/* パイプライン */
	Dx12::RaytracingPipeline* pipe = nullptr;

	/* ヒープ */
	Dx12::DescriptorHeap* heap = nullptr;
	/* 出力データ */
	Dx12::Resource* output = nullptr;
}

/* 頂点の初期化 */
void InitializeVertex(void)
{
	std::vector<Math::Vec3f>vertex_info =
	{
		/* 左から右 */
		/* 上から下 */
		{ Math::Vec3f(-1.0f, -1.0f, 0.0f) },
		{ Math::Vec3f( 1.0f, -1.0f, 0.0f) },
		{ Math::Vec3f(-1.0f,  1.0f, 0.0f) },
		{ Math::Vec3f( 1.0f,  1.0f, 0.0f) },
	};

	vertex = new Dx12::VertexBuffer(vertex_info.data(), vertex_info.size());
}

/* インデックスの初期化 */
void InitializeIndex(void)
{
	/* インデックス情報 */
	std::vector<std::uint16_t>index_info =
	{
		0, 1, 2,
		1, 2, 3,
	};

	index = new Dx12::IndexBuffer(index_info.data(), index_info.size());
}

/* 加速構造の初期化 */
void InitializeAccelerationStructure(void)
{
	Dx12::Runtime::Clear();
	bottom = new Dx12::AccelerationStructure({ vertex }, { index });
	Dx12::Runtime::BuildAccelerationStructure(bottom);
	Dx12::Runtime::SetUavResourceBarrier(bottom);

	top    = new Dx12::AccelerationStructure(1);
	auto* buffer = top->GetBuffer();
	auto a = top->GetInstance()->GetDesc().Width;
	auto w = sizeof(D3D12_RAYTRACING_INSTANCE_DESC);
	buffer[0].AccelerationStructure = bottom->GetAddress();
	buffer[0].Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	buffer[0].InstanceContributionToHitGroupIndex = 0;
	buffer[0].InstanceID = 0;
	buffer[0].InstanceMask = UCHAR_MAX;
	float mat[3][4] = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
	};
	buffer[0].Transform[0][0] = 1.0f;
	buffer[0].Transform[1][1] = 1.0f;
	buffer[0].Transform[2][2] = 1.0f;
	top->ReleaseBuffer();
	Dx12::Runtime::BuildAccelerationStructure(top);
	Dx12::Runtime::SetUavResourceBarrier(top);
	Dx12::Runtime::Execution();
}

/* レイ生成のサブオブジェクトの初期化 */
Str::String InitializeRayGenSubObject(void)
{
	Dx12::SubObject sub(10);

	ray_gen = new Dx12::ShaderCompiler(shader_dir + "DirectXRaytracing/RayGeneration.hlsl", "", "lib" + shader_model);
	ray_gen->AddSubObject(&sub, { ray_gen_name });

	static Dx12::RootSignature local(D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE, {
		Dx12::RootSignature::GetParam(D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_CBV, 0) });
	local.AddSubObject(&sub, D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE, { ray_gen_name });

	Dx12::ObjectConfig obj_config(&sub, D3D12_STATE_OBJECT_FLAGS::D3D12_STATE_OBJECT_FLAG_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS);

	ray_gen_collection = new Dx12::RaytracingPipeline(D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_COLLECTION, &sub);

	return ray_gen_name;
}

/* 最も近いヒットのサブオブジェクトの初期化 */
Str::String InitializeClosestHitSubObject(void)
{
	Dx12::SubObject sub(10);

	closest_hit = new Dx12::ShaderCompiler(shader_dir + "DirectXRaytracing/ClosestHit.hlsl", "", "lib" + shader_model);
	closest_hit->AddSubObject(&sub, { closesthit_name });

	Dx12::RootSignature local(D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE, { });
	local.AddSubObject(&sub, D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE, { closesthit_name });

	Dx12::HitGroup hit(&sub, group_name, closesthit_name.GetMultibyteCodePtr());
	Dx12::ObjectConfig obj_config(&sub, D3D12_STATE_OBJECT_FLAGS::D3D12_STATE_OBJECT_FLAG_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS);

	closesthit_collection = new Dx12::RaytracingPipeline(D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_COLLECTION, &sub);

	return closesthit_name;
}

/* ミスのサブオブジェクトの初期化 */
Str::String InitializeMissSubObject(void)
{
	Dx12::SubObject sub(10);

	miss = new Dx12::ShaderCompiler(shader_dir + "DirectXRaytracing/Miss.hlsl", "", "lib" + shader_model);
	miss->AddSubObject(&sub, { miss_name });

	Dx12::RootSignature local(D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE, { });
	local.AddSubObject(&sub, D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE, { miss_name });

	Dx12::ObjectConfig obj_config(&sub, D3D12_STATE_OBJECT_FLAGS::D3D12_STATE_OBJECT_FLAG_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS);
	miss_collection = new Dx12::RaytracingPipeline(D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_COLLECTION, &sub);

	return miss_name;
}

/* サブオブジェクトの初期化 */
void InitializeSubObject(void)
{
	std::vector<Str::String>func_names =
	{
		InitializeRayGenSubObject(),
		InitializeClosestHitSubObject(),
		InitializeMissSubObject(),
	};

	Dx12::SubObject sub(10);

	global = new Dx12::RootSignature(D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, { 
		Dx12::RootSignature::GetParam(D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE , 0, 
			{ Dx12::RootSignature::GetRange(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0)}),
		Dx12::RootSignature::GetParam(D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 
			{Dx12::RootSignature::GetRange(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0)})
		});
	global->AddSubObject(&sub, D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE);

	ray_gen_collection->AddSubObject(&sub);
	closesthit_collection->AddSubObject(&sub);
	miss->AddSubObject(&sub);


	Dx12::ShaderConfig shader_config(&sub, sizeof(Payload), func_names);
	Dx12::PipelineConfig pipe_config(&sub, 1);
	Dx12::ObjectConfig obj_config(&sub, D3D12_STATE_OBJECT_FLAGS::D3D12_STATE_OBJECT_FLAG_ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS);

	pipe = new Dx12::RaytracingPipeline(D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE, &sub);
}

/* レイ生成シェーダテーブルの初期化 */
void InitializeRayGenShaderTable(void)
{
	ray_gen_constant = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(RaytracingInfo));
	RaytracingInfo info{};
	auto* buffer = ray_gen_constant->GetBuffer();
	std::memcpy(buffer, &info, sizeof(RaytracingInfo));
	ray_gen_constant->ReleaseBuffer();


	ray_gen_table = new Dx12::ShaderTable(1);
	buffer = ray_gen_table->GetBuffer();
	std::memcpy(buffer, pipe->GetShaderIdentifier(ray_gen_name), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	*(D3D12_GPU_VIRTUAL_ADDRESS*)(buffer + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES) = ray_gen_constant->GetAddress();
	ray_gen_table->ReleaseBuffer();
}

/* 最も近いヒットシェーダテーブルの初期化 */
void InitializeClosestHitShaderTable(void)
{
	closesthit_table = new Dx12::ShaderTable(1);
	auto* buffer = closesthit_table->GetBuffer();
	std::memcpy(buffer, pipe->GetShaderIdentifier(group_name), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	closesthit_table->ReleaseBuffer();
}

/* ミス関連 */
void InitializeMissShaderTable(void)
{
	miss_table = new Dx12::ShaderTable(1);
	auto* buffer = miss_table->GetBuffer();
	std::memcpy(buffer, pipe->GetShaderIdentifier(miss_name), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	miss_table->ReleaseBuffer();
}

/* シェーダテーブルの初期化 */
void InitializeShaderTable(void)
{
	InitializeRayGenShaderTable();
	InitializeClosestHitShaderTable();
	InitializeMissShaderTable();
}

/* 初期化 */
void Constructor(void)
{
	InitializeVertex();
	InitializeIndex();
	InitializeAccelerationStructure();
	InitializeSubObject();
	InitializeShaderTable();

	heap = new Dx12::DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2);
	heap->CreateShaderResourceView(top);
	output = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, Dx12::Resource::GetDefaultProp(), DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, Dx12::Runtime::GetViewportSize(), D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	heap->CreateUnorderAccessView(output);
}

/* 加速構造の更新 */
void UpdateAccelerationStructure(void)
{
	auto* buffer = top->GetBuffer();
	buffer[0].AccelerationStructure               = bottom->GetAddress();
	buffer[0].Flags                               = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	buffer[0].InstanceContributionToHitGroupIndex = 0;
	buffer[0].InstanceID                          = 0;
	buffer[0].InstanceMask                        = UCHAR_MAX;
	float mat[3][4] = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
	};
	std::memcpy(buffer[0].Transform, mat, sizeof(mat));
	top->ReleaseBuffer();

	Dx12::Runtime::BuildAccelerationStructure(top, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE);
	Dx12::Runtime::SetUavResourceBarrier(top);
}

/* 終了 */
void Destructor(void)
{
	delete output;
	delete heap;
	delete pipe;
	delete global;
	delete miss_table;
	delete miss_collection;
	delete miss;
	delete closesthit_table;
	delete closesthit_collection;
	delete closest_hit;
	delete ray_gen_table;
	delete ray_gen_constant;
	delete ray_gen_collection;
	delete ray_gen;
	delete top;
	delete bottom;
	delete index;
	delete vertex;
}

/* エントリーポイント */
int main()
{
	Dx12::Runtime::Initialize(win_size);
	Constructor();

	while (Window::CheckMsg() == true)
	{
		Dx12::Runtime::Clear();
		UpdateAccelerationStructure();
		Dx12::Runtime::SetComputeRootSignature(global);
		Dx12::Runtime::SetRaytracingPipeline(pipe);
		Dx12::Runtime::SetDescriptorHeap({ heap });
		Dx12::Runtime::SetComputeResource(top, 0);
		Dx12::Runtime::SetComputeResource(output, 1);
		Dx12::Runtime::DispatchRays(ray_gen_table, closesthit_table, miss_table);
		Dx12::Runtime::Execution();
	}

	Destructor();
	Dx12::Runtime::UnInitialize();
	return 0;
}










//namespace
//{
//	/* ウィンドウサイズ */
//	const Math::Vec2 window_size(640, 480);
//
//	/* 頂点情報 */
//	struct VertexInfo
//	{
//		/* 座標 */
//		Math::Vec3f pos{ 0.0f };
//		/* テクスチャ座標 */
//		Math::Vec2f uv{ 0.0f };
//	};
//	VertexInfo vertex_info[] = 
//	{
//		/* 左から右 */
//		/* 上から下 */
//		{ Math::Vec3f(-1.0f, -1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f) },
//		{ Math::Vec3f( 1.0f, -1.0f, 0.0f), Math::Vec2f(1.0f, 0.0f) },
//		{ Math::Vec3f(-1.0f,  1.0f, 0.0f), Math::Vec2f(0.0f, 1.0f) },
//		{ Math::Vec3f( 1.0f,  1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f) },
//	};
//	/* インデックス情報 */
//	std::uint16_t index_info[] = 
//	{
//		0, 1, 2,
//		1, 2, 3,
//	};
//
//	/* シェーダーファイルディレクトリ */
//	const std::string shader_dir = "../code/hlsl/";
//	/* シェーダエントリーポイント */
//	const std::string shader_func = "main";
//	/* シェーダーモデル */
//	const std::string shader_model = "_6_3";
//
//	/* 入力属性 */
//	const std::vector<D3D12_INPUT_ELEMENT_DESC>input = {
//		{ "POSITION", 0,
//			DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0,
//			DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//	};
//
//	/* カメラ情報 */
//	struct Camera
//	{
//		/* 座標 */
//		Math::Vec3f pos{ 0.0f };
//		/* 視野角 */
//		float fov{ 90.0f };
//		/* 焦点座標 */
//		Math::Vec3f target{ 0.0f };
//		/* レンズの大きさ */
//		float lens_aperture{ 0.0f };
//		/* アップベクトル */
//		Math::Vec3f up{ Math::Vec3f(0.0f, 1.0f, 0.0f) };
//		/* アライメント */
//		float alignment{ 0.0f };
//	};
//	/* マテリアル種別 */
//	enum class Material : std::uint32_t
//	{
//		/* マテリアル無し */
//		None,
//		/* ランバート反射 */
//		Lambert,
//		/* 鏡面反射 */
//		reflect,
//		/* 屈折 */
//		Refract,
//	};
//	/* 球体情報 */
//	struct Sphere
//	{
//		/* 中心座標 */
//		Math::Vec3f center{ 0.0f };
//		/* 半径 */
//		float radius{ 0.0f };
//		/* 色 */
//		Math::Vec3f color{ 1.0f };
//		/* マテリアルタイプ */
//		Material material{ Material::None };
//		/* 移動量 */
//		Math::Vec3f velocity{ 0.0f };
//		/* アライメント */
//		float alignment{ 0.0f };
//	};
//	/* 球体の最大数 */
//	const std::uint32_t sphere_max = 2;
//}
//
//#include "include/TextureLoader.h"
//int main()
//{
//	Tex::TextureInfo tex;
//	Tex::TextureLoader::LoadFromFile("../material/texture/test.bmp", &tex);
//
//
//
//
//	Dx12::Runtime::Initialize(window_size);
//
//	/*==========テクスチャ関連==========*/
//	Dx12::Resource* texture_vertex              = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(vertex_info));
//	Dx12::Resource* texture_index               = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(index_info));
//	Dx12::Resource* texture                     = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetDefaultProp(), DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, tex.width, tex.height, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
//	Dx12::DescriptorHeap* texture_heap          = new Dx12::DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
//	Dx12::ShaderCompiler* texture_shader_vertex = new Dx12::ShaderCompiler(shader_dir + "Texture/Vertex.hlsl", shader_func, "vs" + shader_model);
//	Dx12::ShaderCompiler* texture_shader_pixel  = new Dx12::ShaderCompiler(shader_dir + "Texture/Pixel.hlsl", shader_func, "ps" + shader_model);
//	Dx12::RootSignature* texture_root           = new Dx12::RootSignature(texture_shader_vertex);
//	Dx12::GraphicsPipeline* texture_pipe        = new Dx12::GraphicsPipeline(input, texture_root, texture_shader_vertex, texture_shader_pixel);
//	/* GPUへ頂点情報を送信 */
//	{
//		auto* buffer = texture_vertex->GetBuffer();
//		std::memcpy(buffer, &vertex_info[0], sizeof(vertex_info));
//		texture_vertex->ReleaseBuffer();
//	}
//	/* GPUへインデックス情報を送信 */
//	{
//		auto* buffer = texture_index->GetBuffer();
//		std::memcpy(buffer, &index_info[0], sizeof(index_info));
//		texture_index->ReleaseBuffer();
//	}
//	/* ビューの生成 */
//	{
//		texture_heap->CreateShaderResourceView(texture);
//	}
//
//	/*==========レイトレーシング関連==========*/
//	Dx12::Resource* camera                  = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), (sizeof(Camera) + 0xff) & ~0xff);
//	Dx12::Resource* sphere                  = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, Dx12::Resource::GetDefaultProp(), (sizeof(Sphere) * sphere_max), D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
//	Dx12::Resource* result                  = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, Dx12::Resource::GetDefaultProp(), DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, window_size.x, window_size.y, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
//	Dx12::DescriptorHeap* raytracing_heap   = new Dx12::DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3);
//	Dx12::ShaderCompiler* sphere_shader     = new Dx12::ShaderCompiler(shader_dir + "Raytracing/Sphere.hlsl", shader_func, "cs" + shader_model);
//	Dx12::RootSignature* sphere_root        = new Dx12::RootSignature(sphere_shader);
//	Dx12::ComputePipeline* sphere_pipe      = new Dx12::ComputePipeline(sphere_root, sphere_shader);
//	Dx12::ShaderCompiler* raytracing_shader = new Dx12::ShaderCompiler(shader_dir + "Raytracing/Raytracing.hlsl", shader_func, "cs" + shader_model);
//	Dx12::RootSignature* raytracing_root    = new Dx12::RootSignature(raytracing_shader);
//	Dx12::ComputePipeline* raytracing_pipe  = new Dx12::ComputePipeline(raytracing_root, raytracing_shader);
//
//	/* ビューの生成 */
//	{
//		raytracing_heap->CreateConstantBufferView(camera);
//		raytracing_heap->CreateUnorderAccessView(sphere, sphere_max);
//		raytracing_heap->CreateUnorderAccessView(result);
//	}
//	/* 球体の生成 */
//	{
//		Dx12::Runtime::Clear();
//
//		Dx12::Runtime::SetComputeRootSignature(sphere_root);
//		Dx12::Runtime::SetComputePipeline(sphere_pipe);
//		Dx12::Runtime::SetDescriptorHeap({ raytracing_heap });
//		Dx12::Runtime::SetComputeResource(sphere, 0);
//
//		Dx12::Runtime::Dispatch(sphere_max);
//		Dx12::Runtime::SetUavResourceBarrier(sphere);
//		
//		Dx12::Runtime::Execution();
//	}
//	/* GPUへカメラ情報を送信 */
//	{
//		Camera camera_info{};
//		camera_info.pos           = Math::Vec3f(13.0f, 2.0f, 3.0f);
//		camera_info.fov           = 20.0f;
//		camera_info.target        = Math::Vec3f(0.0f, 0.0f, 0.0f);
//		camera_info.lens_aperture = 0.0f;
//
//		auto* buffer = camera->GetBuffer();
//		std::memcpy(buffer, &camera_info, sizeof(Camera));
//		camera->ReleaseBuffer();
//	}
//	/* レイトレーシングの実行 */
//	/*{
//		Dx12::Runtime::Clear();
//
//		std::uint32_t param_index = 0;
//
//		Dx12::Runtime::SetComputeRootSignature(raytracing_root);
//		Dx12::Runtime::SetComputePipeline(raytracing_pipe);
//		Dx12::Runtime::SetDescriptorHeap({ raytracing_heap });
//		Dx12::Runtime::SetComputeResource(camera, param_index++);
//		Dx12::Runtime::SetComputeResource(sphere, param_index++);
//		Dx12::Runtime::SetComputeResource(result, param_index++);
//
//		Dx12::Runtime::SetResourceBarrier(result, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
//		Dx12::Runtime::Dispatch(window_size.x, window_size.y);
//		Dx12::Runtime::SetUavResourceBarrier(result);
//		Dx12::Runtime::SetResourceBarrier(result, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
//		
//		Dx12::Runtime::Execution();
//	}*/
//	/* テクスチャの更新 */
//	{
//		Dx12::Runtime::Clear();
//		std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>layout;
//		Dx12::Resource upload = texture->UpdateSubResource(*Tex::TextureLoader::GetData(tex), layout);
//		Dx12::Runtime::SetResourceBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
//		Dx12::Runtime::CopyTextureRegion(texture, &upload, layout);
//		Dx12::Runtime::SetResourceBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
//		Dx12::Runtime::Execution();
//	}
//
//	/*==========メインループ==========*/
//	while (Window::CheckMsg() == true) {
//		
//
//		Dx12::Runtime::Clear();
//
//		/* レイトレーシング結果をレンダーターゲットにコピー*/
//		/*{
//			Dx12::Runtime::SetResourceBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
//			Dx12::Runtime::CopyResource(texture, result);
//			Dx12::Runtime::SetResourceBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
//		}*/
//		/* テクスチャ描画 */
//		{
//			std::uint32_t param_index = 0;
//
//			Dx12::Runtime::SetGraphicsRootSignature(texture_root);
//			Dx12::Runtime::SetGraphicsPipeline(texture_pipe);
//			Dx12::Runtime::SetDescriptorHeap({ texture_heap });
//			Dx12::Runtime::SetGraphicsResource(texture, param_index++);
//			Dx12::Runtime::DrawIndexInstance(texture_vertex, _countof(vertex_info), texture_index, _countof(index_info));
//		}
//
//		Dx12::Runtime::Execution();
//	}
//
//	/*==========メモリ解放==========*/
//	delete raytracing_pipe;
//	delete raytracing_root;
//	delete raytracing_shader;
//	delete raytracing_heap;
//	delete result;
//	delete camera;
//
//	delete sphere_pipe;
//	delete sphere_root;
//	delete sphere_shader;
//	delete sphere;
//
//	delete texture_pipe;
//	delete texture_root;
//	delete texture_shader_pixel;
//	delete texture_shader_vertex;
//	delete texture_heap;
//	delete texture;
//	delete texture_index;
//	delete texture_vertex;
//	Dx12::Runtime::UnInitialize();
//	return 0;
//}