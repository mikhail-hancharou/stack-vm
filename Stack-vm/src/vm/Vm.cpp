#include "../../include/vm/Vm.h"

using namespace vm;
using namespace std;


// Allocate RAM
VirtualMachine::VirtualMachine(WORD memorySize = 0xFFFF)
{
	maxAddress = memorySize / sizeof(WORD);
	memory = new WORD[maxAddress];
	memset(memory, 0, sizeof(memory));
}

// Release RAM
VirtualMachine::~VirtualMachine()
{
	delete[]memory;
}

bool VirtualMachine::loadImage(ExecutableImage& const image)
{
	if (image.getSize() > maxAddress)
	{
		return false;
	}

	memcpy(memory, image.getImage(), image.getSize() * sizeof(WORD));
	return true;
}