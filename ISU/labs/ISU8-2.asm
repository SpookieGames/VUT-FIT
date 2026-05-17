%include 'rw32.inc'

section .text
main:
    mov bx, 0010101110101101b
    mov ax, 0
    mov cx, 16
    mov dx, 1

cyklus:
    test bx, dx
    jz preskoc
    inc ax
    
preskoc:
    shl dx, 1
    cmp dx, 0
    jz koniec
    loop cyklus

koniec:
    call WriteInt16
    ret