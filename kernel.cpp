#include "terminal.h"
#include "gdt.h"
#include "idt.h"

#include "keyboard.h"

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

	terminal.writestring("creating gdt\n");
	GlobalDescriptorTable gdt;
	gdt.set();

	terminal.writestring("creating interrupts\n");
	InterruptManager idt(&gdt);
	idt.set();
	/*init other drivers*/

	KeyboardDriver keyboard(&idt);
	keyboard.set();

	/*activate interrupts*/
	idt.activate();
	terminal.writestring("Meow from the class\n");
	terminal.writestring("Some other text\n");

	while(1);
}

