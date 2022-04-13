/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 13:27:27 by fleitz            #+#    #+#             */
/*   Updated: 2022/04/12 15:46:04 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include "../libft/headers/ft_printf.h"
# include "../libft/headers/libft.h"

typedef struct s_data
{
	char	*cmd1;
	char	*cmd2;
	char	**arg1;
	char	**arg2;
	char	*file1;
	char	*file2;
	int		fd1;
	int		fd2;
	int		fd[2];
}			t_data;

// main.c
void	children(t_data *data, char **envp);
int		main(int argc, char **argv, char **envp);
// get_data.c
char	*get_path(char **arg, char **envp);
char	*path_cmd(char **envp, char **arg);
int		get_path2(int i, char **path, char **path2, char *arg);
int		file1(t_data *data, char **argv, char **envp);
int		file2(t_data *data, char **argv, char **envp);
// end.c
void	ft_strcat2(char *dest, char *str, char *str2);
void	ft_close(t_data *data);
void	end(t_data *data);
char	*free_char_tab(char **str);

#endif
