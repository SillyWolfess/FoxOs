#include <drivers/keyboard.h>
#include <terminal.h>
#include <managers/eventManager.h>
#include <macros/keyMacro.h>
#include <events/keyEvent.h>

KeyboardDriver::KeyboardDriver(InterruptManager *manager, EventManager *evManager)
:InterruptHandler(0x21, manager),
_dataPort(0x60),
_commandPort(0x64)
{
    _logs = false;
    _evManager = evManager;
}

void KeyboardDriver::activate() {
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

uint32_t KeyboardDriver::handle(uint32_t esp) {
    uint8_t key = _dataPort.read();
    if (_evManager == 0) {
        return esp;
    }
    if (_logs) {
        Terminal::s_terminal->writeHex8(key);
    }
    KeyEvent event(key < 0x80 ? key + 0x80 : key, key < 0x80 ? KEY_EVENT::DOWN : KEY_EVENT::UP);
    _evManager->keyEvent(event);
    return esp;
}
