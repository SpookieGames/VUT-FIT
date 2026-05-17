%include 'rw32.inc'

section .text
main:
    mov EAX, 0x7F800000

    push dword 100
    FILD dword [esp]
    add ebp, 4

    push dword __float32__(75.5)
    fld dword [esp]
    add esp, 4

    fcom st0, st1
    fstsw ax
    sahf

    ja vacsi
    jb mensi

vacsi:
    mov ax, 0
mensi:
    mov ax, 1


    ret