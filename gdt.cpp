#include "gdt.h"
#include "terminal.h"

GlobalDescriptorTable::GlobalDescriptorTable()
{
    Terminal::s_terminal->writestring("terminal sent to gdt\n");
}

GlobalDescriptorTable::entry GlobalDescriptorTable::setEntry(
    uint32_t base, uint32_t limit,
    uint8_t access, uint8_t flags
) {
    entry e;
    e.access = access;
    // fill base
    e.baseLow = base & READ_16BITS;
    e.baseMiddle = (base >> 16) & READ_8BITS;
    e.baseHigh = (base  >> 24) & READ_8BITS;
    // fill flags    
    e.limitLow = limit & READ_16BITS;
    e.flags = (limit >> 16) & READ_2nd_4BITS;
    e.flags |= flags & READ_1st_4BITS;
    return e;
}


void GlobalDescriptorTable::set() {
    Terminal::s_terminal->writestring("Setting gdt\n");
    _entries[0] = setEntry(0, 0, 0, 0); // null segment
    /**
     * 0xCF with _32BITS - set up 4Kb paging for memory
     * 0x9A - acess permissions 1001 1010
     *      - [0]   = 1 - valid segment
     *      - [1,2] = 00 - highest privilage kernel
     *      - [3]   = 1 - code or data segment
     *      - [4]   = 1 - execution bit
     *      - [5]   = 0 - direction, go up in memory
     *      - [6]   = 1 - read allowed
     *      - [7]   = 0 - was accessed
     */
    _entries[1] = setEntry(0, _32BITS, 0x9A, 0xCF); // kernel code segment
    _entries[2] = setEntry(0, _32BITS, 0x92, 0xCF); // kernel data segment
//    _entries[3] = setEntry(0, _32BITS, 0xFA, 0xCF); // user code segment
//    _entries[4] = setEntry(0, _32BITS, 0xF2, 0xCF); // user data segment

    _ptr.size = (3 * (sizeof (entry))) - 1;
    _ptr.offset = (uint32_t) _entries;

    // asm volatile("lgdt %0": : "m" (_ptr) : "memory");
    asm volatile("lgdt %[_entries]": : [_entries] "m" (_ptr) );
    Terminal::s_terminal->writestring("Gdt loaded\n");
}

GlobalDescriptorTable::~GlobalDescriptorTable() {

}
