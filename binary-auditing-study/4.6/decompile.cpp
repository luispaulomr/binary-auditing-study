/*

proc near
V1 = dword ptr 8
V2 = dword ptr 0Ch
V3 = dword ptr 10h
V4 = dword ptr 14h

push ebp
mov ebp, esp

push esi
push edi

mov edi, [ebp + V4]		-> int length;
mov esi, [ebp + V3]		-> char V3; (terminator character)
mov edx, [ebp + V2]		-> char *V2;
mov eax, [ebp + V1]		-> char *V1;

test edi, edi			-> if (V4 != 0) {
				->	goto loc_40122C;
jnz short loc_40122C		-> } else {
xor eax, eax			->	return NULL; }
jmp short loc_401237

loc_401219 :
	mov ecx, esi		-> 
	cmp cl, [edx]		-> if (V2[V2_index] == V3) {
	jz short loc_401227	->	goto loc_401227;
	mov cl, [edx]		-> } else {
	mov [eax], cl		->	V1[V1_index] = V2[V2_index];
	inc edx			->	V2_index++;
	inc eax			->	V1_index++;
	jmp short loc_40122C	->	goto loc_40122C; }

loc_401227 :
	mov [eax], cl		-> V1[V1_index] = V3
	inc eax			-> V1_index++;
	jmp short loc_401237	-> return V1;

loc_40122C :
	mov ecx, edi		-> 
	add edi, -1		-> if (V4-- != 0) {
	test ecx, ecx		->	goto loc_401219;
	jnz short loc_401219	-> } else {
	xor eax, eax		->	return NULL; }

loc_401237 :
	pop edi
	pop esi
	pop ebp
	retn
	endp


	this is strncpy(V1, V2, V4), which ends when V4 is reached or *V2 == V3;

*/