#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#include "idt.h"
#include "port.h"

class KeyboardDriver: public InterruptHandler {
    Port8Bit _dataPort;
    Port8Bit _commandPort;
    void log(uint8_t);
public:
    KeyboardDriver(InterruptManager* manager);
    ~KeyboardDriver();
    void set();
    virtual uint32_t handle(uint32_t esp);
};

#endif