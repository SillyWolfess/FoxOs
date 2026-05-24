#include "eventManager.h"

#include "terminal.h"
EventManager::EventManager() {

}
EventManager::~EventManager() {

}
void EventManager::set() {
    _nHandlers = 0;
    Terminal::s_terminal->writestring("Event manager set\n");
}
void EventManager::add(EventHandler* handler) {
    if (_nHandlers >= MAX_EVENT_HANDLERS) {
        Terminal::s_terminal->writestring("Max number of event handlers exceeded\n");
        return;
    }
    _handlers[_nHandlers] = handler;
    _nHandlers++;
}

void EventManager::event(Event e) {
    for (int i = 0; i < _nHandlers; i++) {
        _handlers[i]->handle(e);
    }
}
