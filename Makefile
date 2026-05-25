objects = boot/boot.o terminal.o gdt.o hwcoms/port.o hwcoms/idtstub.o hwcoms/idt.o drivers/keyboard.o drivers/driver.o managers/driverManager.o managers/eventManager.o events/event.o events/keyEvent.o handlers/eventHandler.o  handlers/keyboardEventHandler.o kernel.o

%.o: %.cpp
	i686-elf-g++ -m32 -I. -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -c -o $@ $<

%.o: %.s
	i686-elf-as --32 -o $@ $<

FoxOs: linker.ld ${objects}
	i686-elf-gcc -ffreestanding -O2 -nostdlib -lgcc -fno-exceptions -fno-rtti -T $< -o $@ ${objects}

build_iso:
	cp FoxOs isodir/boot/FoxOs 
	cp boot/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o FoxOs.iso isodir

copy_iso:
	cp FoxOs.iso /media/sf_FoxOs/FoxOs.iso
	cp FoxOs /media/sf_FoxOs/FoxOs.bin

deploy_iso: build_iso copy_iso

all: FoxOs build_iso copy_iso

run_vm_debug:
	qemu-system-i386 -kernel FoxOs -no-reboot -no-shutdown -d int

run_vm:
	qemu-system-i386 -kernel FoxOs -no-reboot -no-shutdown

.PHONY: clean
clean:
	rm -f $(objects) FoxOs FoxOs.iso *.o
