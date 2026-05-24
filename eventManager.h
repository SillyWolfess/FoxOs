#ifndef _EVENT_MANAGER_H
#define _EVENT_MANAGER_H

#include "types.h"
#include "eventHandler.h"

#define MAX_EVENT_HANDLERS 256
class EventManager {
    EventHandler* _handlers[MAX_EVENT_HANDLERS];
    int _nHandlers;
    public:
        EventManager();
        ~EventManager();
        void set();
        void add(EventHandler* handler);
        void event(Event);
};

#endif