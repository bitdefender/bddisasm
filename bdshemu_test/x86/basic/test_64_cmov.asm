    bits 64
    
    global _start
    
    section .text
    
_start:
    xor     eax, eax
    mov     rcx, 0xffffffffffffffff
    mov     rbx, 0x0000000011111111
  
    cmovnz  ecx, ebx
    
    int3