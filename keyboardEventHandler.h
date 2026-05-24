#ifndef _KEYBOARD_EVENT_HANDLER_H
#define _KEYBOARD_EVENT_HANDLER_H

#include "eventHandler.h"
#include "keyEvent.h"

class KeyboardEventHandler : public EventHandler {
    public:
        void handle(KeyEvent);
        const char* getKeySring(uint8_t);
};

#endif