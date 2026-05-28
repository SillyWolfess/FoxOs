#include <terminal.h>
#include <gdt.h>
#include <hwcoms/idt.h>
#include <hwcoms/pci.h>
// drivers
#include <drivers/keyboard.h>
// managers
#include <managers/driverManager.h>
#include <managers/eventManager.h>
#include <managers/taskManager.h>
// handlers
#include <handlers/keyboardEventHandler.h>

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "Needs to be compiled with a ix86-elf compiler"
#endif

extern "C"  void kernel_main(void)
{
	/* Initialize terminal interface */
	Terminal terminal;
	Terminal::s_terminal = &terminal;
	terminal.initialize();

	GlobalDescriptorTable gdt;
	gdt.set();

	TaskManager taskManager;

	InterruptManager idt(&gdt, &taskManager);
	idt.set();
	/*init other drivers*/
	EventManager eventManager;
	eventManager.set();

	KeyboardEventHandler keyboardEventHandler;
	keyboardEventHandler.logEvents(true);
	eventManager.addKeyHandler(&keyboardEventHandler);

	DriverManager driverManager;
	KeyboardDriver keyboard(&idt, &eventManager);
	keyboard.setLog(true);

	driverManager.set();
	// register drivers
	driverManager.add(&keyboard);

	PCIController pciController;
	pciController.selectDrivers(&driverManager, &idt);

	// activate drivers
	driverManager.activate();

	terminal.enableCursor();
	/*activate interrupts*/
	idt.activate();

	while(1);
}

