/*

sub_408138 proc near
000 push ebx
004 push esi

008 mov esi, edx
008 dec esi
008 test esi, esi
008 jl short loc_40816F -> if (edx - 1 < 0) {
			->	return eax;
008 inc esi		-> }

loc_408142 :
008	xor edx, edx		-> 
008	mov dl, [eax]		-> 
008	xor ebx, ebx		-> 
008	mov bl, cl		-> 
008	add edx, ebx		-> edx = *eax + ecx
008	test edx, edx		-> if (edx >= 0) {
008	jge short loc_40815B	->	goto loc_40815B;
008	mov ebx, 100h		-> } else {
008	sub ebx, edx		->	ebx = 0x100 - edx;
008	jmp short loc_408169	->	edx = ebx;
				->	goto loc_408169;
				-> }

loc_40815B :
008	cmp edx, 100h		-> if (edx <= 0x100) {
008	jle short loc_408169	->	goto loc_408169;
008	sub edx, 100h		-> } else {
				->	edx -= 0x100;
				-> }

loc_408169 :
008	mov [eax], dl		-> *eax = dl
008	inc eax			-> ++eax;
008	dec esi			-> --esi;
008	jnz short loc_408142	-> if (esi != 0) {
				->	goto loc_408142;
				-> } else {
				->	return eax;
				-> }

loc_40816F :
008	pop esi
004	pop ebx
000	retn

sub_408138 endp

parameters:
- edx: length
- eax: char *array (return)
- ecx: int offset

this algorithm iterates through array eax with length edx, and substitutes every element by (element + ecx) % 256

*/