#include <hwcoms/pci.h>
#include <drivers/driver.h>
#include <managers/driverManager.h>

#include <terminal.h>

PCIController::PCIController():
_dataPort(0xCFC),
_commandPort(0xCF8)
{

}

PCIController::~PCIController() {

}

uint32_t PCIController::getId(uint8_t bus, uint16_t device, uint16_t function, uint32_t registerOffset) {
    uint32_t id = 
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registerOffset & 0xFC);
    return id;
}

uint32_t PCIController::read(uint8_t bus, uint16_t device, uint16_t function, uint32_t registerOffset) {
    uint32_t id = getId(bus, device, function, registerOffset);
    _commandPort.write(id);
    uint32_t result = _dataPort.read();
    return result >> (8* (registerOffset % 4));
}

void PCIController::write(uint8_t bus, uint16_t device, uint16_t function, uint32_t registerOffset, uint32_t value) {
    uint32_t id = getId(bus, device, function, registerOffset);
    _commandPort.write(id);
    _dataPort.write(value);
} 

bool PCIController::hasFunctions(uint16_t bus, uint16_t device) {
    return read(bus, device, 0, 0x0E) & (1<<7); // 1<<7 read 7th bit
}

PCIDescriptor PCIController::getDescriptor(uint8_t bus, uint16_t device, uint16_t function) {
    PCIDescriptor result;

    result._bus = bus;
    result._device = device;
    result._function = function;

    result._vendorId = read(bus, device, function, 0x00);
    result._deviceId = read(bus, device, function, 0x02);

    result._classId = read(bus, device, function, 0x0B);
    result._subclassId = read(bus, device, function, 0x0A);
    result._interfaceId = read(bus, device, function, 0x09);

    result._revision = read(bus, device, function, 0x08);
    result._interrupt = read(bus, device, function, 0x3C);

    return result;
}

void PCIController::selectDrivers(DriverManager* manager) {
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int nFunction = hasFunctions(bus, device) ? 8 : 1;
            for (int function = 0; function < nFunction; function++) {
                PCIDescriptor pci = getDescriptor(bus, device, function);

                if (pci._vendorId == 0x0000 || pci._vendorId == 0xFFFF) {
                    continue;
                }

                Terminal::s_terminal->writestring("PCI BUS ");
                Terminal::s_terminal->writeHex8w0(pci._bus & 0xFF);

                Terminal::s_terminal->writestring(" ,DEVICE ");
                Terminal::s_terminal->writeHex8w0(pci._device & 0xFF);

                Terminal::s_terminal->writestring(" ,FUNCTION ");
                Terminal::s_terminal->writeHex8w0(pci._function & 0xFF);

                Terminal::s_terminal->writestring(" ,VENDOR ID ");
                Terminal::s_terminal->writeHex8w0((pci._vendorId & 0xFF00) >> 8);
                Terminal::s_terminal->writeHex8w0(pci._vendorId & 0xFF);
                Terminal::s_terminal->writestring(" ,DEVICE ID ");
                Terminal::s_terminal->writeHex8w0((pci._deviceId & 0xFF00) >> 8);
                Terminal::s_terminal->writeHex8w0(pci._deviceId & 0xFF);
                Terminal::s_terminal->writestring("\n");
            }
        }
    }
}
