%include 'rw32.inc'
section .data
    pole dw 15, 20, 255, 28, 30, 60000, 1, 9, 10, 3
section .text
main:
    mov ecx, 10
    mov ebx, 0
    mov edx, 0

    cmp ecx, 0
    je koniec

cyklus:
    mov ax, [pole+ebx*2]
    add dx,ax
    inc ebx
    loop cyklus

koniec:
    mov eax, edx
    call WriteUInt16
    ret