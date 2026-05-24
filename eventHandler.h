#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

#include "types.h"
#include "event.h"
#include "terminal.h"

class EventHandler {
    public:
        void handle(Event e) {
            Terminal::s_terminal->writestring("Handling default event ");
            Terminal::s_terminal->writeHex8(e.getData());
        };
};

#endif
