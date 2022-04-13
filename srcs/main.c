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
