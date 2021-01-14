#include <iostream>

/*
* mov edx, Var1
* mov ecx, Var2
* mov eax, edx
* imul ecx
* mov edx, eax
* imul edx, eax
* mov Var3, ecx
*/

/*
* Mnemonic: IMUL r/m32 
* Description: EDX:EAX  EAX * r/m doubleword
* 
* Performs a signed multiplication of two operands. 
* This instruction has three forms, depending on the number of operands.
* 
* One-operand form. This form is identical to that used by the MUL instruction. 
* Here, the source operand (in a general-purpose register or memory location) is 
* multiplied by the value in the AL, AX, or EAX register (depending on the operand size) 
* and the product is stored in the AX, DX:AX, or EDX:EAX registers, respectively.
* 
* Mnemonic: IMUL r32, r/m32
* Description: doubleword register  doubleword register * r/m doubleword
* 
* Two-operand form. With this form the destination operand (the first operand) 
* is multiplied by the source operand (second operand). The destination operand is 
* a general-purpose register and the source operand is an immediate value, 
* a general-purpose register, or a memory location. 
* The product is then stored in the destination operand location.
*/

int main(int argc, char* argv[])
{
	/*
	* mov edx, Var1
	* mov ecx, Var2
	* mov eax, edx
	* imul ecx
	* mov edx, eax -> this is rubish
	* imul edx, eax -> this is rubish
	* mov Var3, ecx
	*/

	int Var1;
	int Var2;

	std::cin >> Var1;
	std::cin >> Var2;

	int Var3 = Var1 * Var2;

	std::cout << "Var3: " << Var3 << "\n";

	return 0;
}