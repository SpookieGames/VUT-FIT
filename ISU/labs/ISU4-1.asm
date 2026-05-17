%include 'rw32.inc'
section .data
    x dq 0x00112345FFFFFFFF
    y dq 0x0000000000777777
    z db 50
section .text
main:
    mov AL, 0
    add AL, [z]
    add [z], byte 20

    mov EAX, [x]
    mov EBX, [x+4]
    mov ECX, [y]
    mov EDX, [y+4]
    
    add EAX, ECX
    adc EBX, EDX

    xchg EAX, EBX
    call WriteHex32
    xchg EAX, EBX
    call WriteHex32


    ret