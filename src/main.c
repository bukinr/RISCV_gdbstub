#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "gdbstub_fe.h"

static FILE *a;
static int fd;

int main (int argc, char **argv)
{
	Gdbstub_FE_Params params;
	int newsockfd;
	socklen_t clilen;
	struct sockaddr_in cli_addr;


    // TODO: listen on socket/PTY/... for GDB connection
    // params.gdb_fd = file descriptor for socket/PTY/...

    // TODO: open logfile to record gdbstub communications w. GDB and Debug Module
    // params.logfile = ...

	fd = socket(PF_INET, SOCK_STREAM, 0);

	int ret;
	int r;
	int nonblocking;
	int port;
	struct sockaddr_in sockaddr;

	nonblocking = 0;
	port = 81;
	char* envport = getenv("RISCV_GDB_STUB_PORT");
	if (envport) port = atoi (envport);

	ret = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, nonblocking ? (ret | O_NONBLOCK) :
	    (ret & ~O_NONBLOCK));

	a = fopen("/tmp/logfile","w");
	params.logfile = a;

	printf("test\n");

	memset (&sockaddr, '\000', sizeof sockaddr);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	r = bind(fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	if (r == -1) {
		printf("Failed to bind GDB stub socket.");
		return (-1);
	}

	r = listen(fd, 0);
	if (r == -1) {
		printf("Failed to listen on GDB stub socket");
		return (-1);
	}

	printf("Waiting for a connection on socket\n");
	newsockfd = accept(fd, (struct sockaddr *)&cli_addr, &clilen);
	if (newsockfd < 0) {
		printf("Failed to connect");
		return (-1);
	}
	printf("Connected\n");
	params.gdb_fd = newsockfd;
	params.stop_fd = -1;

	main_gdbstub (& params);
}
