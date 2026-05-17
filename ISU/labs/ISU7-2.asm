%include 'rw32.inc'
section .data
    a dw 5,10,15
section .text
sucet:
    push ebp
    mov ebp, esp

    mov EAX, [EBP+8]
    add EAX, [ebp+12]
    add eax, [ebp+16]

    pop ebp
    ret 12
main:
    xor EBX, EBX
    xor ECX, EBX
    xor EAX, EAX

    movsx EAX, byte [a+2]
    movsx EBX, byte  [a+1]
    movsx ECX, byte [a]
    
    push eax
    push ebx
    push ecx

    call sucet;

    ret