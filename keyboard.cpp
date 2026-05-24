#include "keyboard.h"
#include "terminal.h"
#include "keyMacro.h"
#include "keyEvent.h"

KeyboardDriver::KeyboardDriver(InterruptManager *manager, EventManager *evManager)
:InterruptHandler(0x21, manager),
_dataPort(0x60),
_commandPort(0x64)
{
    _evManager = evManager;
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

void KeyboardDriver::handleKey(uint8_t key, const char* data) {
    //printKey(data);
    KeyEvent event(key, KEY_EVENT::UP);
    _evManager->keyEvent(event);
}

void KeyboardDriver::printKey(const char* data) {
    Terminal::s_terminal->writestring(data);
}

void KeyboardDriver::log(uint8_t key) {
    Terminal::s_terminal->writeHex8(key);
}

uint32_t KeyboardDriver::handle(uint32_t esp) {
    uint8_t key = _dataPort.read();

    // We now handle only release key events
    if (key < 0x80) {
        return esp;
    }

    switch (key) {
        case KEY_0: handleKey(key,"0"); break;
        case KEY_1: handleKey(key,"1"); break;
        case KEY_2: handleKey(key,"2"); break;
        case KEY_3: handleKey(key,"3"); break;
        case KEY_4: handleKey(key,"4"); break;
        case KEY_5: handleKey(key,"5"); break;
        case KEY_6: handleKey(key,"6"); break;
        case KEY_7: handleKey(key,"7"); break;
        case KEY_8: handleKey(key,"8"); break;
        case KEY_9: handleKey(key,"9"); break;
        case KEY_A: handleKey(key,"a"); break;
        case KEY_B: handleKey(key,"b"); break;
        case KEY_C: handleKey(key,"c"); break;
        case KEY_D: handleKey(key,"d"); break;
        case KEY_E: handleKey(key,"e"); break;
        case KEY_F: handleKey(key,"f"); break;
        case KEY_G: handleKey(key,"g"); break;
        case KEY_H: handleKey(key,"h"); break;
        case KEY_I: handleKey(key,"i"); break;
        case KEY_J: handleKey(key,"j"); break;
        case KEY_K: handleKey(key,"k"); break;
        case KEY_L: handleKey(key,"l"); break;
        case KEY_M: handleKey(key,"m"); break;
        case KEY_N: handleKey(key,"n"); break;
        case KEY_O: handleKey(key,"o"); break;
        case KEY_P: handleKey(key,"p"); break;
        case KEY_Q: handleKey(key,"q"); break;
        case KEY_R: handleKey(key,"r"); break;
        case KEY_S: handleKey(key,"s"); break;
        case KEY_T: handleKey(key,"t"); break;
        case KEY_U: handleKey(key,"u"); break;
        case KEY_V: handleKey(key,"v"); break;
        case KEY_W: handleKey(key,"w"); break;
        case KEY_Y: handleKey(key,"y"); break;
        case KEY_Z: handleKey(key,"z"); break;
        case KEY_X: handleKey(key,"x"); break;
        default:
            log(key);
        break;
    }
    return esp;
}
