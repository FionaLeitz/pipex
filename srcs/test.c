#include <unistd.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int	id;

//	if (argc == 1)
//		return (1);
	id = fork();
	if (id != 0)
		sleep(1);
	if (id == 0)
		printf("Hello number 2\n");
	printf("Hello from id : %d\n", id);
	return (0);
}
