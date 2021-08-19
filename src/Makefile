all:
	cc -c Elf_read.c
	cc -c gdbstub_be.c
	cc -c main.c
	cc -c RVDM.c
	cc -c gdbstub_dmi_stub.c
	cc -c gdbstub.c
	cc -c gdbstub_fe.c
	cc *o -lelf -lpthread -o main
