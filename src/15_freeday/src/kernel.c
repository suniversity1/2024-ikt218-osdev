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

#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

volatile unsigned char *video = (volatile unsigned char *)VIDEO_MEMORY;
int cursor_x = 0;
int cursor_y = 0;

void scroll()
{
    if (cursor_y >= SCREEN_HEIGHT)
    {
        // Move all lines up by one
        for (int y = 1; y < SCREEN_HEIGHT; y++)
        {
            for (int x = 0; x < SCREEN_WIDTH; x++)
            {
                unsigned short index = (y * SCREEN_WIDTH + x) * 2;
                unsigned short next_index = ((y - 1) * SCREEN_WIDTH + x) * 2;
                video[next_index] = video[index];
                video[next_index + 1] = video[index + 1];
            }
        }

        // Clear the last line
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            unsigned short index = ((SCREEN_HEIGHT - 1) * SCREEN_WIDTH + x) * 2;
            video[index] = ' ';
            video[index + 1] = 0x07; // Default color attribute
        }

        cursor_y = SCREEN_HEIGHT - 1;
    }
}

void update_cursor()
{
    unsigned short position = (cursor_y * SCREEN_WIDTH) + cursor_x;
    unsigned short offset = position * 2;

    // Write to VGA hardware ports to update cursor position
    asm volatile("mov %0, %%dx; mov %1, %%eax; out %%al, %%dx; inc %%dx; mov $0x3d5, %%dx; mov %%al, %%dh; dec %%dx; out %%al, %%dx" : : "g"(0x3d4), "g"(14));
    asm volatile("mov %0, %%dx; mov %1, %%eax; out %%al, %%dx; inc %%dx; mov $0x3d5, %%dx; mov %%ah, %%dl; dec %%dx; out %%al, %%dx" : : "g"(0x3d4), "g"(offset >> 8));

    asm volatile("mov %0, %%dx; mov %1, %%eax; out %%al, %%dx; inc %%dx; mov $0x3d5, %%dx; mov %%al, %%dh; dec %%dx; out %%al, %%dx" : : "g"(0x3d4), "g"(15));
    asm volatile("mov %0, %%dx; mov %1, %%eax; out %%al, %%dx; inc %%dx; mov $0x3d5, %%dx; mov %%ah, %%dl; dec %%dx; out %%al, %%dx" : : "g"(0x3d4), "g"(offset & 0xFF));
}

void write_char(int colour, char character)
{
    if (character == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else
    {
        unsigned short offset = (cursor_y * SCREEN_WIDTH + cursor_x) * 2;
        video[offset] = character;
        video[offset + 1] = colour;

        cursor_x++;
        if (cursor_x >= SCREEN_WIDTH)
        {
            cursor_x = 0;
            cursor_y++;
        }
    }

    scroll();
    update_cursor();
}

void write_string(int colour, const char *string)
{
    while (*string != 0)
    {
        write_char(colour, *string++);
    }
}


int kernel_main();

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    
    init_gdt();
    write_string( 0x0F, "Hello, World!" );
    write_char( 0x0F, '\n' );
    write_string( 0x0F, "This is a new line." );
    // Call cpp kernel_main (defined in kernel.cpp)
    return kernel_main();
}