
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char **argv)
{
	int secs = 20;

	if (argc == 2) {
		secs = atoi(argv[1]);
	}

	printf("Hi, this is example3!\n");
	printf("Run in foreground and exit after %d seconds.\n", secs);

	sleep(secs);

	return 0;
}
