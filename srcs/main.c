#include "../pipex.h"

// /dev/stdin ou /dev/stdout !!

// used to free a char **
char	*free_char_tab(char **str)
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
		return (free_char_tab(path));
	path2[i] = NULL;
	i -= 1;
	while (i >= 0)
	{
		path2[i] = malloc(sizeof(char) * (ft_strlen(path[i]) + ft_strlen(arg[0]) + 2));
		if (path2[i] == NULL)
		{
			free_char_tab(path);
			free_char_tab(path2);
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
	free_char_tab(path);
	free_char_tab(path2);
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

int	file1(t_data *data, char **argv, char **envp)
{
	data->file1 = get_file(argv[1], envp);
	if (access(data->file1, F_OK|R_OK) == -1)
	{
		ft_printf("No access\n");
		return (0);
	}
	data->fd2 = open(data->file1, O_RDONLY);
	if (data->fd2 == -1)
	{
		ft_printf("error open 1\n");
		return (0);
	}
	data->arg1 = ft_split(argv[2], ' ');
	if (data->arg1 == NULL)
		return (0);
	data->cmd1 = get_path(data->arg1, envp);
	if (data->cmd1 == NULL)
	{
		free_char_tab(data->arg1);
		return (0);
	}
	data->file2 = get_file(argv[/*argc - 1*/4], envp);
	data->fd1 = open(data->file2, O_CREAT|O_RDWR|O_TRUNC, 0644);
	if (data->fd1 == -1)
	{
		ft_printf("c'est la merde\n");
		free_char_tab(data->arg1);
		free(data->cmd1);
		free(data->file2);
		return (0);
	}
	return (1);
}

void	end(t_data *data)
{
	close(data->fd[0]);
	close(data->fd[1]);
	close(data->fd1);
	close(data->fd2);
	free_char_tab(data->arg2);
	free(data->cmd2);
	free_char_tab(data->arg1);
	free(data->cmd1);
	free(data->file2);
}

int	file2(t_data *data, char **argv, char **envp)
{
	data->arg2 = ft_split(argv[3], ' ');
	if (data->arg2 == NULL)
		return (0);
	data->cmd2 = get_path(data->arg2, envp);
	if (data->cmd2 == NULL)
	{
		free_char_tab(data->arg2);
		return (0);
	}
	if (pipe(data->fd) == -1)
	{
		ft_printf("Error pipe\n");
		free_char_tab(data->arg1);
		free(data->cmd1);
		return (0);
	}
	return (1);
}

void	children(t_data *data, char **envp)
{
	int	id;
	int	id2;
	
	id = fork();
	if (id != 0)
		id2 = fork();
	if (id == 0)
	{
		close(data->fd[0]);
		dup2(data->fd2, 0);
		dup2(data->fd[1], 1);
		execve(data->cmd1, data->arg1, envp);
		close(data->fd[1]);
	}
	else if (id2 == 0)
	{
		close(data->fd[1]);
		dup2(data->fd[0], 0);
		dup2(data->fd1, 1);
		execve(data->cmd2, data->arg2, envp);
		close(data->fd[0]);
	}
	waitpid(0, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc <= 2)
		return (0);

	if (file1(&data, argv, envp) == 0)
		return (0);

	if (file2(&data, argv, envp) == 0)
		return (0);
	
	children(&data, envp);
//	chmod 644
	end(&data);
	return (0);
}
