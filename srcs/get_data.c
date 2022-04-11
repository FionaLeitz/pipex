/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 14:25:02 by fleitz            #+#    #+#             */
/*   Updated: 2022/04/11 14:25:06 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/pipex.h"

// in get_path, all the paths possible for the command we want
int	get_path2(int i, char **path, char **path2, char *arg)
{
	int	size;

	size = ft_strlen(arg);
	path2[i] = NULL;
	while (--i >= 0)
	{
		path2[i] = malloc(sizeof(char) * (ft_strlen(path[i]) + size + 2));
		if (path2[i] == NULL)
		{
			free_char_tab(path);
			free_char_tab(path2);
			return (-1);
		}
		path2[i][0] = '\0';
		ft_strcat(path2[i], path[i]);
		ft_strcat(path2[i], "/");
		ft_strcat(path2[i], arg);
	}
	while (access(path2[++i], F_OK | X_OK) == -1)
	{
		if (path2[i] == NULL)
		{
			ft_printf("zsh: command not found: %s\n", arg);
			return (-1);
		}
	}
	free_char_tab(path);
	return (i);
}

// used to get the path of a command if it exist
// if not, return NULL
char	*get_path(char **arg, char **envp)
{
	int		i;
	char	**path;
	char	**path2;
	char	*cmd;

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
	i = get_path2(i, path, path2, arg[0]);
	if (i == -1)
		return (NULL);
	cmd = ft_strdup(path2[i]);
	free_char_tab(path2);
	return (cmd);
}

// used to get the path of a file
char	*get_file(char *filename, char **envp)
{
	int		i;
	char	*path;
	char	*file;

	if ((ft_strcmp(filename, "/dev/stdin") == 0)
		|| (ft_strcmp(filename, "/dev/stdout") == 0))
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

// used to get the datas for the first file
int	file1(t_data *data, char **argv, char **envp)
{
	data->file1 = get_file(argv[1], envp);
	if (access(data->file1, F_OK | R_OK) == -1)
	{
		ft_printf("zsh: %c%s: %s\n", ft_tolower(strerror(errno)[0]),
			&strerror(errno)[1], argv[1]);
		return (0);
	}
	data->fd2 = open(data->file1, O_RDONLY);
	data->arg1 = ft_split(argv[2], ' ');
	if (data->arg1 == NULL)
		return (0);
	data->cmd1 = get_path(data->arg1, envp);
	if (data->cmd1 == NULL)
	{
		free_char_tab(data->arg1);
		return (0);
	}
	data->file2 = get_file(argv[4], envp);
	data->fd1 = open(data->file2, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (data->fd1 == -1)
	{
		ft_printf("zsh: %c%s: %s\n", ft_tolower(strerror(errno)[0]),
			&strerror(errno)[1], argv[4]);
		free_char_tab(data->arg1);
		free(data->cmd1);
		free(data->file2);
		return (0);
	}
	return (1);
}

// used to get the datas for the second file
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
		free_char_tab(data->arg1);
		free(data->cmd1);
		return (0);
	}
	return (1);
}
