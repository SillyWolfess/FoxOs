#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <types.h>
#include <idt.h>
#include <port.h>
#include <drivers/driver.h>
class EventManager;

class KeyboardDriver: public InterruptHandler, public Driver {
    Port8Bit _dataPort;
    Port8Bit _commandPort;
    EventManager* _evManager;
    void log(uint8_t);
public:
    KeyboardDriver(InterruptManager* manager, EventManager* evManager);
    ~KeyboardDriver();
    void set();
    void activate() { set(); }
    void printKey(const char* data);
    void handleKey(uint8_t key, const char* data);
    virtual uint32_t handle(uint32_t esp);
};

#endif