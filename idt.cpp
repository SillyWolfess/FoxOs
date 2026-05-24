#include "idt.h"
#include "terminal.h"
#include "bitMacros.h"

InterruptManager::idtEntry InterruptManager::idt[256];
InterruptManager* InterruptManager::activeManager = 0;

uint32_t InterruptManager::handle(uint8_t number, uint32_t esp) {
    if (activeManager != 0) {
       return activeManager->doHandle(number, esp);
    }
    Terminal::s_terminal->writestring("no active manager\n");
    return esp;
}

uint32_t InterruptManager::doHandle(uint8_t number, uint32_t esp) {
    char *foo = "INTERRUPT 0x00\n";
    char *hex = "0123456789ABCDEF";
    foo[12] = hex[(number >> 4) & 0x0f];
    foo[13] = hex[number & 0x0f];
    Terminal::s_terminal->writestring(foo);

    if (0x20 <= number && number < 0x30) {
        Terminal::s_terminal->writestring("sending 0x20 to master\n");
        picMasterCommand.write(0x20);
        if (0x28 <= number) {
            Terminal::s_terminal->writestring("sending 0x20 to slave\n");
            picSlaveCommand.write(0x20);
        }
    }

    Terminal::s_terminal->writestring("returning from doHandle\n");
    return esp;
}

void InterruptManager::SetIdtEntries(
    uint8_t number,
    uint32_t base,
    uint16_t selector,
    uint8_t flags
) {

    const uint8_t IDT_DESC_PRESENT = 0x80;

    idt[number].baseLow = base & READ_16BITS;
    idt[number].baseHigh = (base >> 16) & READ_16BITS;
    idt[number].selector = selector;
    idt[number].reserved = 0;
    idt[number].flags = flags | 0x60;
    
}

InterruptManager::InterruptManager(GlobalDescriptorTable *gdt)
:picMasterCommand(0x20),
 picMasterData(0x21),
 picSlaveCommand(0xA0),
 picSlaveData(0xA1)
{
};

void InterruptManager::restartPICs() {
    // Restart both PICs
    picMasterCommand.write(0x11);
    picSlaveCommand.write(0x11);

    picMasterData.write(0x20); // start at 32
    picSlaveData.write(0x28); // start at 40

    // cascading for PICs
    picMasterData.write(0x04);
    picSlaveData.write(0x02);

    //8086 mode for PICs
    picMasterData.write(0x01);
    picSlaveData.write(0x01);

    // activate IRQs
    picMasterData.write(0x00);
    picSlaveData.write(0x00);
}

void InterruptManager::setHandlers() {
    const uint16_t CodeSegment = 0x08;
    const uint8_t IDT_INTERRUPT_GATE = 0x8E;
    for (uint16_t i = 0; i < 256; i++) {
        SetIdtEntries(i, CodeSegment, (uint32_t)ignore, IDT_INTERRUPT_GATE);
    }

    SetIdtEntries(0x20, CodeSegment, (uint32_t)request0x00, IDT_INTERRUPT_GATE);
    SetIdtEntries(0x21, CodeSegment, (uint32_t)request0x01, IDT_INTERRUPT_GATE);
}

void InterruptManager::loadIdt() {
    _idtp.size = (256 * (sizeof(idtEntry))) - 1;
    _idtp.base = (uint32_t) idt;

    //    asm volatile("lidt %0": : "m" (_idtp): "memory");
    asm volatile("lidt %[idt]": : [idt] "m" (_idtp) );
}

void InterruptManager::set() {
    setHandlers();
    restartPICs();
    loadIdt();
    Terminal::s_terminal->writestring("Interrupt manager set\n");
}

void InterruptManager::activate() {
    if (activeManager != 0) {
        activeManager->deactivate();
    }
    Terminal::s_terminal->writestring("Activating interrupt manager\n");
    activeManager = this;
    asm("sti");
}

void InterruptManager::deactivate() {
    if (activeManager == this) {
        Terminal::s_terminal->writestring("Deactivating interrupt manager\n");
        activeManager = 0;
        asm("cli");
    }
}

InterruptManager::~InterruptManager() {

};
