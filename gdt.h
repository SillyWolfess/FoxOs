#ifndef _GDT_H_
#define _GDT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

class Terminal;
class GlobalDescriptorTable {

public:
    struct entry {
        uint16_t limitLow;
        uint16_t baseLow;
        uint8_t baseMiddle;
        uint8_t access;
        uint8_t flags;
        uint8_t baseHigh;
    } __attribute__((packed));
    struct ptr {
        uint16_t size;
        uint32_t offset;
    } __attribute__((packed));

    entry _entries[3];
    ptr _ptr;

    Terminal *_terminal;

    entry setEntry(uint32_t base, uint32_t limit, uint8_t acess, uint8_t flags);
public:
    GlobalDescriptorTable(Terminal*);
    ~GlobalDescriptorTable();
    void set();
};

#endif
