
#include <stdio.h>

int main(int argc, char **argv)
{
	printf("Hi, this is example2.\n");

#if defined(FOO)
	printf("Feature foo was selected.\n");
#endif

#if defined(BAR)
	printf("Feature bar was selected.\n");
#endif
}
