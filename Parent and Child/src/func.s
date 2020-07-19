.intel_syntax noprefix
.text
.code32
.globl check

check:
  push    ebp             
  mov     ebp, esp
strlen:
  mov     ecx, -1
  xor     eax, eax
  mov     edi, [ebp + 8]
  mov     esi, [ebp + 8]  
  repne scasb
  not     ecx
  dec     ecx
  sub     ecx, 39
  mov     eax, 0x1337
  lea     ebx,  [fail]
  lea     edx,  [encode]
  int3             
  
encode:
  mov     eax, 0x7fce2001
  and     eax, 0x7ffe3001
  add     eax, 0xfffaffff
  shr     eax, 0x00000001
  xor     eax, 0x0ae5d541
  not     eax
  int3

  mov     dl, al
  lodsb
  mov     bl, al

  push    0xcdbccdfe
  pop     eax
  not     eax
  add     eax, 0xe0000000
  xor     eax, 0xedbccdfe
  and     eax, 0x12433201
  add     eax, 0xdeadbeef
  int3

  mov     bl, al
  mov     dl, 0x69

  xor     eax, eax
  inc     eax
  shr     eax, 9
  not     eax
  not     eax
  not     eax
  xor     eax, 0x96969696
  int3

  mov     bl, al
  mov     dl, 0x13

  mov     eax, 0x7ffff7ff
  xor     eax, 0x00001800
  and     eax, 0x22021201
  shr     eax, 0x00000001
  add     eax, 0xf0000001
  not     eax
  int3

  test    al, al
  mov     eax, 0x1337
  lea     ebx, [fail]
  lea     edx, [pass]
  int3
pass:
  inc     ecx
  cmp     ecx, 39
  mov     eax, 0x1337
  lea     ebx, [encode]
  lea     edx, [success]
  int3
success:
  xor     eax, eax
  jmp     exit
fail:
  mov     eax, -1
exit:
  mov     esp, ebp        
  pop     ebp
  ret
