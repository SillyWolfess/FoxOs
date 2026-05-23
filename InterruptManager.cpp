#include "InterruptManager.h"
#include "terminal.h"

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

uint32_t InterruptManager::handle(uint8_t number, uint32_t esp) {
    return esp;
}

void InterruptManager::SetIdtEntries(
    uint8_t number,
    uint16_t codeSegmentOffset,
    void (*handler)(),
    uint8_t accessRights,
    uint8_t descriptorType
) {

    const uint8_t IDT_DESC_PRESENT = 0x80;

    interruptDescriptorTable[number].handlerAddressLowBits = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[number].gdt_codeSegmentSelector = codeSegmentOffset;
    interruptDescriptorTable[number].reserved = 0;
    interruptDescriptorTable[number].access = IDT_DESC_PRESENT | descriptorType | ((accessRights&3) << 5);
    interruptDescriptorTable[number].handlerAddressHightBits = (((uint32_t) handler) >> 16) & 0xFFFF;
}

InterruptManager::InterruptManager(GlobalDescriptorTable *gdt, Terminal *t)
: picMasterCommand(0x20),
 picMasterData(0x21),
 picSlaveCommand(0xA0),
 picSlaveData(0xA1)
{
    terminal = t;
    uint16_t CodeSegment = 0x08;// gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for (uint16_t i = 0; i < 256; i++) {
        SetIdtEntries(i, CodeSegment, &ignore, 0, IDT_INTERRUPT_GATE);
    }

    SetIdtEntries(0x20, CodeSegment, &request0x00, 0, IDT_INTERRUPT_GATE);
    SetIdtEntries(0x21, CodeSegment, &request0x01, 0, IDT_INTERRUPT_GATE);

    picMasterCommand.write(0x11);
    picSlaveCommand.write(0x11);

    picMasterData.write(0x20);
    picSlaveData.write(0x28);

    picMasterData.write(0x04);
    picSlaveData.write(0x02);

    picMasterData.write(0x01);
    picSlaveData.write(0x01);

    picMasterData.write(0x00);
    picSlaveData.write(0x00);

    interruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t) interruptDescriptorTable;

    asm volatile("lidt %0": : "m" (idt));

    terminal->writestring("Interrupt manager set\n");

};

void InterruptManager::activate() {
    asm("sti");
}

InterruptManager::~InterruptManager() {

};
