; tiny.asm
  
BITS 32

org         0x00010000
input:
  db        0x7F, "ELF"             ; e_ident
  dd        1                                       ; p_type
  dd        0                                       ; p_offset
  dd        $$                                      ; p_vaddr 
  dw        2                       ; e_type        ; p_paddr
  dw        3                       ; e_machine
  dd        _start                  ; e_version     ; p_filesz
  dd        _start                  ; e_entry       ; p_memsz
  dd        4                       ; e_phoff       ; p_flags
_start:
                                  ; all regs are 0 except esp
                                  ; so we don't have to reset them
  add       edx, 0x30               ; 3 bytes    
  sub       esp, 0x30               ; 3 bytes
  jmp       _start_2                ; 2 bytes

  dw        0x34                    ; e_ehsize        must be correct
  dw        0x20                    ; e_phentsize     must be correct
  dw        1                       ; e_phnum         must be correct
_start_2:
  lea       ecx, [esp]              ; e_shentsize     useless
  add       eax, 3                  ; e_shnum         useless
                                    ; e_shstrndx      useless
                                    ; overwrite with 6 bytes  
  int       0x80
  
  mov       edi, esp
  mov       esi, esp
  dec       eax                     ; can be deleted but too misleading, 1 byte
  cmp       al, 35
  jne       fail
  
  push      esp                   ; save ptr to string
  mov       ecx, eax              ; save strlen
  push      0x1e                  ; var_1
  push      0x1e                  ; var_2
  push      0x1f                  ; var_3

encode:
  lodsb
  add       al, [esp]
  xor       al, [esp + 4]
  sub       al, [esp + 8]
  stosb

shuffle:
  mov       eax, [esp]
  mov       ebx, [esp + 4]
  mov       ebp, [esp + 8]
  inc       eax
  dec       ebx
  inc       ebp
  mov       [esp + 8], ebx
  mov       [esp + 4], eax
  mov       [esp], ebp

  loop      encode

check:
                                  ; ecx == 0 because of the loop
  add       esp, 12
  add       ecx, 35
  pop       edi                   ; input
  lea       esi, [flag]           ; flag
  rep cmpsb
  jnz       fail

succ:
  lea       ecx, [good_msg]
  mov       edx, 12
  call      write_str

fail:
  lea       ecx, [bad_msg]
  mov       edx, 6
  call      write_str

write_str:
  xor       ebx, ebx
  inc       ebx
  xor       eax, eax
  add       eax, 4
  int       0x80
  xor       eax, eax
  inc       eax
  int       0x80

flag      db 0x5e, 0x3b, 0x21, 0x5b, 0x35, 0x32, 0x29, 0x33, 0x2b, 0x9b, 0x54, 0x50, 0x7e, 0x5b, 0x88, 0x58, 0x93, 0x86, 0x7c, 0x1e, 0x9a, 0x53, 0x7e, 0x39, 0x2a, 0x89, 0x97, 0x58, 0x90, 0x4b, 0x8b, 0x1e, 0x8e, 0x49, 0x64    ; flag
good_msg: db "You got it!", 10
bad_msg:  db "fail", 10
