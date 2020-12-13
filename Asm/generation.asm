.586
.MODEL FLAT, STDCALL
	option casemap:none
	include C:\Program Files\masm32\INCLUDE\kernel32.inc
	include C:\Program Files\masm32\INCLUDE\user32.inc
	includelib libucrt.lib
	includelib kernel32.lib
	includelib user32.lib
	ExitProcess PROTO : DWORD
.STACK 4096
.CONST
	num0 DWORD 0
	str0 BYTE "bnv", 0
	num1 DWORD 1
	num2 DWORD 1
	num3 DWORD 1
	num4 DWORD 0
	symb0 BYTE "g", 0
	str1 BYTE "sdfdf", 0
	num5 DWORD 5
	str2 BYTE "Magazine", 0
	num6 DWORD 0
	str3 BYTE "Ошибка!", 0
	num7 DWORD 0
	BSIZE equ 15
.DATA
	1fAi DWORD ?
	mainaA1 DWORD ?
	maini DWORD ?
	mainaA2 DWORD ?
	maincheck DWORD ?
	maintest DWORD ?
	maintre DWORD ?
	ifmt DB "%d", 0
	buf DB BSIZE dup(?)
	stdout DD ?
	cWritten DD ?
.CODE

1fA PROC x : DWORD, y : BYTE
1fA ENDP

main PROC
	invoke GetStdHandle, -11
	mov stdout, EAX
	INVOKE ExitProcess, -1
main ENDP
end main