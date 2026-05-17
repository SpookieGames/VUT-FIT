%include 'rw32.inc'
section .bss
    pole2 dw 4
    pole3 dw 4
section .data
    pole1 dw 10,20,30,64
    vzor db "Cislo %d je mensi nez %d.",EOL,0
section .text
kopiruj_pole: ; void kupiruj_pole(word *p1, word *p2, int pocet)
    push esp
    mov ebp, esp
    push esi
    push edi
    push edx

    mov ecx, [ebp+16]
    mov esi, [ebp+8]
    add esi, 6
    mov edi, [ebp+12]
    add edi, 6
    std
    rep movsw 

    pop edx
    pop edi
    pop esi
    pop ebp
    ret 12

main:

    ; void kupiruj_pole(word *p1, word *p2, int pocet)
    push 4
    push pole2
    push pole1
    call kopiruj_pole

EXTERN printf
    ; int printf(char *vzor,...)
    ; printf("Cislo %d je mensi nez %d.\n", x, y)
    push 20
    push 60
    push vzor
    call printf
    add esp, 12

    ; void* malloc(int size)
    ; ukazatel = malloc(8)

EXTERN malloc
    push 8
    call malloc
    add esp, 4
    cmp eax, 0
    ; je koniec
    push 4
    push eax
    push pole1
    call kopiruj_pole
    add esp, 12


    mov ecx, 4
    mov ebx, 0
    mov esi, pole1
    add esi, 6
    mov edi, pole2
    add edi, 6
    std ;zhodra nadol
    ;cld  ; zdola nahoru

;loop_pole1:
    ;mov ax, [esi]
    ;mov [edi], ax
    ;add esi, 2
    ;add edi, 2
    rep movsw ; opakuje kym ecx > 0, znizuje ecx
    
    ;loop loop_pole1

    mov ecx, 4
    mov esi, pole1
    mov edi, pole3
    cld

loop_pole3:
    lodsw ; z esi nacita ax a posunie ukazatel
    add ax, ax
    stosw ; do edi ulozi ax a posunie ukazatel

    loop loop_pole3

    mov esi, pole1
    mov edi, pole2
    mov ecx, 4

    repne cmpsw 
    sub esi, 2
    sub edi, 2

    mov ax, 10
    mov edi, pole2
    mov ecx, 4
    rep scasw ; posunie EDI a znici ecx, pri najdeni zhody je index (povodny - ecx)-1
    
    ret