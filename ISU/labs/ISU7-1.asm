%include 'rw32.inc'
section .data
    a db 5
    b dd 1000000
section .text
soucet: ;EAX=EBX+ECX
    push EBX
    push ECX

    add EBX, ECX
    mov EAX, EBX

    pop ECX
    pop EBX
    ret

soucet2:
    push ebp
    mov ebp, esp
    sub esp, 16 ; vytvorenie 3 (*4=12) lokalnych premennych

    mov eax, 0
    mov al, [ebp+8]
    mov ebx, [ebp+12]
    add EAX, EBX

    mov [esp-4], eax ;lokalna premenna

    mov esp, ebp
    pop ebp
    ret 8;posun EBP (2*4)=8B
main:
    push dword [b]
    push dword [a]

    call soucet2
    
    ret