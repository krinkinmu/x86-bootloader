LD ?= ld
CC ?= gcc

CFLAGS = -Wall -g -ffreestanding -Os \
	-march=i386 -m16 -mregparm=3

all: bootloader.bin

OBJS = \
	header.o \
	setup.o \
	console.o \
	utils.o \
	string.o \
	bioscall.o \
	regs.o \
	memory.o \
	printf.o

image: bootloader.bin
	dd if=/dev/zero of=image bs=512c count=4
	dd conv=notrunc if=bootloader.bin of=image

bootloader.bin: $(OBJS) boot.ld
	$(LD) --oformat binary -T boot.ld $(OBJS) -o bootloader.bin

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o bootloader.bin image

.PHONY: clean
