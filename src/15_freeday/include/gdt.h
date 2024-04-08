#include <libc/stdint.h>

void init_gdt();


// Define a GDT entry. This structure is packed to prevent the compiler from changing its layout.
struct gdt_entry {
    uint16_t limit_low;           // The lower 16 bits of the limit.
    uint16_t base_low;            // The lower 16 bits of the base.
    uint8_t base_middle;          // The next 8 bits of the base.
    uint8_t access;               // Access flags, determine what ring this segment can be used in.
    uint8_t granularity;
    uint8_t base_high;            // The last 8 bits of the base.
} __attribute__((packed));

// Define a pointer to the GDT array.
struct gdt_ptr {
    uint16_t limit;               // The upper 16 bits of all selector limits.
    uint32_t base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed));