#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <types.h>
#include <hwcoms/idt.h>
#include <hwcoms/port.h>
#include <drivers/driver.h>
class EventManager;

class KeyboardDriver: public InterruptHandler, public Driver {
    Port8Bit _dataPort;
    Port8Bit _commandPort;
    EventManager* _evManager;
    bool _logs;
public:
    KeyboardDriver(InterruptManager* manager, EventManager* evManager);
    ~KeyboardDriver();
    void activate();
    /**
     * handle keyboard interruption by reading data port and raising key event
     * @param esp uint32_t
     */
    virtual uint32_t handle(uint32_t);
    void setLog(bool v) { _logs = v;}
};

#endif
