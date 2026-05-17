%include 'rw32.inc'
section .bss
    pole2 dd 10
section .data
    pole1 dd 10,20,30,40,50,60,70,80,90,100
section .text
main:
    mov ecx, 10
    mov ebx, 0
    mov esi, pole1
    mov edi, pole1
    cld
nasobenie2:
    lodsd
    add eax, eax
    stosd
    loop nasobenie2
    
    ret