#include <handlers/eventHandler.h>
#include <terminal.h>

void EventHandler::handle(Event e) {
    Terminal::s_terminal->writestring("Handling default event ");
    Terminal::s_terminal->writeHex8(e.getData());
};