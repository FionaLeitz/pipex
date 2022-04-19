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
		ft_strcat2(path2[i], path[i], arg);
	}
	i++;
	while (path2[i] && access(path2[i], F_OK | X_OK) == -1)
		i++;
	free_char_tab(path);
	if (path2[i] == NULL)
	{
		ft_printf("command not found: %s\n", arg);
		return (-1);
	}
	return (i);
}

// check if command exist if no env
int	check_cmd(int i, char *arg, char **envp)
{
	if (envp[i] == NULL)
	{
		if (access(arg, F_OK | X_OK) == -1)
			ft_printf("command not found: %s\n", arg);
		return (0);
	}
	return (1);
}

// used to get the path of a command if it exist
// if not, return NULL
char	*get_path(char **arg, char *cmd, char **envp)
{
	char	*new_cmd;

	if (arg[0] == NULL)
	{
		if (cmd[0] == ' ')
			ft_printf("command not found: %s\n", cmd);
		else
			ft_printf("permission denied:\n");
		return (NULL);
	}
	if (access(arg[0], F_OK | X_OK) != -1)
		return (ft_strdup(arg[0]));
	new_cmd = path_cmd(envp, arg);
	return (new_cmd);
}

// used to get the datas for the first file
int	file1(t_data *data, char **argv, char **envp)
{
	data->file1 = ft_strdup(argv[1]);
	data->fd2 = -1;
	if (access(data->file1, F_OK | R_OK) == -1)
		ft_printf("%c%s: %s\n", ft_tolower(strerror(errno)[0]),
			&strerror(errno)[1], argv[1]);
	else
		data->fd2 = open(data->file1, O_RDONLY);
	data->arg1 = ft_split(argv[2], ' ');
	if (data->fd2 != -1)
		data->cmd1 = get_path(data->arg1, argv[2], envp);
	if (data->cmd1 == NULL)
		cpy_arg(data);
	data->file2 = ft_strdup(argv[4]);
	data->fd1 = open(data->file2, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (data->fd1 == -1)
	{
		ft_printf("%c%s: %s\n", ft_tolower(strerror(errno)[0]),
			&strerror(errno)[1], argv[4]);
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
	if (data->arg2[0] == NULL && argv[3][0] == ' ')
	{
		free(data->arg2);
		data->arg2 = malloc(sizeof(char *) * 2);
		if (data->arg2 == NULL)
			return (0);
		data->arg2[1] = NULL;
		data->arg2[0] = ft_strdup(argv[3]);
	}
	data->cmd2 = get_path(data->arg2, argv[3], envp);
	if (data->cmd2 == NULL)
		return (0);
	if (pipe(data->fd) == -1)
		return (0);
	return (1);
}
