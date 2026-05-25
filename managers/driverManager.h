#ifndef _DRIVER_MANAGER_H
#define _DRIVER_MANAGER_H

#include <types.h>

#define MAX_DRIVERS 256
class Driver;
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