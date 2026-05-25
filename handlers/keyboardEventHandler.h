#ifndef _KEYBOARD_EVENT_HANDLER_H
#define _KEYBOARD_EVENT_HANDLER_H

#include <handlers/eventHandler.h>
#include <events/keyEvent.h>

class KeyboardEventHandler : public EventHandler {
    private:
        bool _logEvents;
    public:
        KeyboardEventHandler();
        ~KeyboardEventHandler();
        void handle(KeyEvent);
        void logEvents(bool);
        const char* getKeyString(uint8_t);
};

#endif
