#include "terminal.h"

uint8_t Terminal::vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

uint16_t Terminal::vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t Terminal::strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void Terminal::initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }

    writestring("Terminal was initialized\n");
}

Terminal* Terminal::s_terminal = 0;

void Terminal::writeHex8(uint8_t n) {
    char *foo = "0x00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(n >> 8) & 0x0f];
    foo[2] = hex[(n >> 4) & 0x0f];
    foo[3] = hex[n &0x0f];
    writestring(foo);
}

void Terminal::writeHex16(uint16_t n) {
    writeHex8(n);
}

void Terminal::setcolor(uint8_t color)
{
    terminal_color = color;
}

void Terminal::putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void Terminal::putchar(char c)
{
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
        if (terminal_row >= VGA_HEIGHT) {
            terminal_row = 0;
        }
        return;
    }
    putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void Terminal::write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        putchar(data[i]);
}

void Terminal::writestring(const char* data)
{
    write(data, strlen(data));
}
