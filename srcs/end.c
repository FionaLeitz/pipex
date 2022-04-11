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

// close and free in the end
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
