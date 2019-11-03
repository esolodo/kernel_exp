
ENTRY(_start)

SECTIONS {
    . = 1M;

    .boot  :
    {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }
    .init  ALIGN(4096):
    {
        PROVIDE(__start = .);
        *(.init)
    }
    .init.bss ALIGN(4096) :
    {
        *(.init.bss)
    }
    . = 0xC0000000;
    .text ALIGN(4096) : AT(ADDR(.text) - 0xC0000000)
    {
        *(.text)
    }
}