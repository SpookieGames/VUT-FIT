%include 'rw32.inc'
section .data
    x dw 40000
section .text
main:
    mov EAX, 0
    mov EDX, 0
    mov AX, [x]
    mov BX, 400
    mov DX, 0
    div BX

    call WriteInt16
    ret