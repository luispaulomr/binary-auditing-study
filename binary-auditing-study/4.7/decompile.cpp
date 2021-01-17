/*

proc near

000 push ebx
004 push esi

Parameters:

	- edx: char *
	- ecx: int > 0 (num of operations)
	- eax: int *

008 xor ebx, ebx
008 mov [eax], ebx		-> *eax = 0
008 mov ebx, ecx		-> ebx = ecx
008 dec ebx			-> --ebx
008 test ebx, ebx		-> if (ebx < 0) {
008 jl short loc_408135		->	return eax;
008 inc ebx			-> } else { ++ebx; }

loc_40810E :
008 mov ecx, [eax]		-> ecx = *eax
008 shl ecx, 4			-> ecx = ecx  << 4
008 movzx esi, byte ptr[edx]	-> esi = *edx
008 add ecx, esi		-> ecx += esi
008 mov [eax], ecx		-> *eax = ecx
008 mov ecx, [eax]		-> ecx = *eax
008 and ecx, 0F0000000h		-> ecx &= 0xf0000000
008 test ecx, ecx		-> if (ecx == 0) {
008 jz short loc_40812D		->	goto loc_40812D;
008 mov esi, ecx		-> } else {
008 shr esi, 18h		->	esi = esi >> 0x18
008 xor [eax], esi		->	*eax = *eax ^ esi }

ecx = *eax << 4 + edx[edx_index];
*eax = ecx;
ecx &= 0xf0000000;
if (ecx != 0) {
	goto loc_40812D;
} else {
	*eax ^= (edx[edx_index] >> 24);
	ecx = ~ecx;
	*eax = *eax & ecx;
	++edx_index;
	--ebx;
}

loc_40812D :
008 not ecx			-> ecx = ~ecx
008 and [eax], ecx		-> *eax = *eax & ecx
008 inc edx			-> ++edx
008 dec ebx			-> --ebx
008 jnz short loc_40810E	-> if (ebx != 0) { goto loc_40810E } else { return eax; }

loc_408135 :
008 pop esi
004 pop ebx
000 retn
sub_408100 endp


looks like eax is a pointer to an int, which gets modified during this function, and *eax is the returned value;
looks like edx is a char array, could be a polynomial array.
looks like ecx is length of edx array.
i guess it's a CRC algorithm, or something like that.
*/