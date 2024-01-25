#include <iostream>
#include "../../include/vm/Vm.h"

using namespace vm;
using namespace std;

ExecutableImage::ExecutableImage()
{
	clear();
}

ExecutableImage::~ExecutableImage()
{
	clear();
}

void ExecutableImage::clear()
{
	image.clear();
	emitAddress = 0;
}

// Checks available space and resize image if required
void ExecutableImage::prepareSpace(WORD wordsCount)
{
	WORD required = emitAddress + wordsCount;
	if (image.size() < required)
	{
		image.resize(required);
	}
}

void ExecutableImage::prepareSpace(WORD address, WORD wordsCount)
{
	WORD required = address + wordsCount;
	if (image.size() < required)
	{
		image.resize(required);
	}
}

// Sets EmitAddress value to specified address
WORD ExecutableImage::setEmitAddress(WORD address) 
{
	return emitAddress = address;
}


// Returns EmitAddress value
WORD ExecutableImage::getEmitAddress() 
{
	return emitAddress;
}

// Writes opcode to executable image at current EmitAddress
WORD ExecutableImage::emit(WORD opcode) 
{
	prepareSpace(1);
	WORD startAddress = emitAddress;
	image[emitAddress++] = opcode;
	return startAddress;
}

// Writes opcode and its operand to executable image at current EmitAddress
WORD ExecutableImage::emit(WORD opcode, WORD operand) 
{
	prepareSpace(2);
	WORD startAddress = emitAddress;
	image[emitAddress++] = opcode;
	image[emitAddress++] = operand;
	return startAddress;
}

// Writes opcode and its operands to executable image at current EmitAddress
WORD ExecutableImage::emit(WORD opcode, WORD operand1, WORD operand2) 
{
	prepareSpace(3);
	WORD startAddress = emitAddress;
	image[emitAddress++] = opcode;
	image[emitAddress++] = operand1;
	image[emitAddress++] = operand2;
	return startAddress;
}

// Writes specified image to this executable image at current EmitAddress
WORD ExecutableImage::emit(ExecutableImage& img) 
{
	WORD startAddress = emitAddress;
	WORD wordsCount = img.getSize();
	prepareSpace(wordsCount);
	memcpy(image.data() + emitAddress, img.getImage(), wordsCount * sizeof(WORD));
	emitAddress += wordsCount;
	return startAddress;
}

// Write WORD to specified memory address
void ExecutableImage::writeWord(WORD address, WORD value) 
{
	WORD temp = emitAddress;
	prepareSpace(address, 1);
	image[address] = value;
	emitAddress = temp;
}

// Writes data to executable image at current EmitAddress
void ExecutableImage::writeData(WORD address, void* data, WORD bytesCount) 
{
	WORD reminder = bytesCount % sizeof(WORD);
	WORD wordsCount = bytesCount / sizeof(WORD);
	wordsCount += reminder != 0 ? 1 : 0;
	//if (reminder != 0) wordsCount++;
	prepareSpace(address, wordsCount);
	memcpy(image.data() + address, data, bytesCount);
}

// Reads WORD from executable image at specified EmitAddress
WORD ExecutableImage::readWord(WORD address) 
{
	return image[address];
}

// Returns pointer to executable image
WORD* ExecutableImage::getImage() {
	return image.data();
}

// Returns memory size in words
WORD ExecutableImage::getSize() {
	return (WORD)image.size();
}

// Disassembles executable image to console
void ExecutableImage::disassemble() 
{
	cout << "-----------------------------------------------------" << endl;
	cout << "Virtual machine executable image disassembly" << endl;
	cout << "-----------------------------------------------------" << endl;
	if (image.size() == 0) return;
	WORD opcode;
	WORD previousOp = -1;
	WORD ip = 0;
	do 
	{
		opcode = image[ip];
		if (opcode != OP_HALT) ip += printMnemomic(ip);
		else {
			if (previousOp != OP_HALT) printMnemomic(ip);
			ip++;
		}
		previousOp = opcode;
	} while (ip < image.size());
}