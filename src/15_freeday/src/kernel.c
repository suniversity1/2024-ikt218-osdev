#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include <multiboot2.h>
#include "gdt.h"


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};



int kernel_main();

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    
    init_gdt();
    write_string( 0x0F, "Hello, World!" );
    write_char( 0x0F, '\n' );
    write_string( 0x0F, "This is a new line." );
    // Call cpp kernel_main (defined in kernel.cpp)
    return kernel_main();
}