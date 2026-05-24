#include "InterruptManager.h"
#include "terminal.h"

InterruptManager::GateDescriptor InterruptManager::idt[256];
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
    uint16_t codeSegmentOffset,
    void (*handler)(),
    uint8_t accessRights,
    uint8_t descriptorType
) {

    const uint8_t IDT_DESC_PRESENT = 0x80;

    idt[number].handlerAddressLowBits = (uint16_t)(((uint32_t)handler) & 0xFFFF);
    idt[number].selector = codeSegmentOffset;
    idt[number].reserved = 0;
    idt[number].access = IDT_DESC_PRESENT | descriptorType | ((accessRights&3) << 5);
    idt[number].handlerAddressHightBits = (uint16_t)((((uint32_t)handler) >> 16) & 0xFFFF);
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
    uint16_t CodeSegment = 0x08;// gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for (uint16_t i = 0; i < 256; i++) {
        SetIdtEntries(i, CodeSegment, &ignore, 0, IDT_INTERRUPT_GATE);
    }

    SetIdtEntries(0x20, CodeSegment, &request0x00, 0, IDT_INTERRUPT_GATE);
    SetIdtEntries(0x21, CodeSegment, &request0x01, 0, IDT_INTERRUPT_GATE);
}

void InterruptManager::loadIdt() {
    _idtp.size = (256 * (sizeof(GateDescriptor))) - 1;
    _idtp.base = (uint32_t) idt;

    //    asm volatile("lidt %0": : "m" (_idtp): "memory");
    asm volatile("lidt %[idt]": : [idt] "m" (_idtp) );
}

void InterruptManager::set() {
    setHandlers();
    loadIdt();
    restartPICs();
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
