#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

class Terminal {
public:
    /* Hardware text mode color constants. */
    enum vga_color {
        VGA_COLOR_BLACK = 0,
        VGA_COLOR_BLUE = 1,
        VGA_COLOR_GREEN = 2,
        VGA_COLOR_CYAN = 3,
        VGA_COLOR_RED = 4,
        VGA_COLOR_MAGENTA = 5,
        VGA_COLOR_BROWN = 6,
        VGA_COLOR_LIGHT_GREY = 7,
        VGA_COLOR_DARK_GREY = 8,
        VGA_COLOR_LIGHT_BLUE = 9,
        VGA_COLOR_LIGHT_GREEN = 10,
        VGA_COLOR_LIGHT_CYAN = 11,
        VGA_COLOR_LIGHT_RED = 12,
        VGA_COLOR_LIGHT_MAGENTA = 13,
        VGA_COLOR_LIGHT_BROWN = 14,
        VGA_COLOR_WHITE = 15,
    };

    uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
    uint16_t vga_entry(unsigned char uc, uint8_t color);
    size_t strlen(const char* str);

    #define VGA_WIDTH   80
    #define VGA_HEIGHT  25
    #define VGA_MEMORY  0xB8000

    size_t terminal_row;
    size_t terminal_column;
    uint8_t terminal_color;
    uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

    void initialize(void);
    void setcolor(uint8_t color);
    void putentryat(char c, uint8_t color, size_t x, size_t y);
    void putchar(char c);
    void write(const char* data, size_t size);
    void writestring(const char* data);
    void writeHex8(uint8_t);
    void writeHex16(uint16_t);

    static Terminal* s_terminal;
};
#endif
