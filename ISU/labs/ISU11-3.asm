%include 'rw32.inc'

section .text
main:
    FLDPI
    call ReadFloat
    push EAX
    FLD dword [esp]
    pop EAX

    FMUL st0
    FMUL st0, st1

    call WriteNewLine
    call WriteDouble

    ret