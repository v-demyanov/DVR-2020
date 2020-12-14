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
	str0 BYTE "Result:", 0
	num1 DWORD 2
	num2 DWORD 4
	num3 DWORD 4
	num4 DWORD 0
	BSIZE equ 15
.DATA
	furesult DWORD ?
	mainfuresult DWORD ?
	maink DWORD ?
	maina DWORD ?
	mainb DWORD ?
	mainc DWORD ?
	mainresult DWORD ?
	ifmt DB "%d", 0
	buf DB BSIZE dup(?)
	stdout DD ?
	cWritten DD ?
	stdin DWORD ?
	cRead DD ?
.CODE

fu PROC fux : DWORD, fuy : DWORD
	push fux
	push fuy
	pop EAX
	pop EBX
	add EAX, EBX
	push EAX
	pop furesult
	push 0
	ret

fu ENDP

main PROC
	invoke GetStdHandle, -11
	mov stdout, EAX
	invoke GetStdHandle, -10
	mov stdin, EAX
	push offset str0
	pop mainc
	push num1
	pop maina
	push num2
	push maina
	mov EDX, 0
	pop EBX
	pop EAX
	div EBX
	push EAX
	pop maink
	push num3
	pop mainb
	push maina
	push mainb
	pop mainfuresult
	invoke wsprintf, ADDR buf, ADDR ifmt, mainfuresult
	invoke WriteConsoleA, stdout, ADDR buf, BSIZE, ADDR cWritten, 0
	push maina
	push mainb
	pop EAX
	pop EBX
	add EAX, EBX
	push EAX
	push maink
	pop EAX
	pop EBX
	mul EBX
	push EAX
	push maina
	mov EDX, 0
	pop EBX
	pop EAX
	div EBX
	push EAX
	push mainb
	push maink
	pop EAX
	pop EBX
	mul EBX
	push EAX
	pop EAX
	pop EBX
	add EAX, EBX
	push EAX
	push maink
	mov EDX, 0
	pop EBX
	pop EAX
	div EBX
	push EAX
	pop mainresult
	mov ESI, offset mainc
	invoke WriteConsoleA, stdout, ESI, lengthof mainc, ADDR cWritten, 0
	invoke wsprintf, ADDR buf, ADDR ifmt, mainresult
	invoke WriteConsoleA, stdout, ADDR buf, BSIZE, ADDR cWritten, 0
	INVOKE ReadConsole, stdin, ADDR buf, 20, ADDR cRead, 0
	INVOKE ExitProcess, -1
main ENDP
end main