#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../libft/headers/ft_printf.h"
#include "../libft/headers/libft.h"
#include <sys/wait.h>

// /dev/stdin ou /dev/stdout !!

// used to free a char **
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

// used to get the path of a command if it exist
// if not, return NULL
char	*get_path(char **arg, char **envp)
{
	int	i;
	char **path;
	char **path2;
	char *cmd;

	if (access(arg[0], F_OK) != -1)
		return (arg[0]);
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
		path2[i] = malloc(sizeof(char) * (ft_strlen(path[i]) + ft_strlen(arg[0]) + 2));
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

// used to get the path of a file
char	*get_file(char *filename, char **envp)
{
	int	i;
	char	*path;
	char	*file;

	if (ft_strcmp(filename, "/dev/stdin") == 0)
	{
		file = ft_strdup(filename);
		return (file);
	}
	else if (ft_strcmp(filename, "/dev/stdout") == 0)
	{
		file = ft_strdup(filename);
		return (file);
	}
	i = 0;
	while (ft_strncmp(envp[i], "PWD=", 4) != 0)
		i++;
	path = ft_strdup(&envp[i][4]);
	file = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(filename) + 2));
	file[0] = '\0';
	ft_strcat(file, path);
	ft_strcat(file, "/");
	ft_strcat(file, filename);
	free(path);
	return (file);
}

// used to create a char ** with the command and its arguments
char	**make_arg(char **arg, char *file)
{
	char	**cpy;
	int		i;

	if (arg == NULL)
		return (NULL);
	i = 0;
	while (arg[i] != NULL)
		i++;
	cpy = malloc(sizeof(char *) * (i + 2));
	cpy[i + 1] = NULL;
	i = 0;
	while (arg[i] != NULL)
	{
		cpy[i] = ft_strdup(arg[i]);
		i++;
	}
	if (!(ft_strcmp(file, "/dev/stdin") == 0 || ft_strcmp(file, "/dev/stdout") == 0))
		cpy[i] = file;
	else
		cpy[i] = NULL;
	free_split(arg);
	return (cpy);
}

/* ce que j'ai a faire.
*/



int	main(int argc, char **argv, char **envp)
{
	int		id;
	int		id2;
	char 	*cmd;
	char	**arguments;
	char 	*cmd2;
	char	**arguments2;
	char	*file1;
	char	*file2;
	int		fd[2];
	int		fd1;

	if (argc <= 2)
		return (0);
	file1 = get_file(argv[1], envp);
	if (access(file1, F_OK|R_OK) == -1)
	{
		ft_printf("No access\n");
		return (0);
	}
	arguments = ft_split(argv[2], ' ');
	arguments = make_arg(arguments, file1);
	if (arguments == NULL)
		return (0);
	cmd = get_path(arguments, envp);
	if (cmd == NULL)
	{
		free_split(arguments);
		return (0);
	}

	file2 = get_file(argv[argc - 1], envp);

//	if (access(file1, F_OK) != -1)
	fd1 = open(file2, O_CREAT|O_RDWR);
	if (fd1 == -1)
	{
		ft_printf("c'est la merde\n");
		free_split(arguments);
		free(cmd);
		free(file2);
		return (0);
	}
/*	else
	{
		fd1 = open(file2, O_WRONLY|O_RDONLY);
	}*/
	
	arguments2 = ft_split(argv[3], ' ');
	if (arguments2 == NULL)
		return (0);
	cmd2 = get_path(arguments2, envp);
	if (cmd2 == NULL)
	{
		free_split(arguments2);
		return (0);
	}

	if (pipe(fd) == -1)
	{
		ft_printf("Error pipe\n");
		free_split(arguments);
		free(cmd);
		return (0);
	}

	id = fork();
	if (id != 0)
		id2 = fork();
	if (id == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		execve(cmd, arguments, envp);
		close(fd[1]);
	}
	else if (id2 == 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		dup2(fd1, 1);
		execve(cmd2, arguments2, envp);
		close(fd[0]);
	}

	waitpid(0, NULL, 0);
	close(fd[0]);
	close(fd[1]);
	close(fd1);
	free_split(arguments2);
	free(cmd2);
	free_split(arguments);
	free(cmd);
	free(file2);
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
*/