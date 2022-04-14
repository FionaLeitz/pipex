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

// get path for the command
char	*path_cmd(char **envp, char **arg)
{
	int		i;
	char	**path;
	char	**path2;
	char	*cmd;

	i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (check_cmd(i, arg[0], envp) == 0)
		return (ft_strdup(arg[0]));
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
		return (free_char_tab(path2));
	cmd = ft_strdup(path2[i]);
	free_char_tab(path2);
	return (cmd);
}

void	children2(t_data *data, char **envp, int id, int id2)
{
	if (id != 0 && id2 == 0)
	{
		close(data->fd[1]);
		dup2(data->fd[0], 0);
		dup2(data->fd1, 1);
		execve(data->cmd2, data->arg2, envp);
		close(data->fd[0]);
		close(data->fd1);
		if (data->fd2 != -1)
			close(data->fd2);
		write(2, "zsh: command not found: ", 25);
		write(2, data->cmd2, ft_strlen(data->cmd2));
		write(2, "\n", 1);
		return ;
	}
	close(data->fd[0]);
	close(data->fd[1]);
	waitpid(id, NULL, 0);
	waitpid(id2, NULL, 0);
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
		if (data->fd2 != -1)
		{
			execve(data->cmd1, data->arg1, envp);
			close(data->fd2);
			write(2, "zsh: command not found: ", 25);
			write(2, data->cmd1, ft_strlen(data->cmd1));
			write(2, "\n", 1);
		}
		close(data->fd[1]);
		return ;
	}
	children2(data, envp, id, id2);
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
