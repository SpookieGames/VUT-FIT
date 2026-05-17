%include 'rw32.inc'

section .text
main:
    mov AX, -10
    movsx EBX, AX

    mov EAX, 10
    push EAX
    FILD dword [esp]
    pop EAX

    push EBX
    FILD dword [esp]
    pop EBX

    call ReadFloat
    push EAX
    FLD dword [esp]
    pop EAX

    FXCH st0, st2
    FSUBP st2
    FMULP st1

    call WriteNewLine
    call WriteDouble

    ret