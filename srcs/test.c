#include <unistd.h>
#include <stdio.h>
#include "../libft/headers/ft_printf.h"
#include "../libft/headers/libft.h"
#include <sys/wait.h>

int	main(void)
{
	int	fd[2];
	int	tab[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int	id;
	int	i;
	int	j;
	int	i2;

	i = 0;
	i2 = 0;
	if (pipe(fd) == -1)
	{
		ft_printf("Error pipe\n");
		return (0);
	}
	id = fork();
	if (id == 0)
	{
		close(fd[0]);
		j = (sizeof(tab) / sizeof(int)) / 2;
		while (i < j)
		{
			i2 += tab[i];
			i++;
		}
		write(fd[1], &i2, sizeof(i));
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		j = (sizeof(tab) / sizeof(int));
		i2 = (sizeof(tab) / sizeof(int)) / 2;
		while (i2 < j)
		{
			i += tab[i2];
			i2++;
		}
		wait(NULL);
		read(fd[0], &i2, sizeof(i));
		i += i2;
		close(fd[0]);
		ft_printf("i = %d\n", i);
	}
	return (1);
}

/*
int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int	id;
	int	i;

	i = 0;
	while (i++ < 2)
	{
		id = fork();
		if (id == 0)
			break ;
	}
	if (id == 0)
		ft_printf("Hello from fork !\n");
	while (wait(NULL) != -1)
		i++;
	ft_printf("Hello !\n");
	return (0);
}
*/