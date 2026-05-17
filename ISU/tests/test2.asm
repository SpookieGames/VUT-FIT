; 8/9b

task21:
    mov ESI, EAX
    mov EDI, EBX
    
    cld
    rep movsd
    ret

task22:
    push ebp
    mov ebp,esp

    push ecx
    push EBX
    push edx


    mov eax, [ebp+8]
    mov ecx, [ebp+12]
    ;mov edx, [ebp+16]
    cmp eax, 0
    je chyba
    cmp ecx, 0
    jle chyba

    mov ebx, 0
    for:
        mov edx, [eax+ebx*4]
        cmp edx, [ebp+16]
        je vysledok
        inc ebx
    loop for
    jne chyba

    vysledok:
        mov eax, ebx
        jmp koniec

    chyba:
        mov eax, -1
        jmp koniec

    koniec:
    ;pop esi
    ;pop edi
    pop edx
    pop EBX
    pop ecx

    pop ebp
    ret 12

task23:
EXTERN malloc
    push ebp
    mov ebp, esp

    push ecx
    push edx
    push ebx

    cmp ecx, 0
    jle chyba

    mov eax, ecx
    mov ebx, 4
    mul ebx

    push ecx
    push eax
    call malloc
    add esp, 4
    cmp eax, 0
    je chyba
    pop ecx
    
    mov [eax], dword 0
    mov [eax+4], dword 1
    mov ebx, 2

    while:
        mov edx, [eax+(ebx-1)*4]
        add edx, [eax+(ebx-2)*4]
        mov [eax+ebx*4], edx
        inc EBX

    cmp ebx, ecx
    jl while

    jmp koniec

    chyba:
        mov eax, 0
        jmp koniec

    koniec:
        pop ebx
        pop edx
        pop ecx

        pop ebp
        ret
