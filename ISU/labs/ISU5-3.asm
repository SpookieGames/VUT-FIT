%include 'rw32.inc'

section .text
main:
    xor EAX, EAX
    xor EBX, EBX
    mov AL, 01101101b
    
    shl AL, 1
    adc BL, 0

    shl AL, 1
    adc BL, 0

    shl AL, 1
    adc BL, 0

    shl AL, 1
    adc BL, 0

    shl AL, 1
    adc BL, 0

    shl AL, 1
    adc BL, 0

    shl AL, 1
    adc BL, 0

    shl AL, 1
    adc BL, 0

    mov AL, BL
    call WriteInt8
    
    ret