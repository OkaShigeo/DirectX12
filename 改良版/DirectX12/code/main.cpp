#include "include/Runtime.h"

namespace
{
	/* �E�B���h�E�T�C�Y */
	const Dx12::Vec2 window_size(640, 480);

	/* ���_��� */
	struct VertexInfo
	{
		/* ���W */
		Dx12::Vec3f pos;
		/* �e�N�X�^���W */
		Dx12::Vec2f uv;
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
}

int main()
{
	Dx12::Runtime::Initialize(window_size);

	Dx12::Resource* vertex_rsc   = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(vertex_info));
	Dx12::Resource* index_rsc    = new Dx12::Resource(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, Dx12::Resource::GetUploadProp(), sizeof(index_info));
	Dx12::ShaderCompiler* vertex = new Dx12::ShaderCompiler(shader_dir + L"TextureVertex.hlsl", shader_func, L"vs" + shader_model);
	Dx12::ShaderCompiler* pixel  = new Dx12::ShaderCompiler(shader_dir + L"TexturePixel.hlsl", shader_func, L"ps" + shader_model);
	Dx12::RootSignature* root    = new Dx12::RootSignature(vertex);
	Dx12::GraphicsPipeline* pipe = new Dx12::GraphicsPipeline(input, root, vertex, pixel);

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

	while (Window::CheckMsg() == true) {
		Dx12::Runtime::Clear();

		Dx12::Runtime::SetGraphicsRootSignature(root);
		Dx12::Runtime::SetGraphicsPipeline(pipe);
		//Dx12::Runtime::DrawVertexInstance(vertex_rsc, _countof(vertex_info));
		Dx12::Runtime::DrawIndexInstance(vertex_rsc, _countof(vertex_info), index_rsc, _countof(index_info));

		Dx12::Runtime::Execution();
	}

	delete pipe;
	delete root;
	delete pixel;
	delete vertex;
	delete index_rsc;
	delete vertex_rsc;
	Dx12::Runtime::UnInitialize();
	return 0;
}