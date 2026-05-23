objects = boot.o kernel.o port.o InterruptManagertubs.o InterruptManager.o terminal.o gdt.o

%.o: %.cpp
	i686-elf-g++ -m32 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -c -o $@ $<

%.o: %.s
	i686-elf-as --32 -o $@ $<

FoxOs: linker.ld ${objects}
	i686-elf-gcc -ffreestanding -O2 -nostdlib -lgcc -T $< -o $@ ${objects}

build_iso:
	cp FoxOs isodir/boot/FoxOs 
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o FoxOs.iso isodir

copy_iso:
	cp FoxOs.iso /media/sf_FoxOs/FoxOs.iso

deploy_iso: build_iso copy_iso

all: FoxOs build_iso copy_iso

.PHONY: clean
clean:
	rm -f $(objects) FoxOs FoxOs.iso
