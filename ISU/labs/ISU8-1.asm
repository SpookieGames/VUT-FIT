%include 'rw32.inc'
section .data
    pole db 5,3,4,1,2
    t_jerovno db "cislo je rovne: ",EOL, 0
    text resb 20 
section .text
main:
    mov ecx, 5
    mov ebx, 0
    cmp ecx, 0  ; osetrenie

    mov edi, text
    mov ebx, 19
    call ReadString

    mov esi, t_jerovno
    call WriteStringASCIIZ

    je koniec   ; osetrenie

cyklus1:
    mov al, [pole+ebx]
    add al,al
    mov [pole+ebx], al
    inc ebx
    ;dec ecx
    ;JECXZ koniec
    ;jmp cyklus1
    loop cyklus1

koniec:
    ret