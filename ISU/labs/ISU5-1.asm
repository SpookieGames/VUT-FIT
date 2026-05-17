%include 'rw32.inc'

section .text
main:
    mov AL, -20

    shr AL, 1
    shl AL, 1
    ret