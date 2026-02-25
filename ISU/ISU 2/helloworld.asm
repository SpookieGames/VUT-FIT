%include "rw32-2022.inc"

section .data
	sMessage db "Hello World!",EOL,0

section .text
CMAIN:
	push ebp
	mov ebp,esp
	
	mov esi,sMessage	; ukazka volani funkce, ktera napise "Hello World!"
	call WriteString

	call ReadInt16
	call WriteInt16
	call WriteNewLine

	mov AL, 42
	mov AH, 1
	call WriteInt16 ;  vzdy register A
	call WriteNewLine
	call WriteUInt16
	call WriteNewLine
	call WriteHex16 ;  vzdy register A
	call WriteNewLine
	call WriteBin16 ;  vzdy register A
	call WriteNewLine

	pop ebp
	ret
