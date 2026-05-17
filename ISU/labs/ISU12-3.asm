%include 'rw32.inc'
section .data
section .text
; kvadraticka (a, b, c, x1, x2)
kvadraticka:
    push ebp
    mov ebp , esp

    FLD dword [ebp+12]
    FMUL dword [ebp+12]
    FLD dword [ebp+8]
    FADD st0, st0
    FADD st0, st0
    FMUL dword [ebp+16]
    FSUBP st1, st0

    FSQRT

    FLD dword [ebp+12]
    FCHS

    FSUB st1
    
    FLD dword [ebp+8]
    FXCH st0, st1
    FDIV st1

    

    pop ebp
    ret 20

main: