#include "include/Runtime.h"

namespace
{
	/* �E�B���h�E�T�C�Y */
	const Math::Vec2 win_size = { 640, 480 };

	/* �V�F�[�_�[�t�@�C���f�B���N�g�� */
	const std::string shader_dir = "../code/hlsl/";
	/* �V�F�[�_�G���g���[�|�C���g */
	const std::string shader_func = "main";
	/* �V�F�[�_�[���f�� */
	const std::string shader_model = "_6_4";

	/* �y�C���[�h */
	struct Payload
	{
		/* �o�̓J���[ */
		float color[4];
	};

	/* ���C�g���[�V���O�̏�� */
	struct RaytracingInfo
	{
		/* ���_ */
		Math::Vec3f eye_pos{ 0.0f, 0.0f, -2.0f };
		/* ���C�̕��� */
		Math::Vec3f direction{ 0.0f, 0.0f, 1.0f };
		/* �q�b�g����J�n���� */
		float tmin{ 0.01f };
		/* �q�b�g����I������ */
		float tmax{ 10000.0f };
	};

	/* ���_���\�[�X */
	Dx12::VertexBuffer* vertex = nullptr;
	
	/* �C���f�b�N�X���\�[�X */
	Dx12::IndexBuffer* index = nullptr;

	/* �{�g�����x�������\�� */
	Dx12::AccelerationStructure* bottom = nullptr;
	/* �g�b�v���x�������\�� */
	Dx12::AccelerationStructure* top = nullptr;

	/* ���C�����V�F�[�_�֐��� */
	Str::String ray_gen_name = "RayGen";
	/* ���C�����V�F�[�_ */
	Dx12::ShaderCompiler* ray_gen = nullptr;
	/* ���C�����R���N�V���� */
	Dx12::RaytracingPipeline* ray_gen_collection = nullptr;
	/* ���C�����V�F�[�_�̒萔���\�[�X */
	Dx12::Resource* ray_gen_constant = nullptr;
	/* ���C�����V�F�[�_�̃V�F�[�_�e�[�u�� */
	Dx12::ShaderTable* ray_gen_table = nullptr;

	/* �ł��߂��q�b�g�V�F�[�_�֐��� */
	Str::String closesthit_name = "ClosestHit";
	/* �q�b�g�O���[�v�� */
	Str::String group_name = "Triangle";
	/* �ł��߂��q�b�g�V�F�[�_ */
	Dx12::ShaderCompiler* closest_hit = nullptr;
	/* �ł��߂��q�b�g�R���N�V���� */
	Dx12::RaytracingPipeline* closesthit_collection = nullptr;
	/* �ł��߂��q�b�g�V�F�[�_�e�[�u�� */
	Dx12::ShaderTable* closesthit_table = nullptr;

	/* �~�X�V�F�[�_�� */
	Str::String miss_name = "Miss";
	/* �~�X�V�F�[�_ */
	Dx12::ShaderCompiler* miss = nullptr;
	/* �~�X�R���N�V���� */
	Dx12::RaytracingPipeline* miss_collection = nullptr;
	/* �~�X�V�F�[�_�e�[�u�� */
	Dx12::ShaderTable* miss_table = nullptr;

	/* �O���[�o�����[�g�V�O�l�`�� */
	Dx12::RootSignature* global = nullptr;
	/* �p�C�v���C�� */
	Dx12::RaytracingPipeline* pipe = nullptr;

	/* �q�[�v */
	Dx12::DescriptorHeap* heap = nullptr;
	/* �o�̓f�[�^ */
	Dx12::Resource* output = nullptr;
}

/* ���_�̏����� */
void InitializeVertex(void)
{
	std::vector<Math::Vec3f>vertex_info =
	{
		/* ������E */
		/* �ォ�牺 */
		{ Math::Vec3f(-1.0f, -1.0f, 0.0f) },
		{ Math::Vec3f( 1.0f, -1.0f, 0.0f) },
		{ Math::Vec3f(-1.0f,  1.0f, 0.0f) },
		{ Math::Vec3f( 1.0f,  1.0f, 0.0f) },
	};

	vertex = new Dx12::VertexBuffer(vertex_info.data(), vertex_info.size());
}

