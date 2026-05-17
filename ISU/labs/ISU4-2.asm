%include 'rw32.inc'
section .data
    x dq 0x00112345FFFFFFFF
    y dq 0x0000000000777777
    z db 50
    c dw -30000
section .text
main:
    mov AL, [z]
    mov BL , 10

    mul BL ; vzdy iba register, ziadne cislo alebo [] ;; DX:AX=AL*BL 

    call WriteInt16
    call WriteNewLine

    mov AX, [c]
    cwde
    mov EBX, 10

    imul EBX
    call WriteInt32

    


    ret