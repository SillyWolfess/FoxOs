#include "gdt.h"
#include "terminal.h"

GlobalDescriptorTable::GlobalDescriptorTable(Terminal* terminal)
{
    _terminal = terminal;
    _terminal->writestring("terminal send to gdt\n");
}

void GlobalDescriptorTable::set() {
    _terminal->writestring("Setting gdt\n");
    _entries[0] = setEntry(0, 0, 0, 0);
    _entries[1] = setEntry(0, 64*1024*1024, 0x9A, 0xCF);
    _entries[2] = setEntry(0, 64*1024*1024, 0x92, 0xCF);

    _ptr.size = (3 * (sizeof (entry))) - 1;
    _ptr.offset = (uint32_t) _entries;

    asm volatile("lgdt %0": : "m" (_ptr) : "memory");
    _terminal->writestring("Gdt loaded\n");
}

GlobalDescriptorTable::~GlobalDescriptorTable() {

}

GlobalDescriptorTable::entry GlobalDescriptorTable::setEntry(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    entry e;
    e.limitLow = limit & 0xFFFF;
    e.baseLow = base & 0xFFFF;
    e.baseMiddle |= ((base & 0xFF0000) >> 16);
    e.access = access;
    e.flags = (limit & 0xFF0000) >> 16;
    e.flags |= (flags << 4) & 0xF0;
    e.baseHigh = ((base & 0xFF000000) >> 24);
    return e;
}
