%include 'rw32.inc'
section .bss
section .data
    znak db "x",0
    text db "text",0
section .text
najdi_znak:
    push ebp
    mov ebp, esp
    push esi
    push edi
    push ecx

    mov ecx, [ebp+8]
    mov edi, [ebp+16]
    mov al, [ebp+12]

    cld
    repne scasb

    mov eax, [ebp+8]
    sub eax, ecx
    dec eax

    pop ecx
    pop edi
    pop esi
    pop ebp
    ret 12
    
main:
    ; najdi_znak(char* text, char znak, int pocet) 
    push dword text
    push dword [znak]
    push 4
    call najdi_znak

    call WriteInt32
    ret