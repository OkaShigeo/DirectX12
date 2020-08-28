#pragma once
#include <vector>

namespace Dx12
{
	class DescriptorHeap;
	class Resource;

	class AcceleratoinStructure
	{
	public:
		/* �R���X�g���N�^ */
		AcceleratoinStructure();
		/* �f�X�g���N�^ */
		~AcceleratoinStructure();

	public:
		/* �������̉�� */
		void Release(void);

	private:
		/* �q�[�v */
		DescriptorHeap* heap{ nullptr };
		/* ���\�[�X */
		std::vector<Resource*>rsc;
	};
}
