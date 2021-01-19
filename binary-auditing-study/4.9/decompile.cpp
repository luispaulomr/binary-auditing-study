/*

procedure0 proc near
000 push 0
004 push procedure1
008 call EnumWindows				-> EnumWindows(procedure1, 0)
000 retn
procedure0 endp

------

procedure1 proc near
ClassName = byte ptr - 204h
String = byte ptr - 104h
hwnd = dword ptr - 4
V1 = dword ptr 8

000	push ebp				-> 
004	mov ebp, esp				-> 
004	add esp, -204h
208	push ebx
20C	push esi
210	push edi

214	mov edi, [ebp + V1]			-> 
214	push 100h				-> 
218	lea eax, [ebp + String]			-> 
218	push eax				-> 
21C	push edi				-> 
220	call GetWindowTextA			-> GetWindowTextA(V1, &String, 0x100) (GetWindowTextA(HWND, LPSTR, nMaxCount))
214	mov [ebp + eax + String], 0		-> String[eax] = 0; // inserting terminator character
214	push 100h			
218	lea eax, [ebp + ClassName]
218	push eax
21C	push edi
220	call GetClassNameA			-> GetClassNameA(V1, &ClassName, 0x100) (GetClassNameA(HWND, LPSTR, nMaxCount))
214	mov [ebp + eax + ClassName], 0		-> ClassName[eax] = 0; // inserting terminator character
214	mov esi, 3				-> esi = 3
214	mov ebx, offset Address1		-> ebx = Address1

Label1 :
214	lea eax, [ebp + ClassName]		-> 
214	call tolower				-> tolower(&ClassName)
214	mov edx, [ebx]				-> edx = *ebx
214	call Sysutils::StrPos(char*, char*)	-> if (strpos(?, ?) != NULL) {
214	test eax, eax				->	goto Label2;
214	jnz short Label2			-> } else {
214	lea eax, [ebp + String]			->	tolower(&String)
214	call tolower				->	edx = *ebx
214	mov edx, [ebx]				->	if (strpos(?, ?) == NULL) {
214	call Sysutils::StrPos(char*, char*)	->		goto Label3;
214	test eax, eax				->	} else {
214	jz short Label3				->		

Label2:
214	push 0
218	push offset aSyslistview32
21C	push 0
220	push edi
224	call FindWindowExA			-> hwnd = FindWindowExA(V1, 0, "SysListView32", 0); (FindWindowEx(HWND, HWND, LPCSTR, LPCTS))
214	mov [ebp+hwnd], eax
214	push 0
218	push 0
21C	push 1009h
220	mov eax, [ebp+hwnd]
220	push eax
224	call SendMessageA			-> SendMessageA(hwnd, 0x1009, 0, 0) (SendMessageA(HWND, UINT, WPARAM, LPARAM))
214	push 0
218	push 0
21C	push 0Fh
220	mov eax, [ebp+hwnd]
220	push eax
224	call SendMessageA			-> SendMessageA(hwnd, 0xf, 0, 0)
214	push 0
218	push 0
21C	push 02h
220	push edi
224	call SendMessageA			-> SendMessageA(V1, 0x2, 0, 0)
214	push 0
218	push 0
21C	push 10h
220	push edi
224	call SendMessageA			-> SendMessageA(V1, 0x10, 0, 0)

Label3:
214	add ebx, 4				-> ebx += 4
214	dec esi					-> --esi
214	jnz short Label1			-> if (esi != 0) {
214	mov al, 1				->	goto Label1;
214	pop edi					-> } else {
210	pop esi					->	return 1;
20C	pop ebx					-> }
208	mov esp, ebp
004	pop ebp
000	retn 8

procedure1 endp
aSyslistview32 db 'SysListView32'

Address1:
	dd offset aRegmon ; "REGMON"
	dd offset aFilemon ; "FILEMON"
	dd offset aRegmonex ; "REGMONEX"



	The program first calls EnumWindows(procedure1, 0), which will list all windows opened, and call procedure1 passing their handles.
	procedure1 calls GetWindowTextA and GetClassNameA, and searches this strings for strings "REGMON", "FILEMON", "REGMONEX".
	If they find it, then they call:
	
	- SendMessageA(hwnd, LVM_DELETEALLITEMS, 0, 0)	-> 
	- SendMessageA(hwnd, WM_PAINT, 0, 0)
	- SendMessageA(V1, WM_DESTROY, 0, 0)		-> try to close application?
	- SendMessageA(V1, WM_ENABLE, 0, 0)		->
*/

#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <iostream>
#include <string>

#define LVM_DELETEALLITEMS 0x1009

BOOL CALLBACK procedure1(HWND hWnd, long lParam)
{
	char buff[255] = { 0 };

	if (IsWindowVisible(hWnd)) {
		printf("WINDOW\n");
		int n = GetWindowTextA(hWnd, (LPSTR) buff, 254);
		buff[n] = '\0';
		std::string to_search_1(buff);

		if (to_search_1.find("Release") != std::string::npos) {
			std::cout << "LUL\n";
			printf("%s\n", buff);
			printf("Sending messages\n");
			HWND new_hwnd = FindWindowExA(hWnd, 0, "SysListView32", 0);

			printf("%d\n", new_hwnd);

			SendMessageA(new_hwnd, LVM_DELETEALLITEMS, 0, 0);
			SendMessageA(new_hwnd, WM_PAINT, 0, 0);
			SendMessageA(hWnd, WM_DESTROY, 0, 0);
			SendMessageA(hWnd, WM_ENABLE, 0, 0);
			printf("Messages sent\n");
		}

		//printf("%s\n", buff);
		//n = GetClassNameA(hWnd, (LPSTR)buff, 254);
		//buff[n] = '\0';
		//printf("%s\n", buff);

		//std::string to_search_2(buff);

		//if (to_search_2.find("Calculadora") != std::string::npos) {
		//	std::cout << "LUL\n";
		//	//SendMessageA(hWnd, LVM_DELETEALLITEMS, 0, 0);
		//	//SendMessageA(hWnd, WM_PAINT, 0, 0);
		//	SendMessageA(hWnd, WM_DESTROY, 0, 0);
		//	SendMessageA(hWnd, WM_ENABLE, 0, 0);
		//}
	}

	return TRUE;
}

int main()
{
	//int i = 0;
	//char str[] = "Test String.\n";
	//char c;
	//while (str[i]) {
	//	c = str[i];
	//	putchar(tolower(c));
	//	i++;
	//}
	//return 0;

	EnumWindows(procedure1, 0);
}