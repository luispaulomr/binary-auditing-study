#include <iostream>

/*
 *	...
 *	mov dword ptr [esi], 1
 *	xor edx, edx
 *	mov [ebx], edx
 *	jmp short loc_4012F1
 * loc_4012E8:
 *	mov ecx, [esi]
 *	imul ecx, [esi]
 *	mov [esi], ecx
 *	inc dword ptr [ebx]
 * loc_4012F1:
 *	cmp dword ptr [ebx], 8
 *	jl short loc_4012E8
 *	...
*/

int main(int argc, char* argv[])
{
	/*
	* this is ecx^8
	*/

	return 0;
}