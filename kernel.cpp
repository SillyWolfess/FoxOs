#include "terminal.h"
#include "globalDescriptorTable.h"
#include "InterruptManager.h"

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
	GlobalDescriptorTable gdt;
	InterruptManager interrupts(&gdt);

	Terminal terminal;
	/* Initialize terminal interface */
	terminal.initialize();

	/* Newline support is left as an exercise. */
	terminal.writestring("Hello, kernel Woof!\n");
	terminal.writestring("Meow from the class");

	interrupts.activate();

	while(1);
}

