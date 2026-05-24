#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

#include "types.h"
#include "event.h"

class EventHandler {
    public:
        virtual void handle(Event) { };
};

#endif