%include 'rw32.inc'
section .data
    pole dd 12, 56, 84, 21, 33
section .text
my_comp:
    mov eax, dword [esp+4]  ; Pointer to first element 'a'
    mov ecx, dword [esp+8]  ; Pointer to second element 'b' (Fixed from [esp])

    mov edx, [eax]          ; Dereference 'a' (load 32-bit value into edx)
    cmp edx, [ecx]          ; Compare 'a' with the value at pointer 'b
    ja vetsi
    jb mensi
    mov eax, 0
    ret
vetsi:
    mov eax, 1
    ret
mensi:
    mov eax, -1
    ret
main:
    EXTERN qsort
    push my_comp
    push 4
    push 5
    push pole
    call qsort
    add esp, 16

    ret