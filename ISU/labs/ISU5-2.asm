%include 'rw32.inc'
section .data
    a db 0x11, 0x22, 0x33, 0x44
section .text
main:
    xor EAX, EAX

    mov AL, [a+3]
    shl EAX, 8

    mov AL, [a+2]
    shl EAX, 8

    mov AL, [a+1]
    shl EAX, 8

    mov AL, [a]

    call WriteHex32
    call WriteNewLine

    xor EAX, EAX

    mov AL, [a]
    shl EAX, 8

    mov AL, [a+1]
    shl EAX, 8

    mov AL, [a+2]
    shl EAX, 8

    mov AL, [a+3]
    call WriteHex32

    ret