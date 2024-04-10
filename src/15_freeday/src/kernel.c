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

    char* World = "World!";
    init_gdt();
    my_printf("Hello,  %s\n%d", World, 50+75);
    // Call cpp kernel_main (defined in kernel.cpp)
    return kernel_main();
}