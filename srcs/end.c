/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 14:24:41 by fleitz            #+#    #+#             */
/*   Updated: 2022/04/11 14:24:50 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/pipex.h"

// strcat
void	ft_strcat2(char *dest, char *str, char *str2)
{
	dest[0] = '\0';
	ft_strcat(dest, str);
	ft_strcat(dest, "/");
	ft_strcat(dest, str2);
}

// close fd
void	ft_close(t_data *data)
{
	if (data->fd[0] != -1)
		close(data->fd[0]);
	if (data->fd[1] != -1)
		close(data->fd[1]);
}

// close and free in the end
void	end(t_data *data)
{	
	if (data->arg1 != NULL)
		free_char_tab(data->arg1);
	if (data->arg2 != NULL)
		free_char_tab(data->arg2);
	if (data->cmd1 != NULL)
		free(data->cmd1);
	if (data->cmd2 != NULL)
		free(data->cmd2);
	if (data->file1 != NULL)
		free(data->file1);
	if (data->file2 != NULL)
		free(data->file2);
}

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
