#include <hwcoms/pci.h>
#include <drivers/driver.h>
#include <managers/driverManager.h>
#include <hwcoms/idt.h>

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

void PCIController::selectDrivers(DriverManager* manager, InterruptManager* interruptManager) {
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int nFunction = hasFunctions(bus, device) ? 8 : 1;
            for (int function = 0; function < nFunction; function++) {
                PCIDescriptor pci = getDescriptor(bus, device, function);

                if (pci._vendorId == 0x0000 || pci._vendorId == 0xFFFF) {
                    continue;
                }

                for (int barNum = 0; barNum < 6; barNum++) {
                    BaseAddressRegister bar = getBaseAddressRegister(bus, device, function, barNum);
                    if (bar._address && (bar._type == BASE_ADDRESS_REGISTER_TYPE::INPUT_OUTPUT)) {
                        pci._portBase = (uint32_t) bar._address;

                        Driver* driver = getDriver(pci, interruptManager);
                        if (driver != 0) {
                            manager->add(driver);
                        }
                    }
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

BaseAddressRegister PCIController::getBaseAddressRegister(uint8_t bus, uint16_t device, uint16_t function, uint16_t barNumber) {
    BaseAddressRegister result;

    uint32_t headerType = read(bus, device, function, 0x0E) & 0x7F;
    int maxBARS = 6 - (4 * headerType);
    if (barNumber >= maxBARS) {
        return result;
    }

    uint32_t value = read(bus, device, function, 0x10 + (4 * barNumber));
    result._type = (value & 0x1) ? BASE_ADDRESS_REGISTER_TYPE::INPUT_OUTPUT : BASE_ADDRESS_REGISTER_TYPE::MEMORY_MAPPING;
    
    uint32_t tmp;

    if (result._type == BASE_ADDRESS_REGISTER_TYPE::MEMORY_MAPPING) {
        result._prefechable = ((value >> 3) & 0x01) == 0x01;
        //TODO lowlevel.eu pci
        switch ((value >> 1) & 0x3) {
            case 0: break;// 32 bit mode
            case 1: break;// 20 bit mode
            case 2: break;// 64 bit mode
            default: break;
        }
    } else if (result._type == BASE_ADDRESS_REGISTER_TYPE::INPUT_OUTPUT) {
        result._address  = (uint8_t*) (value & ~0x3);
        result._prefechable = false;
    }

    return result;
}

Driver* PCIController::getDriver(PCIDescriptor pci, InterruptManager* manager) {
    /*
    switch (pci._vendorId) {
        case 0x8086: { // intel
            switch (pci._deviceId) {
                
            }
        } break;
        
    }

    switch (pci._classId) {
        case 0x03: // graphics
            switch (pci._subclassId) {
                case 0x00: // vga
                break;
            }
        break; 
    }
    */
    return 0;
}
