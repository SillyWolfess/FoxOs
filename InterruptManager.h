#ifndef _INTERRUPT_MANAGER_H
#define _INTERRUPT_MANAGER_H
#include "types.h"
#include "port.h"
#include "terminal.h"
#include "gdt.h"

class InterruptManager {
    Terminal* terminal;
protected:
    struct GateDescriptor{
        uint16_t handlerAddressLowBits;
        uint16_t gdt_codeSegmentSelector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handlerAddressHightBits;
    } __attribute__((packed));

    static GateDescriptor interruptDescriptorTable[256];

    struct interruptDescriptorTablePointer
    {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed));

    static void SetIdtEntries(uint8_t number, uint16_t codeSegmentOffset, void (*handler)(), uint8_t accessRights, uint8_t descriptorType);

    Port8BitSlow picMasterCommand;
    Port8BitSlow picMasterData;
    Port8BitSlow picSlaveCommand;
    Port8BitSlow picSlaveData;

public:

    InterruptManager(GlobalDescriptorTable *gdt, Terminal *terminal);
    ~InterruptManager();

    void activate();

    static uint32_t handle(uint8_t number, uint32_t esp);

    static void ignore();
    static void request0x01();
    static void request0x00();
};
#endif
