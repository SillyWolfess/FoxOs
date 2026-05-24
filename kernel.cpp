#include "terminal.h"
#include "gdt.h"
#include "idt.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

extern "C"  void kernel_main(void)
{
	Terminal terminal;
	Terminal::s_terminal = &terminal;
	/* Initialize terminal interface */
	terminal.initialize();

	terminal.writestring("creating gdt\n");
	GlobalDescriptorTable gdt;
	gdt.set();

	terminal.writestring("creating interrupts\n");
	InterruptManager interrupts(&gdt);
	interrupts.set();

	/* Newline support is left as an exercise. */
	terminal.writestring("Meow from the class\n");
	terminal.writestring("Some other text\n");

	interrupts.activate();
	terminal.writestring("Interrupts activated\n");
	while(1);
}

