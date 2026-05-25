#ifndef _EVENT_MANAGER_H
#define _EVENT_MANAGER_H

#include <events/event.h>

class EventHandler;
class KeyboardEventHandler;
#define MAX_EVENT_HANDLERS 256
class EventManager {
    EventHandler* _handlers[MAX_EVENT_HANDLERS];
    int _nHandlers;
    KeyboardEventHandler* _keyHandler[MAX_EVENT_HANDLERS];
    int _nKeyHandler;
    public:
        EventManager();
        ~EventManager();
        void set();
        void add(EventHandler* handler);
        void addKeyHandler(KeyboardEventHandler* handler);
        void event(Event);
        void keyEvent(KeyEvent);
};

#endif
