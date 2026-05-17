%include 'rw32.inc'
section .data
    tx_mensi db "Hodnota je mensi.", EOL, 0
    tx_vacsi db "Hodnota je vacsi.", EOL, 0
    tx_nula db "Hodnota je 0.", EOL, 0 
    tx_rovna db "Hodnota je rovna.", EOL, 0 
section .text
main:
    FLDZ
    call ReadDouble
    call WriteNewLine
    fcom st0, st1
    fstsw ax
    sahf
                    
    je nula
    ja vacsi
    jb mensi

nula:
    mov esi, tx_nula
    call WriteStringASCIIZ
    jmp b
vacsi:
    mov esi, tx_vacsi
    call WriteStringASCIIZ
    jmp b
mensi:
    mov esi, tx_mensi
    call WriteStringASCIIZ

b:
    call ReadDouble
    call WriteNewLine
    fcom st0, st1
    fstsw ax
    sahf

    je rovna
    ja vacsi_2
    jb mensi_2

rovna:
    mov esi, tx_rovna
    call WriteStringASCIIZ
    jmp koniec
vacsi_2:
    mov esi, tx_vacsi
    call WriteStringASCIIZ
    jmp koniec
mensi_2:
    mov esi, tx_mensi
    call WriteStringASCIIZ
koniec:
    ret