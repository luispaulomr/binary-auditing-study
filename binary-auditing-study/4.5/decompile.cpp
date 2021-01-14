/*


proc near

arg_0 = dword ptr 8		-> define arg_0 offset (pointer to byte array) (char *arg_0)
arg_4 = dword ptr 0Ch		-> define arg_4 offset (pointer to byte array) (char *arg_4)
arg_8 = dword ptr 10h		-> define arg_8 offset (length)

 push ebp			-> setup stack frame
 mov ebp, esp			-> setup stack frame

 push ebx			-> save ebx
 push esi			-> sabe esi
 mov ecx, [ebp+arg_8]		-> take arg_8
 mov esi, [ebp+arg_0]		-> take arg_0
 mov eax, [ebp+arg_4]		-> take arg_4
 mov edx, esi			-> duplicate arg_0
 test ecx, ecx			-> check arg_8
 jnz short loc_40125A		-> if not zero, jump to loc_40125A
 xor eax, eax			-> else, clear eax
 jmp short loc_401265		-> jump to loc_401265
loc_401254:
 mov bl, [eax]			->
 inc eax			-> arg_0[arg_0_index] = arg_4[arg_4_index]
 mov [edx], bl			-> arg_4_index++
 inc edx			-> arg_0_index++
loc_40125A:
 mov ebx, ecx			-> 
 add ecx, -1			-> 
 test ebx, ebx			-> if (arg8-- != 0)
 jnz short loc_401254		-> jump loc_401254
 mov eax, esi			-> else, return arg_0
loc_401265:
 pop esi
 pop ebx
 pop ebp
 retn
endp


this is strncpy(arg_0, arg_4, arg_8)


*/