/* �C���f�b�N�X�̏����� */
void InitializeIndex(void)
{
	/* �C���f�b�N�X��� */
	std::vector<std::uint16_t>index_info =
	{
		0, 1, 2,
		1, 2, 3,
	};

	index = new Dx12::IndexBuffer(index_info.data(), index_info.size());
}

/* �����\���̏����� */
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

/* ���C�����̃T�u�I�u�W�F�N�g�̏����� */
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

/* �ł��߂��q�b�g�̃T�u�I�u�W�F�N�g�̏����� */
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

/* �~�X�̃T�u�I�u�W�F�N�g�̏����� */
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

/* �T�u�I�u�W�F�N�g�̏����� */
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

/* ���C�����V�F�[�_�e�[�u���̏����� */
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

/* �ł��߂��q�b�g�V�F�[�_�e�[�u���̏����� */
void InitializeClosestHitShaderTable(void)
{
	closesthit_table = new Dx12::ShaderTable(1);
	auto* buffer = closesthit_table->GetBuffer();
	std::memcpy(buffer, pipe->GetShaderIdentifier(group_name), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	closesthit_table->ReleaseBuffer();
}

/* �~�X�֘A */
void InitializeMissShaderTable(void)
{
	miss_table = new Dx12::ShaderTable(1);
	auto* buffer = miss_table->GetBuffer();
	std::memcpy(buffer, pipe->GetShaderIdentifier(miss_name), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	miss_table->ReleaseBuffer();
}

/* �V�F�[�_�e�[�u���̏����� */
void InitializeShaderTable(void)
{
	InitializeRayGenShaderTable();
	InitializeClosestHitShaderTable();
	InitializeMissShaderTable();
}

/* ������ */
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

/* �����\���̍X�V */
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

/* �I�� */
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

/* �G���g���[�|�C���g */
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
//	/* �E�B���h�E�T�C�Y */
//	const Math::Vec2 window_size(640, 480);
//
//	/* ���_��� */
//	struct VertexInfo
//	{
//		/* ���W */
//		Math::Vec3f pos{ 0.0f };
//		/* �e�N�X�`�����W */
//		Math::Vec2f uv{ 0.0f };
//	};
//	VertexInfo vertex_info[] = 
//	{
//		/* ������E */
//		/* �ォ�牺 */
//		{ Math::Vec3f(-1.0f, -1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f) },
//		{ Math::Vec3f( 1.0f, -1.0f, 0.0f), Math::Vec2f(1.0f, 0.0f) },
//		{ Math::Vec3f(-1.0f,  1.0f, 0.0f), Math::Vec2f(0.0f, 1.0f) },
//		{ Math::Vec3f( 1.0f,  1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f) },
//	};
//	/* �C���f�b�N�X��� */
//	std::uint16_t index_info[] = 
//	{
//		0, 1, 2,
//		1, 2, 3,
//	};
//
//	/* �V�F�[�_�[�t�@�C���f�B���N�g�� */
//	const std::string shader_dir = "../code/hlsl/";
//	/* �V�F�[�_�G���g���[�|�C���g */
//	const std::string shader_func = "main";
//	/* �V�F�[�_�[���f�� */
//	const std::string shader_model = "_6_3";
//
//	/* ���͑��� */
//	const std::vector<D3D12_INPUT_ELEMENT_DESC>input = {
//		{ "POSITION", 0,
//			DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0,
//			DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//	};
//
//	/* �J������� */
//	struct Camera
//	{
//		/* ���W */
//		Math::Vec3f pos{ 0.0f };
//		/* ����p */
//		float fov{ 90.0f };
//		/* �œ_���W */
//		Math::Vec3f target{ 0.0f };
//		/* �����Y�̑傫�� */
//		float lens_aperture{ 0.0f };
//		/* �A�b�v�x�N�g�� */
//		Math::Vec3f up{ Math::Vec3f(0.0f, 1.0f, 0.0f) };
//		/* �A���C�����g */
//		float alignment{ 0.0f };
//	};
//	/* �}�e���A����� */
//	enum class Material : std::uint32_t
//	{
//		/* �}�e���A������ */
//		None,
//		/* �����o�[�g���� */
//		Lambert,
//		/* ���ʔ��� */
//		reflect,
//		/* ���� */
//		Refract,
//	};
//	/* ���̏�� */
//	struct Sphere
//	{
//		/* ���S���W */
//		Math::Vec3f center{ 0.0f };
//		/* ���a */
//		float radius{ 0.0f };
//		/* �F */
//		Math::Vec3f color{ 1.0f };
//		/* �}�e���A���^�C�v */
//		Material material{ Material::None };
//		/* �ړ��� */
//		Math::Vec3f velocity{ 0.0f };
//		/* �A���C�����g */
//		float alignment{ 0.0f };
//	};
//	/* ���̂̍ő吔 */
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
//	/*==========�e�N�X�`���֘A==========*/
//	Dx12::Resource* texture_vertex              = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(vertex_info));
//	Dx12::Resource* texture_index               = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(index_info));
//	Dx12::Resource* texture                     = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetDefaultProp(), DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, tex.width, tex.height, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
//	Dx12::DescriptorHeap* texture_heap          = new Dx12::DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
//	Dx12::ShaderCompiler* texture_shader_vertex = new Dx12::ShaderCompiler(shader_dir + "Texture/Vertex.hlsl", shader_func, "vs" + shader_model);
//	Dx12::ShaderCompiler* texture_shader_pixel  = new Dx12::ShaderCompiler(shader_dir + "Texture/Pixel.hlsl", shader_func, "ps" + shader_model);
//	Dx12::RootSignature* texture_root           = new Dx12::RootSignature(texture_shader_vertex);
//	Dx12::GraphicsPipeline* texture_pipe        = new Dx12::GraphicsPipeline(input, texture_root, texture_shader_vertex, texture_shader_pixel);
//	/* GPU�֒��_���𑗐M */
//	{
//		auto* buffer = texture_vertex->GetBuffer();
//		std::memcpy(buffer, &vertex_info[0], sizeof(vertex_info));
//		texture_vertex->ReleaseBuffer();
//	}
//	/* GPU�փC���f�b�N�X���𑗐M */
//	{
//		auto* buffer = texture_index->GetBuffer();
//		std::memcpy(buffer, &index_info[0], sizeof(index_info));
//		texture_index->ReleaseBuffer();
//	}
//	/* �r���[�̐��� */
//	{
//		texture_heap->CreateShaderResourceView(texture);
//	}
//
//	/*==========���C�g���[�V���O�֘A==========*/
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
//	/* �r���[�̐��� */
//	{
//		raytracing_heap->CreateConstantBufferView(camera);
//		raytracing_heap->CreateUnorderAccessView(sphere, sphere_max);
//		raytracing_heap->CreateUnorderAccessView(result);
//	}
//	/* ���̂̐��� */
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
//	/* GPU�փJ�������𑗐M */
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
//	/* ���C�g���[�V���O�̎��s */
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
//	/* �e�N�X�`���̍X�V */
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
//	/*==========���C�����[�v==========*/
//	while (Window::CheckMsg() == true) {
//		
//
//		Dx12::Runtime::Clear();
//
//		/* ���C�g���[�V���O���ʂ������_�[�^�[�Q�b�g�ɃR�s�[*/
//		/*{
//			Dx12::Runtime::SetResourceBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
//			Dx12::Runtime::CopyResource(texture, result);
//			Dx12::Runtime::SetResourceBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
//		}*/
//		/* �e�N�X�`���`�� */
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
//	/*==========���������==========*/
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