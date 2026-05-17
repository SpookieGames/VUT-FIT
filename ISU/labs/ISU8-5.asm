%include 'rw32.inc'
section .data
    text resb 20
section .text
main:
    mov ecx, 19
    mov edi, text
    mov ebx, 19
    call ReadString

    mov esi, text

cyklus:
    cmp al, 'A'
    jb male

male:
    cmp al, 'a'
    jb cyklus
    jmp cyklus

zmen_pismeno:
    xor AL, 32
    mov [esi], al
    ret