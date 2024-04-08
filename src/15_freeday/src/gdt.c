#include "gdt.h"
#include <libc/stdint.h> 

#define NUM_GDT_ENTRIES 5
struct gdt_entry gdt_entries[NUM_GDT_ENTRIES];
struct gdt_ptr gdt_pointer;

extern void gdt_flush(uint32_t);

// Function to set the value of a single GDT entry.
void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

// Function to initialize the GDT.
void init_gdt() {
    gdt_pointer.limit = (sizeof(struct gdt_entry) * NUM_GDT_ENTRIES) - 1;
    gdt_pointer.base  = (uint32_t)&gdt_entries;

    gdt_set_entry(0, 0, 0, 0, 0);                // Null segment
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment


    
    gdt_flush((uint32_t)&gdt_pointer);
}

