#ifndef _PCI_H 
#define _PCI_H

#include <hwcoms/port.h>
#include <types.h>
#include <hwcoms/idt.h>

class DriverManager;
class InterruptManager;
enum class BASE_ADDRESS_REGISTER_TYPE {
        MEMORY_MAPPING = 0,
        INPUT_OUTPUT = 1
};

struct BaseAddressRegister {
    public:
        bool _prefechable; // used only in memory mapping
        uint8_t* _address;
        uint32_t _size;
        BASE_ADDRESS_REGISTER_TYPE _type; 
};

struct PCIDescriptor {
    public:
        uint32_t _portBase;
        uint32_t _interrupt;

        uint16_t _bus;
        uint16_t _device;
        uint16_t _function;

        uint16_t _vendorId;
        uint16_t _deviceId;

        uint8_t _classId;
        uint8_t _subclassId;
        uint8_t _interfaceId;

        uint8_t _revision;
};

class PCIController {
    private:
        Port32Bit _dataPort;
        Port32Bit _commandPort;
        uint32_t getId(uint8_t bus, uint16_t device, uint16_t function, uint32_t registerOffset);
        PCIDescriptor getDescriptor(uint8_t bus, uint16_t device, uint16_t function);
    public:
        PCIController();
        ~PCIController();

        /***
         * @param bus bus number
         * @param device device number
         * @param function function number
         * @param registerOffset register offset
         */
        uint32_t read(uint8_t bus, uint16_t device, uint16_t function, uint32_t registerOffset);   
        void write(uint8_t bus, uint16_t device, uint16_t function, uint32_t registerOffset, uint32_t value);
        bool hasFunctions(uint16_t bus, uint16_t device);

        void selectDrivers(DriverManager* manager, InterruptManager* interruptManager);
        /**
         * @param bus bus number
         * @param device device number
         * @param function function number
         * @param barNumber number of base address register
         */
        BaseAddressRegister getBaseAddressRegister(uint8_t bus, uint16_t device, uint16_t function, uint16_t barNumber);
        Driver* getDriver(PCIDescriptor pci, InterruptManager* manager);
};

#endif