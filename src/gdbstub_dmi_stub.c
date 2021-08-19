// Copyright (c) 2016-2020 Bluespec, Inc. All Rights Reserved
// Author: Rishiyur Nikhil

// ================================================================
// Stub implementation of DMI read/write functions

// ================================================================
// C lib includes

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <sys/mman.h>
#include <sys/types.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// ----------------
// Project includes

#include  "RVDM.h"

// ================================================================
// DMI interface (gdbstub invokes these functions)
// These should be filled in with the appropriate mechanisms that
// perform the actual DMI read/write on the RISC-V Debug module.

static uint32_t *map_base;
static int fd;
static int opened = 0;

static void
mem_open(void)
{
	uint64_t map_size;
	uint64_t target;
	int reg;

	printf("%s\n", __func__);

	fd = open("/dev/mem", O_RDWR | O_SYNC);

	map_size = 4096;
	target = 0xf9000000;

	map_base = mmap(0, map_size, PROT_READ | PROT_WRITE,
	    MAP_SHARED, fd, target);

	printf("%s: map_base %p\n", __func__, map_base);

	//fflush(stdout);
	//*(volatile uint8_t *)(map_base) = 11;

	//printf("%s: trying to access reg...\n", __func__);

	//reg = *(volatile uint32_t *)(map_base + 0x10);
	//printf("%s: reg %x\n", __func__, reg);

	//munmap(map_base, map_size);
	//close(fd);

	/* Reset */
	*(volatile uint32_t *)(map_base + 0x10) = 0x80000003;
	sleep(1);
	*(volatile uint32_t *)(map_base + 0x10) = 0x80000001;
}

void
dmi_write(uint16_t addr, uint32_t data)
{
	uint32_t data1;

	if (!opened) {
		opened = 1;
		mem_open();
	}

	if (addr == 0x10) {
		data1 = 0x00000003;
		printf("%s: addr %x: data %x\n", __func__, addr, data1);
		//*(volatile uint32_t *)(map_base + addr) = data1;
	}

	printf("%s: addr %x: data %x\n", __func__, addr, data);
	*(volatile uint32_t *)(map_base + addr) = data;
}

uint32_t
dmi_read(uint16_t addr)
{
	uint32_t reg;

	if (!opened) {
		opened = 1;
		mem_open();
	}

	reg = *(volatile uint32_t *)(map_base + addr);

	//if (reg != 0 && addr != 0x11)
	//	printf("%s: addr %x, val %x\n", __func__, addr, reg);

	return (reg);
}
