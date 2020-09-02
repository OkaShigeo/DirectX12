#include "..\include\AccelerationStructure.h"
#include "..\include\DescriptorHeap.h"
#include "..\include\Resource.h"

Dx12::AcceleratoinStructure::AcceleratoinStructure()
{
}

Dx12::AcceleratoinStructure::~AcceleratoinStructure()
{
	Release();
}

void Dx12::AcceleratoinStructure::Release(void)
{
	for (auto& i : rsc) {
		if (i != nullptr) {
			delete i;
		}
	}
	if (heap != nullptr) {
		delete heap;
	}
}
