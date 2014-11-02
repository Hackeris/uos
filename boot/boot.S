	#define ASM_FILE        1
     #include <multiboot.h>
     
     #ifdef HAVE_ASM_USCORE
     # define EXT_C(sym)                     _ ## sym
     #else
     # define EXT_C(sym)                     sym
     #endif
     
     #define STACK_SIZE                      0x4000
     
     #ifdef __ELF__
     # define MULTIBOOT_HEADER_FLAGS         0x00000003
     #else
     # define MULTIBOOT_HEADER_FLAGS         0x00010003
     #endif
             .text
     
             .globl  start, _start
     start:
     _start:
             jmp     multiboot_entry
     
             /* Align 32 bits boundary. */
             .align  4
     
             /* Multiboot header. */
     multiboot_header:
             /* magic */
             .long   MULTIBOOT_HEADER_MAGIC
             /* flags */
             .long   MULTIBOOT_HEADER_FLAGS
             /* checksum */
             .long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
     #ifndef __ELF__
             /* header_addr */
             .long   multiboot_header
             /* load_addr */
             .long   _start
             /* load_end_addr */
             .long   _edata
             /* bss_end_addr */
             .long   _end
             /* entry_addr */
             .long   multiboot_entry
     #endif /* ! __ELF__ */
     
     multiboot_entry:
             /* Initialize the stack pointer. */
             movl    $(stack + STACK_SIZE), %esp
     
             /* Reset EFLAGS. */
             pushl   $0
             popf
     
             /* Push the pointer to the Multiboot information structure. */
             pushl   %ebx
             /* Push the magic value. */
             pushl   %eax
     
             /* Now enter the C main function... */
             call    EXT_C(kmain)
     
             /* Halt. */
             pushl   $halt_message
             call    EXT_C(kprintf)
     
     loop:   hlt
             jmp     loop
     
     halt_message:
             .asciz  "Halted."
     
             /* Our stack area. */
             .comm   stack, STACK_SIZE
