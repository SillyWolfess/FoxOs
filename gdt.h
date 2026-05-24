#ifndef _GDT_H_
#define _GDT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define READ_4BITS_8FORM 0xF0
#define READ_8BITS 0xFF
#define READ_16BITS 0xFFFF
#define READ_8BITS_24FORM 0xFF0000
#define READ_8BITS_32FORM 0xFF000000
#define READ_1st_4BITS 0xF0;
#define READ_2nd_4BITS 0x0F;
#define _32BITS 0xFFFFFFFF
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
