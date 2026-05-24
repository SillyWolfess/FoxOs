#ifndef _DRIVER_MANAGER_H
#define _DRIVER_MANAGER_H

#include "driver.h"
#include "types.h"

#define MAX_DRIVERS 256
class DriverManager {
    Driver* _drivers[MAX_DRIVERS];
    int _nDrivers;
    public:
        DriverManager();
        ~DriverManager();
        void set();
        void add(Driver* driver);
        void activate();
};

#endif