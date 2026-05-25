#include <terminal.h>

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

void Terminal::clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void Terminal::initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    clear();
    writestring("1234567890\nqwertyuiop\nasdfghjkl\nzxcvbnm\n");

    writeHex8(0x01); writestring(" ");
    writeHex8(0x02); writestring(" ");
    writeHex8(0x03); writestring(" ");
    writeHex8(0x04); writestring(" ");
    writeHex8(0x05); writestring(" ");
    writeHex8(0x06); writestring(" ");
    writeHex8(0x07); writestring(" ");
    writeHex8(0x08); writestring(" ");
    writeHex8(0x09); writestring(" ");
    writeHex8(0x0A); writestring(" ");
    writeHex8(0x0B); writestring(" ");
    writeHex8(0x0C); writestring(" ");
    writeHex8(0x0D); writestring(" ");
    writeHex8(0x0E); writestring(" ");
    writeHex8(0x0F); writestring(" ");
    writestring("\n");

    writeHex8(0x10); writestring(" ");
    writeHex8(0x20); writestring(" ");
    writeHex8(0x30); writestring(" ");
    writeHex8(0x40); writestring(" ");
    writeHex8(0x50); writestring(" ");
    writeHex8(0x60); writestring(" ");
    writeHex8(0x70); writestring(" ");
    writeHex8(0x80); writestring(" ");
    writeHex8(0x90); writestring(" ");
    writeHex8(0xA0); writestring(" ");
    writeHex8(0xB0); writestring(" ");
    writeHex8(0xC0); writestring(" ");
    writeHex8(0xD0); writestring(" ");
    writeHex8(0xE0); writestring(" ");
    writeHex8(0xF0); writestring(" ");
    writestring("\n");
}

Terminal* Terminal::s_terminal = 0;

void Terminal::writeHex8(uint8_t n) {
    char *hex = "0123456789ABCDEF";
    putchar('0'); putchar('x');
    putchar(hex[(n >> 4) & 0x0F]);
    putchar(hex[n & 0x0F]);

}

void Terminal::writeBin8(uint8_t n) {
    for (int i = 0; i < 8; i++) {
        if ((n >> (7 - i)) & 0x01) {
            putchar('1');
        } else {
            putchar('0');
        }
    }
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
        ++terminal_row;
        terminal_column = 0;
        if (terminal_row > VGA_HEIGHT) {
            terminal_row = 0;
        //    clear();
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
