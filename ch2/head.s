global _start

section .init
bits 32

global _start:function _start.end-_start
_start: mov     ebp, eax        ; temp use esp to stash the multiboot magic...
        mov     eax, pd         ; Set up a page directory
        mov     dword [eax], pt + 3 ; addrs 0x0..0x400000 = pt | WRITE | PRESENT
        mov     dword [eax+0xC00], pt + 3 ; addrs 0xC0000000..0xC0400000 = same

        ;; Loop through all 1024 pages in page table 'pt', setting them to be
        ;; identity mapped with full permissions.
        mov     edx, pt
        mov     ecx, 0          ; Loop induction variable: start at 0

.loop:  mov     eax, ecx        ; tmp = (%ecx << 12) | WRITE | PRESENT
        shl     eax, 12
        or      eax, 3
        mov     [edx+ecx*4], eax ; pt[ecx * sizeof(entry)] = tmp

        inc     ecx
        cmp     ecx, 1024       ; End at %ecx == 1024
        jnz     .loop

        mov     eax, pd+3       ; Load page directory | WRITE | PRESENT
        mov     cr3, eax        ; Store into cr3.
        mov     eax, cr0
        or      eax, 0x80000000 ; Set PG bit in cr0 to enable paging.
        mov     cr0, eax

        jmp     higherhalf
.end:

section .init.bss nobits
pd:     resb    0x1000          ; Page directory
pt:     resb    0x1000          ; Page table

extern show

;; Note that we're now defining functions in the normal .text section,
;; which means we're linked in the higher half (based at 3GB).
section .text
global higherhalf:function higherhalf.end-higherhalf
higherhalf:
        mov     eax, ebp        ; put multiboot magic back into eax so ebp can be used properly!
        mov     esp, stack      ; Ensure we have a valid stack.
        xor     ebp, ebp        ; Zero the frame pointer for backtraces.
        call    show          ; Call second-stage loader.
        cli                     ; Kernel has finished, so disable interrupts ...
        hlt                     ; ... And halt the processor.
.end:

section .bss
align 8192
global stack_base
stack_base:
        resb    0x2000
stack:
