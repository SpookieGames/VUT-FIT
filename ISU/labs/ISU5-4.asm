%include 'rw32.inc'
section .data
    d dd 1
section .text
main:
    mov CX, 30000
    mov BX, 30000
    xor EAX, EAX
    xor EDX, EDX

    mov AX, CX
    imul BX ; EDX:EAX

    shl EDX, 16
    or EAX, EDX

    call WriteInt32 
    call WriteNewLine

    cdq
    mov ECX, [d]
    idiv ECX

    call WriteInt32

    ret