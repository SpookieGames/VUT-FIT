; 6/8b

task31:
    push ebp
    mov ebp, esp
    push eax
    push ebx
    push ecx
    push edx

    fld dword [ebp+8]
    fld dword [ebp+12]

    fcomp st0, st1
    fstsw ax
    sahf
    jb ret_qnan

    fld dword [ebp+16]

    ftst
    fstsw ax
    sahf
    je ret_qnan

    fldz
    fld dword [ebp+20]
    
    fcompp
    fstsw ax
    sahf
    jbe ret_qnan

    fstp st0
    fstp st0
    mov ecx, [ebp+20]

    fld dword [ebp+8]
    fld dword [ebp+12]
    fld dword [ebp+20]
    ; dx = (b-a)/N
    fld dword [ebp+12]
    fsub dword [ebp+8]
    fdiv dword [ebp+20]

    ; for(int i = 0; i < N; i++) sum += f(a + i*dx), if f(x) == NaN => return QNaN
    fldz
for: 
    
    
    cmp ecx, 0
    dec ecx
    ja for
    ;jmp end
    
ret_qnan:
    push dword 0x7FC00000
    fld dword [esp]
    add esp,4
end:
    fstp st1
    fstp st1
    fstp st1
    fstp st1
    fstp st1
    fstp st1

    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret

task32:
    push ebp
    mov ebp, esp

    fldpi
    fmul dword[ebp+12]
    fsubr dword [ebp+8]
    fsin

    push __float32__(5.25)
    fld dword [esp]
    add esp, 4
    fadd dword[ebp+8]
    fsqrt

    fmulp

    push dword 2
    fild dword[esp]
    add esp, 4
    fmul dword [ebp+12]
    fsubr dword [ebp+8]
    fabs

    fdivp

    pop ebp
    ret
