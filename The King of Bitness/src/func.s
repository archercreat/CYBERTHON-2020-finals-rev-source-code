.intel_syntax noprefix
.text
.code32
.globl _check
_check:
    push        ebp
    mov         ebp, esp
    mov         eax, [ebp + 8]
    call 0x33:secret
    mov         esp, ebp
    pop         ebp
    ret

.code64
secret:
    xor         r8, r8
    xor         r10, r10
strlen:
    mov         r11b, byte [rax + r8 + 0]
    test        r11b, r11b
    je          next_stage
    inc         r8
    jmp         strlen
next_stage:
    xchg        r8, r10
encode:
    mov         r11b, byte [rax + r8 + 0]
    mov         r12b, byte [rax + r8 + 1]
    mov         r9b, r11b
    shl         r9b, 5
    shr         r11b, 3
    or          r11b, r9b
    add         r11b, 0xab
    xor         r11b, 0x69
    not         r11b
    mov         r9b, r12b
    shl         r9b, 7
    shr         r12b, 1
    or          r12b, r9b
    not         r12b
    add         r12b, 0xba
    xor         r12b, 0x69
    xor         r11b, r12b
    mov         byte [rax + r8 + 0], r11b
    inc         r8
    dec         r10
    jne         encode

    sub         rsp, 0x30
    mov         byte ptr  [rsp], 0xE1
    mov         byte ptr  [rsp+0x1], 0xB1
    mov         byte ptr  [rsp+0x2], 0x1B
    mov         byte ptr  [rsp+0x3], 0x3C
    mov         byte ptr  [rsp+0x4], 0x85
    mov         byte ptr  [rsp+0x5], 0x5F
    mov         byte ptr  [rsp+0x6], 0x59
    mov         byte ptr  [rsp+0x7], 0xF9
    mov         byte ptr  [rsp+0x8], 0x77
    mov         byte ptr  [rsp+0x9], 0xC5
    mov         byte ptr  [rsp+0xA], 0x4E
    mov         byte ptr  [rsp+0xB], 0x96
    mov         byte ptr  [rsp+0xC], 0x2D
    mov         byte ptr  [rsp+0xD], 0x51
    mov         byte ptr  [rsp+0xE], 0xCC
    mov         byte ptr  [rsp+0xF], 0xC8
    mov         byte ptr  [rsp+0x10], 0xCE
    mov         byte ptr  [rsp+0x11], 0xC
    mov         byte ptr  [rsp+0x12], 0xF7
    mov         byte ptr  [rsp+0x13], 0x11
    mov         byte ptr  [rsp+0x14], 0x46
    mov         byte ptr  [rsp+0x15], 0x29
    mov         byte ptr  [rsp+0x16], 0x51
    mov         byte ptr  [rsp+0x17], 0xCC
    mov         byte ptr  [rsp+0x18], 0x97
    mov         byte ptr  [rsp+0x19], 0xF3
    mov         byte ptr  [rsp+0x1A], 0xEA
    mov         byte ptr  [rsp+0x1B], 0x44
    mov         byte ptr  [rsp+0x1C], 0x71
    mov         byte ptr  [rsp+0x1D], 0x73
    mov         byte ptr  [rsp+0x1E], 0x5E
    mov         byte ptr  [rsp+0x1F], 0x2A
    mov         byte ptr  [rsp+0x20], 0xEE
    mov         byte ptr  [rsp+0x21], 0x1D
    mov         byte ptr  [rsp+0x22], 0x1C
    xor         r8, r8
check_key:
    mov         r10b, byte ptr  [rax + r8]
    mov         r11b, byte ptr  [rsp + r8]
    cmp         r10b, r11b
    jne         fail
    inc         r8
    cmp         r8, 0x23
    jne         check_key
    xor         rax, rax
    jmp         exit
fail:
    mov         rax, -1
exit:
    add         rsp, 0x30
    retf
