#ifndef EVENT_H
#define EVENT_H

#include "types.h"

class Event {
    protected:
        uint8_t _data;
    public:
        uint8_t getData() { return _data; }
        void setData(uint8_t data) { _data = data; }
};
#endif