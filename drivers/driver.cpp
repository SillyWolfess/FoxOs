#include <drivers/driver.h>
#include <terminal.h>

Driver::Driver() {

}

Driver::~Driver() {

}

void Driver::activate() {
    Terminal::s_terminal->writestring("Driver activated");
}