
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

	printf("Hi, this is myapp1!\nForking into background now and exit after %d seconds.\n", secs);

	pid_t result = fork();

	if (result == -1)
	{
		fprintf(stderr, "Failed to fork: %s.", strerror(errno));
		return 1;
	}
	else if (result == 0)
	{
		// Child process sleeps some seconds before exit.
		sleep(secs);
	}

	return 0;
}
