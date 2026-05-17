; 5/6b

task11:
    xchg AH, AL
    rol EAX, 8
    ret

task12:
    xor EAX, EAX
    xor EBX, EBX
    xor ECX, ECX
    xor EDX, EDX
    xor ESI, ESI

    movsx EAX, word [d]
    mov EBX, 12
    imul EBX
    
    movsx ECX, word [e]
    add EAX, ECX
    ;adc EDX, 0
    add EAX, -1060
    ;adc EDX, 0
    mov dword [r], EDX
    mov dword [q], EAX

    xchg EAX, ESI
    add AL, [a]
    movsx EAX, AL
    mov EBX, [b]
    imul EBX ; EDX:EAX
    ;adc EDX, dword [r]

    xor ESI, ESI
    movsx ECX, word [c]
    add EAX, ECX
    ;adc EDX, 0
    add EAX, -31
    cdq
    idiv dword [q]
    
    mov dword [r], EDX
    mov dword [q], EAX

    ret
