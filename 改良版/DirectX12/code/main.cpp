#include "include/Runtime.h"

int main()
{
	Dx12::Runtime::Initialize(Dx12::Vec2(640, 480));

	while (Window::CheckMsg() == true) {
		
	}

	Dx12::Runtime::UnInitialize();
	return 0;
}