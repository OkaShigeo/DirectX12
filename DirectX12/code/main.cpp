#include "include/Runtime.h"
#include <random>

namespace
{
	/* �E�B���h�E�T�C�Y */
	const Dx12::Vec2 window_size(640, 480);

	/* ���_��� */
	struct VertexInfo
	{
		/* ���W */
		Dx12::Vec3f pos{ 0.0f };
		/* �e�N�X�`�����W */
		Dx12::Vec2f uv{ 0.0f };
	};
	VertexInfo vertex_info[] = 
	{
		/* ������E */
		/* �ォ�牺 */
		{ Dx12::Vec3f(-1.0f, -1.0f, 0.0f), Dx12::Vec2f(0.0f, 0.0f) },
		{ Dx12::Vec3f( 1.0f, -1.0f, 0.0f), Dx12::Vec2f(1.0f, 0.0f) },
		{ Dx12::Vec3f(-1.0f,  1.0f, 0.0f), Dx12::Vec2f(0.0f, 1.0f) },
		{ Dx12::Vec3f( 1.0f,  1.0f, 0.0f), Dx12::Vec2f(1.0f, 1.0f) },
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
		Dx12::Vec3f pos{ 0.0f };
		/* ����p */
		float fov{ 90.0f };
		/* �œ_���W */
		Dx12::Vec3f target{ 0.0f };
		/* �����Y�̑傫�� */
		float lens{ 0.0f };
		/* �A�b�v�x�N�g�� */
		Dx12::Vec3f up{ Dx12::Vec3f(0.0f, 1.0f, 0.0f) };
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
	/* Axis Aligned Bounding Box */
	struct AABB
	{
		/* �ŏ��ʒu */
		Dx12::Vec3f min{ 0.0f };
		/* �ő�ʒu */
		Dx12::Vec3f max{ 0.0f };

		/* ���S�ʒu�̎擾 */
		Dx12::Vec3f GetCenter(void) const
		{
			return (min - max) / 2.0f;
		}
		/* �����̖̂ʐς̎擾 */
		float GetArea(void) const
		{
			return 2.0f * std::pow((max.x - min.x), 2.0f) + std::pow((max.y - min.y), 2.0f) + std::pow((max.z - min.z), 2.0f);
		}
		/* �T�C�Y�̎擾 */
		std::uint32_t GetSize(void) const
		{
			return sizeof(min) + sizeof(max);
		}
		/* AABB�̃}�[�W */
		static AABB MergeAABB(const AABB& box1, const AABB& box2)
		{
			return { Dx12::Vec3f(std::fmin(box1.min.x, box2.min.x), std::fmin(box1.min.y, box2.min.y), std::fmin(box1.min.z, box2.min.z)),
					 Dx12::Vec3f(std::fmax(box1.max.x, box2.max.x), std::fmax(box1.max.y, box2.max.y), std::fmax(box1.max.z, box2.max.z)) };
		}
	};
	/* ���̏�� */
	struct Sphere
	{
		/* ���S���W */
		Dx12::Vec3f center{ 0.0f };
		/* ���a */
		float radius{ 0.0f };
		/* �F */
		Dx12::Vec3f color{ 1.0f };
		/* �}�e���A���^�C�v */
		Material material{ Material::None };
		/* �ړ��� */
		Dx12::Vec3f velocity{ 0.0f };
		/* �A���C�����g */
		float alignment{ 0.0f };

		/* AABB�̎擾 */
		AABB GetAABB(void) const
		{
			AABB min = { center - radius, center + radius };
			AABB max = { (center + velocity) - radius, (center + velocity) + radius };

			return AABB::MergeAABB(min, max);
		}
	};
	/* BVH�m�[�h */
	template <typename T>
	struct BoundingVolume
	{
		/* AABB */
		AABB aabb{};
		/* ���m�[�h�ԍ� */
		std::int32_t left{ 0 };
		/* �E�m�[�h�ԍ� */
		std::int32_t right{ 0 };
		/* �I�u�W�F�N�g���X�g */
		std::vector<T>obj;

		/* �T�C�Y�̎擾 */
		std::uint32_t GetSize(void) const
		{
			return aabb.GetSize() + (sizeof(std::int32_t) * 2) + (sizeof(T) * obj.size());
		}
	};
	/* Bounding Volume Hierarchy */
	template <typename T>
	struct BVH
	{
		/* �m�[�h */
		std::vector<BoundingVolume<T>>node;

		/* �f�[�^�̍\�z */
		void Construction(const std::vector<T>& obj, const bool& reset = true)
		{
			static std::uint32_t node_index = 0;
			if (reset == true) {
				node_index = 1;
			}

			BoundingVolume node{};
			for (std::uint32_t i = 0; i < obj.size(); ++i) {
				node.aabb = AABB::MergeAABB(node.aabb, obj[i]);
			}

			/* SAH�̒l */
			float sah = float(obj.size());
			/* ������ */
			std::int32_t axis = -1;
			/* ������ */
			std::int32_t sprite = -1;

			/* �\�[�g���� */
			std::vector<AABB>sort = obj;

			for (std::uint32_t i = 0; i < 3; ++i) {
				/* �e���̒��S���W�Ń\�[�g */
				std::sort(sort.begin(), sort.end(), [i](const AABB& a, const AABB& b) {
					return a.GetCenter().value[i] < b.GetCenter().value[i];
				});

				AABB tmp = {};
				std::vector<float>area1;
				for (std::uint32_t n = 0; n < sort.size(); ++n) {
					tmp = AABB::MergeAABB(tmp, sort[n]);
					area1.push_back(std::abs(tmp.GetArea()));
				}

				tmp = {};
				std::vector<float>area2;
				for (std::int32_t n = std::int32_t(sort.size() - 1); n >= 0; --n) {
					tmp = AABB::MergeAABB(tmp, sort[n]);
					area2.push_back(std::abs(tmp.GetArea()));

					/* SAH�̎Z�o */
					float cost = 2.0f + (area1[n] * n + (*area2.rbegin()) * (sort.size() - n)) / node.aabb.GetArea();
					if (sah > cost) {
						sah    = cost;
						axis   = i;
						sprite = n;
					}
				}
			}

			if (axis != -1) {
				std::vector<AABB>sort = obj;
				std::sort(sort.begin(), sort.end(), [axis](const AABB& a, const AABB& b)->bool {
					return a.GetCenter().value[axis] < b.GetCenter().value[axis];
				});

				node.left  = node_index++;
				node.right = node_index++;

				this->node.push_back(node);

				Construction(std::vector<AABB>(sort.begin(), sort.begin() + sprite), false);
				Construction(std::vector<AABB>(sort.begin() + sprite, sort.end()), false);
			}
			else {
				node.left = node.right = -1;
				node.obj = obj;

				this->node.push_back(node);
			}
		}
	};
	/* ���̂̍ő吔 */
	const std::uint32_t sphere_max = 128;
	/* ���T���C�g���[�V���O�p�����[�^ */
	struct FirstRaytracingParam
	{
		/* �J������� */
		Camera cam;
		/* ���̂̐� */
		std::uint32_t sphere_cnt{ 0 };
		/* ���̏�� */
		std::vector<Sphere>sp;

		/* ���̂̒ǉ� */
		void AddSphere(const Sphere& sphere)
		{
			if (sphere_cnt < sphere_max) {
				sp.push_back(sphere);
				sphere_cnt++;
			}
		}
		/* �f�[�^�̃R�s�[ */
		void Copy(std::uint8_t* buffer)
		{
			std::uint32_t index = 0;
			std::memcpy(&buffer[index], &cam, sizeof(Camera));
			index += sizeof(Camera);
			std::memcpy(&buffer[index], &sphere_cnt, sizeof(std::uint32_t));
			index += sizeof(std::uint32_t);
			std::memcpy(&buffer[index], sp.data(), sizeof(Sphere) * sp.size());
		}
		/* �T�C�Y�̎擾 */
		std::uint32_t GetSize(void) const 
		{
			return sizeof(cam) + sizeof(sphere_cnt) + (sizeof(Sphere) * sphere_cnt);
		}
		/* �ő�T�C�Y�̎擾 */
		static std::uint32_t GetMaxSize(void)
		{
			return sizeof(Camera) + sizeof(std::uint32_t) + (sizeof(Sphere) * sphere_max);
		}
	};
}

int main()
{
	Dx12::Runtime::Initialize(window_size);

	Dx12::Resource* vertex_rsc            = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(vertex_info));
	Dx12::Resource* index_rsc             = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(index_info));
	Dx12::Resource* texture			      = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetDefaultProp(), DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, window_size.x, window_size.y, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	Dx12::Descriptor* heap                = new Dx12::Descriptor(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
	Dx12::ShaderCompiler* vertex          = new Dx12::ShaderCompiler(shader_dir + L"TextureVertex.hlsl", shader_func, L"vs" + shader_model);
	Dx12::ShaderCompiler* pixel           = new Dx12::ShaderCompiler(shader_dir + L"TexturePixel.hlsl", shader_func, L"ps" + shader_model);
	Dx12::RootSignature* graphics_root    = new Dx12::RootSignature(vertex);
	Dx12::GraphicsPipeline* graphics_pipe = new Dx12::GraphicsPipeline(input, graphics_root, vertex, pixel);

	std::vector<Dx12::Resource*>compute_rsc =
	{
		new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), (FirstRaytracingParam::GetMaxSize() + 0xff) & ~0xff),
		new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, Dx12::Resource::GetDefaultProp(), 
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,window_size.x, window_size.y, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
	};
	Dx12::Descriptor* compute_heap      = new Dx12::Descriptor(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, compute_rsc.size());
	Dx12::ShaderCompiler* compute       = new Dx12::ShaderCompiler(shader_dir + L"FirstRaytracing.hlsl", shader_func, L"cs" + shader_model);
	Dx12::RootSignature* compute_root   = new Dx12::RootSignature(compute);
	Dx12::ComputePipeline* compute_pipe = new Dx12::ComputePipeline(compute_root, compute);

	/* GPU�֒��_���𑗐M */
	{
		auto* buffer = vertex_rsc->GetBuffer();
		std::memcpy(buffer, &vertex_info[0], sizeof(vertex_info));
		vertex_rsc->ReleaseBuffer();
	}
	/* GPU�փC���f�b�N�X���𑗐M */
	{
		auto* buffer = index_rsc->GetBuffer();
		std::memcpy(buffer, &index_info[0], sizeof(index_info));
		index_rsc->ReleaseBuffer();
	}
	/* �r���[�̐��� */
	{
		heap->CreateShaderResourceView(texture);
	}

	/* �r���[�̐��� */
	{
		std::uint32_t index = 0;
		compute_heap->CreateConstantBufferView(compute_rsc[index++]);
		compute_heap->CreateUnorderAccessView(compute_rsc[index++]);
	}
	/* GPU�֌v�Z�p�f�[�^�𑗐M */
	{
		FirstRaytracingParam param{};
		param.cam.pos    = Dx12::Vec3f(13.0f, 2.0f, 3.0f);
		param.cam.fov    = 20.0f;
		param.cam.target = Dx12::Vec3f(0.0f, 0.0f, 0.0f);
		param.cam.lens   = 0.1f;

		if (param.sphere_cnt < sphere_max) {
			param.AddSphere({ Dx12::Vec3f(0.0f, -1000.0f, 0.0f), 1000.0f, Dx12::Vec3f(0.5f, 0.5f, 0.5f), Material::Lambert });
			param.AddSphere({ Dx12::Vec3f(0.0f,     1.0f, 0.0f),    1.0f, Dx12::Vec3f(1.0f, 1.0f, 1.0f), Material::Refract });
			param.AddSphere({ Dx12::Vec3f(-4.0f,    1.0f, 0.0f),    1.0f, Dx12::Vec3f(0.4f, 0.2f, 0.1f), Material::Lambert });
			param.AddSphere({ Dx12::Vec3f(4.0f,     1.0f, 0.0f),    1.0f, Dx12::Vec3f(0.7f, 0.6f, 0.5f), Material::reflect });
		}

		Sphere sp{};
		for (std::int32_t i = -5; i < 5; ++i)
		{
			for (std::int32_t n = -5; n < 5; ++n)
			{
				if (param.sphere_cnt < sphere_max) {
					std::random_device seed;
					std::mt19937 mt(seed());
					std::uniform_real_distribution<float>rand(-1.0f, 1.0f);
					Dx12::Vec3f center = Dx12::Vec3f(i + 0.9f * rand(mt), 0.2f, n + 0.9f * rand(mt));

					if ((center - Dx12::Vec3f(4.0f, 0.2f, 0.0f)).Length() > 0.9f)
					{
						std::uniform_real_distribution<float>color(0.0f, 1.0f);
						sp.center = center;
						sp.color  = Dx12::Vec3f(color(mt), color(mt), color(mt));
						sp.radius = 0.2f;

						std::uniform_real_distribution<float>velocity(0.0f, 0.3f);
						sp.velocity.x = velocity(mt);
						sp.velocity.y = velocity(mt);
						sp.velocity.z = velocity(mt);

						if (rand(mt) < 0.4f)
						{
							sp.material = Material::Lambert;
						}
						else if (rand(mt) < 0.8f)
						{
							sp.material = Material::reflect;
						}
						else
						{
							sp.material = Material::Refract;
						}

						param.AddSphere(sp);
					}
				}
			}
		}

		auto* buffer = (*compute_rsc.begin())->GetBuffer();
		param.Copy(buffer);
		(*compute_rsc.begin())->ReleaseBuffer();
	}

	while (Window::CheckMsg() == true) {
		Dx12::Runtime::Clear();

		/* �v���O���}�u�� */
		{
			std::uint32_t rsc_index   = 0;
			std::uint32_t param_index = 0;

			Dx12::Runtime::SetComputeRootSignature(compute_root);
			Dx12::Runtime::SetComputePipeline(compute_pipe);
			Dx12::Runtime::SetDescriptorHeap({ compute_heap });
			Dx12::Runtime::SetComputeResource(compute_rsc[rsc_index++], param_index++);
			Dx12::Runtime::SetComputeResource(compute_rsc[rsc_index], param_index++);

			Dx12::Runtime::SetRscBarrier(compute_rsc[rsc_index],
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			Dx12::Runtime::Dispatch(window_size.x, window_size.y);
			Dx12::Runtime::SetUavRscBarrier(compute_rsc[rsc_index]);
			Dx12::Runtime::SetRscBarrier(compute_rsc[rsc_index++],
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
		}
		/* �v���O���}�u�����\�[�X���O���t�B�b�N�X���\�[�X�ɃR�s�[ */
		{
			Dx12::Runtime::SetRscBarrier(texture,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
			Dx12::Runtime::CopyResource(texture, (*compute_rsc.rbegin()));
			Dx12::Runtime::SetRscBarrier(texture,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
		}
		/* �O���t�B�b�N�X */
		{
			std::uint32_t param_index = 0;

			Dx12::Runtime::SetGraphicsRootSignature(graphics_root);
			Dx12::Runtime::SetGraphicsPipeline(graphics_pipe);
			Dx12::Runtime::SetDescriptorHeap({ heap });
			Dx12::Runtime::SetGraphicsResource(texture, param_index++);
			//Dx12::Runtime::DrawVertexInstance(vertex_rsc, _countof(vertex_info));
			Dx12::Runtime::DrawIndexInstance(vertex_rsc, _countof(vertex_info), index_rsc, _countof(index_info));
		}

		Dx12::Runtime::Execution({});
	}

	delete compute_pipe;
	delete compute_root;
	delete compute;
	delete compute_heap;
	for (auto& i : compute_rsc) {
		delete i;
	}

	delete graphics_pipe;
	delete graphics_root;
	delete pixel;
	delete vertex;
	delete heap;
	delete texture;
	delete index_rsc;
	delete vertex_rsc;
	Dx12::Runtime::UnInitialize();
	return 0;
}