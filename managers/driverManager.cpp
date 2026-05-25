#include <managers/driverManager.h>
#include <drivers/driver.h>
#include <terminal.h>

DriverManager::DriverManager() {

}
DriverManager::~DriverManager() {

}
void DriverManager::set() {
    _nDrivers = 0;
}
void DriverManager::add(Driver* driver) {
    if (_nDrivers >= MAX_DRIVERS) {
        Terminal::s_terminal->writestring("Max number of dirvers exceeded\n");
        return;
    }
    _drivers[_nDrivers] = driver;
    _nDrivers++;
}
void DriverManager::activate() {
    for (int i = 0; i < _nDrivers; i++) {
        _drivers[i]->activate();
    }
}