#include <unistd.h>
#include <stdio.h>
#include "../libft/headers/ft_printf.h"
#include "../libft/headers/libft.h"
#include <sys/wait.h>

char	*free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

char	*get_path(char **arg, char **envp)
{

	int	i;
	char **path;
	char **path2;
	char *cmd;

	execve(arg[0], arg, envp);
	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	path = ft_split(&envp[i][5], ':');
	if (path == NULL)
		return (NULL);
	i = 0;
	while (path[i] != NULL)
		i++;
	path2 = malloc(sizeof(char *) * (i + 1));
	if (path2 == NULL)
		return (free_split(path));
	path2[i] = NULL;
	i -= 1;
	while (i >= 0)
	{
		path2[i] = malloc(sizeof(char) * (ft_strlen(path[i]) + ft_strlen(arg[0]) + 3));
		if (path2[i] == NULL)
		{
			free_split(path);
			free_split(path2);
			return (NULL);
		}
		path2[i][0] = '\0';
		ft_strcat(path2[i], path[i]);
		ft_strcat(path2[i], "/");
		ft_strcat(path2[i], arg[0]);
		i--;
	}
	i = 0;
	while (access(path2[i], F_OK|X_OK) == -1)
	{
		if (path2[i] == NULL)
			return (0);
		i++;
	}
	cmd = ft_strdup(path2[i]);
	free_split(path);
	free_split(path2);
	return (cmd);
}

int	main(int argc, char **argv, char **envp)
{
	int	id;
	int	id2;
	char 	*cmd;
	char	**arguments;

	if (argc == 1)
		return (0);
	arguments = ft_split(argv[1], ' ');
	if (arguments == NULL)
		return (0);
	cmd = get_path(arguments, envp);
	if (cmd == NULL)
	{
		free_split(arguments);
		return (0);
	}
	id = fork();
	if (id == 0)
		execve(cmd, arguments, envp);
	wait(NULL);
	free_split(arguments);
	free(cmd);
	arguments = ft_split(argv[2], ' ');
	if (arguments == NULL)
		return (0);
	cmd = get_path(arguments, envp);
	if (cmd == NULL)
	{
		free_split(arguments);
		return (0);
	}
	if (id != 0)
		id2 = fork();
	if (id != 0 && id2 == 0)
		execve(cmd, arguments, envp);
	wait(NULL);
	free_split(arguments);
	free(cmd);
	return (0);
}

/*
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