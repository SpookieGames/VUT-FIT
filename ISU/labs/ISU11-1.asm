%include 'rw32.inc'
section .data
    i dd 5 ;32b = int = dword
    f dd 5.0 ;32b = float = dword
    c dq 6.00 ;64b = double = qword
    d dt 17.0 ;80b = fpu reg
section .text
main:
    mov eax, 5
    mov ebx, [f]
fst st0
    FLD1
    FLDZ
    FLDPI

    FLD dword [f]
    FILD dword [i] ; int load
    FLD qword [c] 
    FLD tword [d]

    FSTP tword [d] ; store & pop
    FSTP tword [d]

    FISTP qword [c]

    FADD st0, st3 ; dest = dest + source => st0 = st0 + st3
    FADD st2, st0 ; st2 = st2 + st0
    FADD st2      ; st0 = st0 + st2
    FADD          ; st1 = st0 + st1, + pop
    FADDP ; = FADD

    FADDP st1, st0

    FST st3 ; st0 do st3
    
    FLD dword [f]
    FILD dword [i] ; int load
    FLD qword [c] 
    FLD tword [d]

    ;FSTP tword [d] ; store & pop
    ;FSTP tword [d]

    FSUB st0, st4  ; st0 = st0 - st4
    FSUB st4       ; st0 = st0 - st4
    FSUBR st0, st4 ; dest = src - dest, st0 = st4 - st0
    FSUBP st4      ; st4 = st4 - st0, + POP
    FSUBRP st4     ; st4 = st0 - st4, + POP
    FSUB           ; st1 = st1 - st0, + POP
    FSUBR          ; st1 = st0 - st1, + POP

    FXCH st3       ; st0 a st3
    FXCH           ; st0 a st1
    FXCH st0, st2  ; st0 a st2

    FSQRT          ; sqrt(st0)
    FSIN 
    FCOS
    FCHS           ; change sign
    FABS           ; absolute

    call ReadFloat  ; EAX

    push EAX
    FLD dword [esp]
    pop EAX

    call ReadDouble ; FLD cisla do st0 
    call WriteFloat ; EAX
    call WriteNewLine
    call WriteDouble; st0

    ret