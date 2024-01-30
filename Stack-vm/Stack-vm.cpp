// Stack-vm.cpp : Defines the entry point for the application.
//

#include "Stack-vm.h"

using namespace vm;
using namespace std;

void createExecutableImage(ExecutableImage* img, WORD iterations) 
{

	WORD dataSeg = 256;							// Data segment starts at 256

	WORD iVar = dataSeg;
	WORD myStr = dataSeg + 1;
	img->writeWord(iVar, iterations);
	img->writeData(myStr, "Hello, world from VM!\n", 23);

	WORD fn = 128;

	WORD addr = img->emit(OP_PUSH, iVar);       // stack <- [iVar] (operand 1)
	img->emit(OP_DEC);                          // stack[top]--  (operand 1 decrement)
	img->emit(OP_CALL, fn);                     // Call function fn()     
	img->emit(OP_DUP);                          // duplicate stack top (operand 1 duplicate)
	img->emit(OP_POP, iVar);                    // stack -> [iVar] (pop operand 1 duplicate to iVar)
	img->emit(OP_CONST, 0);                     // push const 0 (operand 2)
	img->emit(OP_GREATER, addr);  //OP_CMPJG    // if (operand1 > operand2) jump to addr           
	img->emit(OP_HALT);                         // end of program


	img->setEmitAddress(fn);                    // Function fn()
	img->emit(OP_CONST, myStr);                 // Push constant string address
	img->emit(OP_SYSCALL, 0x20);                // Call system call 0x20, to print C style string to standard output
	img->emit(OP_RET);                          // Return
}

void opcodesTest() 
{
	ExecutableImage* img = new ExecutableImage();
	createExecutableImage(img, 10);
	VirtualMachine* vm = new VirtualMachine();
	vm->loadImage(*img); // vm->loadImage(img->getImage());
	cout << "-----------------------------------------------------" << endl;
	cout << "Virtual Machine test:" << endl;
	cout << "-----------------------------------------------------" << endl;
	auto start = std::chrono::high_resolution_clock::now();
	vm->execute(); // run();
	auto end = std::chrono::high_resolution_clock::now();
	auto ms_int = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
	cout << "EXECUTION TIME: " << ms_int / 1000000000.0 << "s" << endl;
	delete vm;
	delete img;
}

int main()
{
	cout << "Hello CMake." << endl;
	opcodesTest();
	return 0;
}
