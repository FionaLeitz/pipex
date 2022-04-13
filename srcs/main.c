/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 13:25:16 by fleitz            #+#    #+#             */
/*   Updated: 2022/04/12 12:13:16 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/pipex.h"

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
	while (envp[i] != NULL && (ft_strncmp(envp[i], "PWD=", 4) != 0))
		i++;
	if (i == 0)
		return (NULL);
	path = ft_strdup(&envp[i][4]);
	file = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(filename) + 2));
	ft_strcat2(file, path, filename);
	free(path);
	return (file);
}

// do the forks
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

// main
int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc != 5)
		return (0);
	data.cmd1 = NULL;
	data.cmd2 = NULL;
	data.arg1 = NULL;
	data.arg2 = NULL;
	data.file1 = NULL;
	data.file2 = NULL;
	if (file1(&data, argv, envp) == 0)
	{
		end(&data);
		return (0);
	}
	if (file2(&data, argv, envp) == 0)
	{
		end(&data);
		return (0);
	}
	children(&data, envp);
	end(&data);
	ft_close(&data);
	return (0);
}
