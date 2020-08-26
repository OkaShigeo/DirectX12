#pragma once
#include "BaseObject.h"
#include <cstdint>

namespace Dx12
{
	class Resource;

	class Descriptor :
		public BaseObject<ID3D12DescriptorHeap>
	{
	public:
		static /* �f�B�X�N���v�^�q�[�v�̐��� */
			ID3D12DescriptorHeap* CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type,
				const std::uint64_t& rsc_num, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag);

	public:
		/* �R���X�g���N�^ */
		Descriptor();
		Descriptor(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint64_t& rsc_num, 
			const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		Descriptor(ID3D12DescriptorHeap* heap);
		/* �f�X�g���N�^ */
		~Descriptor();

	public:
		/* RTV�̐��� */
		bool CreateRenderTargetView(Resource* rsc);
		/* CBV�̐��� */
		bool CreateConstantBufferView(Resource* rsc);
		/* SRV�̐��� */
		bool CreateShaderResourceView(Resource* rsc);
		/* UAV�̐���(�o�b�t�@�[) */
		bool CreateUnorderAccessView(Resource* rsc, const std::uint64_t& element_num);
		/* UAV�̐���(�e�N�X�`��) */
		bool CreateUnorderAccessView(Resource* rsc);

	private:
		/* �r���[�����J�E���g */
		std::uint32_t count{ 0 };
	};
}
