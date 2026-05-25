#include <events/keyEvent.h>

KeyEvent::KeyEvent(uint8_t code, KEY_EVENT action)
{
    _code = code;
    _action = action;
    _data = code;
}

bool KeyEvent::isUp() {
    return _action == KEY_EVENT::UP;
}
bool KeyEvent::isDown() {
    return _action == KEY_EVENT::DOWN;
}
