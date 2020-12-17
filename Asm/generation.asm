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
	str0 BYTE "InPrint", 0
	num1 DWORD 1
	num2 DWORD 0
	str1 BYTE "cycle", 0
	num3 DWORD 3
	num4 DWORD 4
	num5 DWORD 0
	num6 DWORD 2
	num7 DWORD 5
	str2 BYTE "Result:", 0
	num8 DWORD 0
	BSIZE equ 15
.DATA
	printi DWORD ?
	makeCoefresult DWORD ?
	makeComputeresult DWORD ?
	mainstr DWORD ?
	mainresult DWORD ?
	mainx DWORD ?
	mainy DWORD ?
	ifmt DB "%d", 0
	buf DB BSIZE dup(?)
	stdout DD ?
	cWritten DD ?
	stdin DWORD ?
	cRead DD ?
.CODE

print PROC printstr : DWORD, printcount : DWORD
	push num0
	pop printi
	mov ESI, offset str0
	invoke WriteConsoleA, stdout, ESI, 7, ADDR cWritten, 0
	CYCLE:
	mov EAX, printi
	cmp EAX, printcount
	jb cycle_body
	cycle_end:
		jmp CYCLEend
	cycle_body:
	mov ESI, printstr
	invoke WriteConsoleA, stdout, ESI, 6 , ADDR cWritten, 0
	push printi
	push num1
	pop EAX
	pop EBX
	add EAX, EBX
	push EAX
	pop printi
	jmp CYCLE
	CYCLEend:
	push printi
	pop EAX
	ret
print ENDP

makeCoef PROC makeCoefx : DWORD, makeCoefy : DWORD
	push makeCoefx
	push makeCoefy
	pop EAX
	pop EBX
	add EAX, EBX
	push EAX
	pop makeCoefresult
	push makeCoefresult
	pop EAX
	ret
makeCoef ENDP

makeCompute PROC makeComputex : DWORD, makeComputey : DWORD, makeComputez : DWORD
	push num2
	pop makeComputeresult
	push makeComputex
	push makeComputey
	pop EAX
	pop EBX
	add EAX, EBX
	push EAX
	push makeComputez
	pop EAX
	pop EBX
	add EAX, EBX
	push EAX
	push makeComputez
	mov EDX, 0
	pop EBX
	pop EAX
	div EBX
	push EAX
	push makeComputex
	push makeComputey
	pop edx
	pop edx
	push makeComputey
	push makeComputex
	call makeCoef
	push EAX
	pop EAX
	pop EBX
	mul EBX
	push EAX
	pop makeComputeresult
	push makeComputeresult
	pop EAX
	ret
makeCompute ENDP

main PROC
	invoke GetStdHandle, -11
	mov stdout, EAX
	invoke GetStdHandle, -10
	mov stdin, EAX
	push offset str1
	pop mainstr
	push num3
	pop mainx
	push num4
	pop mainy
	push num5
	pop mainresult
	push mainx
	push mainy
	push num6
	pop edx
	pop edx
	pop edx
	push num6
	push mainy
	push mainx
	call makeCompute
	push EAX
	pop mainresult
	push mainstr
	push num7
	pop edx
	pop edx
	push num7
	push mainstr
	call print
	push EAX
	pop mainx
	mov ESI, offset str2
	invoke WriteConsoleA, stdout, ESI, 7, ADDR cWritten, 0
	invoke wsprintf, ADDR buf, ADDR ifmt, mainresult
	invoke WriteConsoleA, stdout, ADDR buf, BSIZE, ADDR cWritten, 0
	INVOKE ReadConsole, stdin, ADDR buf, 20, ADDR cRead, 0
	INVOKE ExitProcess, -1
main ENDP
end main