#ifndef _GDT_H_
#define _GDT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

    static entry _entries[3];
    static ptr _ptr;

    /***
     * @param base uint32_t - usually 0
     * @param limit uint32_t
     * @param acess uint8_t
     * @param flags uint8_t
     */
    entry setEntry(uint32_t base, uint32_t limit, uint8_t acess, uint8_t flags);
public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();
    void set();
};

#endif
