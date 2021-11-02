all:
	$(CC) -c Elf_read.c
	$(CC) -c gdbstub_be.c
	$(CC) -c main.c
	$(CC) -c RVDM.c
	$(CC) -c gdbstub_dmi_stub.c
	$(CC) -c gdbstub.c
	$(CC) -c gdbstub_fe.c
	$(CC) *o -lelf -lpthread -o main
