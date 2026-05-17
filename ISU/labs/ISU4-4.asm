%include 'rw32.inc'
section .data

; BX + (CL * 50 * AH)
section .text
main:
    mov BX, 30
    mov CL, -20
    mov AH, 10

    mov AL, 50
    imul AH
    mov DX, AX
    mov AL, CL
    cbw
    imul DX
    add AX, BX

    call WriteInt16
    
    ret