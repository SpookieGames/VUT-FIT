%include 'rw32.inc'
section .data
    t_cislo db "Cislo ",0
    t_jenula db " je nula",0
    t_kladne db " je kladne",0
    t_zaporne db " je zaporne",0
section .text
main:
    mov bx, -25
    cmp bx, 0
    je nula
    jg kladne
    jl zaporne

nula:
    mov esi, t_cislo
    call WriteStringASCIIZ
    mov ax, bx
    call WriteInt16
    mov esi, t_jenula
    call WriteStringASCIIZ
    jmp koniec

kladne:
    mov esi, t_cislo
    call WriteStringASCIIZ
    mov ax, bx
    call WriteInt16
    mov esi, t_kladne
    call WriteStringASCIIZ
    jmp koniec

zaporne:
    mov esi, t_cislo
    call WriteStringASCIIZ
    mov ax, bx
    call WriteInt16
    mov esi, t_zaporne
    call WriteStringASCIIZ
    jmp koniec

koniec:
    ret