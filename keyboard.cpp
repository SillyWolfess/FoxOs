#include "keyboard.h"
#include "terminal.h"

KeyboardDriver::KeyboardDriver(InterruptManager *manager)
:InterruptHandler(0x21, manager),
_dataPort(0x60),
_commandPort(0x64)
{

}

void KeyboardDriver::set() {
    // wait for ports to be empty
    while (_commandPort.read() & 0x1) {
        _dataPort.read();
    }
    _commandPort.write(0xAE); // activate interrupts
    _commandPort.write(0x20); // get current state
    uint8_t status = (_dataPort.read() | 1) & ~0x10;
    _commandPort.write(0x60); // set state
    _dataPort.write(status);

    _dataPort.write(0xF4);
    Terminal::s_terminal->writestring("Keyboard set\n");
}

KeyboardDriver::~KeyboardDriver() {

}

void KeyboardDriver::log(uint8_t key) {
    Terminal::s_terminal->writestring("KEYBOARD ");
    Terminal::s_terminal->writeHex8(key);
    Terminal::s_terminal->writestring("\n");
}

uint32_t KeyboardDriver::handle(uint32_t esp) {
    uint8_t key = _dataPort.read();
    log(key);
    return esp;
}