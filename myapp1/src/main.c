
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
	printf("Hi, this is myapp1! Forking to background now and exit in one minute.\n");

	pid_t result = fork();

	if (result == -1)
	{
		fprintf(stderr, "Failed to fork: %s.", strerror(errno));
		return 1;
	}
	else if (result == 0)
	{
		// Just sleep a minute and exit the daemon.
		sleep(60);
	}
	else
	{
		// Exit parent progess.
		return 0;
	}
}
