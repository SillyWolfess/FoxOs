#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include <events/event.h>

enum class KEY_EVENT {UP, DOWN};
class KeyEvent: public Event {
    private:
        uint8_t _code;
        KEY_EVENT _action;
    public:
        KeyEvent(uint8_t code, KEY_EVENT action);
        bool isUp();
        bool isDown();
        uint8_t getCode() { return _code; }
};
#endif