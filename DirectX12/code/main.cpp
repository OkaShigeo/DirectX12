#include "include/Runtime.h"

namespace
{
	/* �E�B���h�E�T�C�Y */
	const Math::Vec2 window_size(640, 480);

	/* ���_��� */
	struct VertexInfo
	{
		/* ���W */
		Math::Vec3f pos{ 0.0f };
		/* �e�N�X�`�����W */
		Math::Vec2f uv{ 0.0f };
	};
	VertexInfo vertex_info[] = 
	{
		/* ������E */
		/* �ォ�牺 */
		{ Math::Vec3f(-1.0f, -1.0f, 0.0f), Math::Vec2f(0.0f, 0.0f) },
		{ Math::Vec3f( 1.0f, -1.0f, 0.0f), Math::Vec2f(1.0f, 0.0f) },
		{ Math::Vec3f(-1.0f,  1.0f, 0.0f), Math::Vec2f(0.0f, 1.0f) },
		{ Math::Vec3f( 1.0f,  1.0f, 0.0f), Math::Vec2f(1.0f, 1.0f) },
	};
	/* �C���f�b�N�X��� */
	std::uint16_t index_info[] = 
	{
		0, 1, 2,
		1, 2, 3,
	};

	/* �V�F�[�_�[�t�@�C���f�B���N�g�� */
	const std::wstring shader_dir = L"../code/hlsl/";
	/* �V�F�[�_�G���g���[�|�C���g */
	const std::wstring shader_func = L"main";
	/* �V�F�[�_�[���f�� */
	const std::wstring shader_model = L"_6_3";

	/* ���͑��� */
	const std::vector<D3D12_INPUT_ELEMENT_DESC>input = {
		{ "POSITION", 0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	/* �J������� */
	struct Camera
	{
		/* ���W */
		Math::Vec3f pos{ 0.0f };
		/* ����p */
		float fov{ 90.0f };
		/* �œ_���W */
		Math::Vec3f target{ 0.0f };
		/* �����Y�̑傫�� */
		float lens_aperture{ 0.0f };
		/* �A�b�v�x�N�g�� */
		Math::Vec3f up{ Math::Vec3f(0.0f, 1.0f, 0.0f) };
		/* �A���C�����g */
		float alignment{ 0.0f };
	};
	/* �}�e���A����� */
	enum class Material : std::uint32_t
	{
		/* �}�e���A������ */
		None,
		/* �����o�[�g���� */
		Lambert,
		/* ���ʔ��� */
		reflect,
		/* ���� */
		Refract,
	};
	/* ���̏�� */
	struct Sphere
	{
		/* ���S���W */
		Math::Vec3f center{ 0.0f };
		/* ���a */
		float radius{ 0.0f };
		/* �F */
		Math::Vec3f color{ 1.0f };
		/* �}�e���A���^�C�v */
		Material material{ Material::None };
		/* �ړ��� */
		Math::Vec3f velocity{ 0.0f };
		/* �A���C�����g */
		float alignment{ 0.0f };
	};
	/* ���̂̍ő吔 */
	const std::uint32_t sphere_max = 2;
}

#include "include/TextureLoader.h"
int main()
{
	Tex::TextureInfo tex;
	Tex::TextureLoader::LoadFromFile("../material/texture/test.bmp", &tex);




	Dx12::Runtime::Initialize(window_size);

	/*==========�e�N�X�`���֘A==========*/
	Dx12::Resource* texture_vertex              = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(vertex_info));
	Dx12::Resource* texture_index               = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(index_info));
	Dx12::Resource* texture                     = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetDefaultProp(), DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, tex.width, tex.height, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	Dx12::DescriptorHeap* texture_heap          = new Dx12::DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
	Dx12::ShaderCompiler* texture_shader_vertex = new Dx12::ShaderCompiler(shader_dir + L"Texture/Vertex.hlsl", shader_func, L"vs" + shader_model);
	Dx12::ShaderCompiler* texture_shader_pixel  = new Dx12::ShaderCompiler(shader_dir + L"Texture/Pixel.hlsl", shader_func, L"ps" + shader_model);
	Dx12::RootSignature* texture_root           = new Dx12::RootSignature(texture_shader_vertex);
	Dx12::GraphicsPipeline* texture_pipe        = new Dx12::GraphicsPipeline(input, texture_root, texture_shader_vertex, texture_shader_pixel);
	/* GPU�֒��_���𑗐M */
	{
		auto* buffer = texture_vertex->GetBuffer();
		std::memcpy(buffer, &vertex_info[0], sizeof(vertex_info));
		texture_vertex->ReleaseBuffer();
	}
	/* GPU�փC���f�b�N�X���𑗐M */
	{
		auto* buffer = texture_index->GetBuffer();
		std::memcpy(buffer, &index_info[0], sizeof(index_info));
		texture_index->ReleaseBuffer();
	}
	/* �r���[�̐��� */
	{
		texture_heap->CreateShaderResourceView(texture);
	}
	//Dx12::AccelerationStructure a = Dx12::AccelerationStructure::CreateBottomLevel({ texture_vertex }, { 4 }, 0, { texture_index }, { 6 });

