#include "..\include\AccelerationStructure.h"
#include "..\include\DescriptorHeap.h"
#include "..\include\Resource.h"

Dx12::AcceleratoinStructure::AcceleratoinStructure()
{
}

Dx12::AcceleratoinStructure::~AcceleratoinStructure()
{
}

void Dx12::AcceleratoinStructure::Release(void)
{
	for (auto& i : rsc) {
		i->Release();
	}
	heap->Release();
}
