#ifndef _INTERRUPT_MANAGER_H
#define _INTERRUPT_MANAGER_H
#include "types.h"
#include "port.h"
#include "gdt.h"

#define _IDT_TIMER 0x20

class InterruptHandler {
protected:
    uint8_t number;
};

class InterruptManager {
protected:
    static InterruptManager* activeManager;

    struct idtEntry{
        uint16_t baseLow;
        uint16_t selector;
        uint8_t reserved;
        uint8_t flags;
        uint16_t baseHigh;
    } __attribute__((packed));

    struct Idtp
    {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed));

    static idtEntry idt[256];
    static Idtp _idtp;

    static void SetIdtEntries(uint8_t number, uint32_t base, uint16_t selector, uint8_t flags);

    void log(uint8_t number);

    /**
     * 0x20 8BitPort
     */
    Port8BitSlow picMasterCommand;
    /**
     * 0x21 8BitPort
     */
    Port8BitSlow picMasterData;

    /**
     * 0xA0 8BitPort
     */
    Port8BitSlow picSlaveCommand;
    /**
     * 0xA1 8BitPort
     */
    Port8BitSlow picSlaveData;

    void setHandlers();
    void restartPICs();
    void loadIdt();

public:

    InterruptManager(GlobalDescriptorTable *gdt);
    ~InterruptManager();

    void activate();
    void deactivate();
    void set();

    static uint32_t handle(uint8_t number, uint32_t esp);
    uint32_t doHandle(uint8_t number, uint32_t esp);

    static void ignore();
    static void request0x00();
    static void request0x01();
};
#endif