	/*==========���C�g���[�V���O�֘A==========*/
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

	/* �r���[�̐��� */
	{
		raytracing_heap->CreateConstantBufferView(camera);
		raytracing_heap->CreateUnorderAccessView(sphere, sphere_max);
		raytracing_heap->CreateUnorderAccessView(result);
	}
	/* ���̂̐��� */
	{
		Dx12::Runtime::Clear();

		Dx12::Runtime::SetComputeRootSignature(sphere_root);
		Dx12::Runtime::SetComputePipeline(sphere_pipe);
		Dx12::Runtime::SetDescriptorHeap({ raytracing_heap });
		Dx12::Runtime::SetComputeResource(sphere, 0);

		Dx12::Runtime::Dispatch(sphere_max);
		Dx12::Runtime::SetUavResourceBarrier(sphere);
		
		Dx12::Runtime::Execution();
	}
	/* GPU�փJ�������𑗐M */
	{
		Camera camera_info{};
		camera_info.pos           = Math::Vec3f(13.0f, 2.0f, 3.0f);
		camera_info.fov           = 20.0f;
		camera_info.target        = Math::Vec3f(0.0f, 0.0f, 0.0f);
		camera_info.lens_aperture = 0.0f;

		auto* buffer = camera->GetBuffer();
		std::memcpy(buffer, &camera_info, sizeof(Camera));
		camera->ReleaseBuffer();
	}
	/* ���C�g���[�V���O�̎��s */
	/*{
		Dx12::Runtime::Clear();

		std::uint32_t param_index = 0;

		Dx12::Runtime::SetComputeRootSignature(raytracing_root);
		Dx12::Runtime::SetComputePipeline(raytracing_pipe);
		Dx12::Runtime::SetDescriptorHeap({ raytracing_heap });
		Dx12::Runtime::SetComputeResource(camera, param_index++);
		Dx12::Runtime::SetComputeResource(sphere, param_index++);
		Dx12::Runtime::SetComputeResource(result, param_index++);

		Dx12::Runtime::SetResourceBarrier(result, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		Dx12::Runtime::Dispatch(window_size.x, window_size.y);
		Dx12::Runtime::SetUavResourceBarrier(result);
		Dx12::Runtime::SetResourceBarrier(result, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
		
		Dx12::Runtime::Execution();
	}*/
	/* �e�N�X�`���̍X�V */
	{
		Dx12::Runtime::Clear();
		std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>layout;
		Dx12::Resource upload = texture->UpdateSubResource(*Tex::TextureLoader::GetData(tex), layout);
		Dx12::Runtime::SetResourceBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
		Dx12::Runtime::CopyTextureRegion(texture, &upload, layout);
		Dx12::Runtime::SetResourceBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
		Dx12::Runtime::Execution();
	}

	/*==========���C�����[�v==========*/
	while (Window::CheckMsg() == true) {
		

		Dx12::Runtime::Clear();

		/* ���C�g���[�V���O���ʂ������_�[�^�[�Q�b�g�ɃR�s�[*/
		/*{
			Dx12::Runtime::SetResourceBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
			Dx12::Runtime::CopyResource(texture, result);
			Dx12::Runtime::SetResourceBarrier(texture, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
		}*/
		/* �e�N�X�`���`�� */
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

	/*==========���������==========*/
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