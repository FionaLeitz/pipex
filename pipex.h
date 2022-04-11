#ifndef PIPEX_H
# define PIPEX_H

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "libft/headers/ft_printf.h"
#include "libft/headers/libft.h"
#include <sys/wait.h>

typedef	struct	s_data
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
}				t_data;

char	*free_char_tab(char **str);
char	*get_path(char **arg, char **envp);
char	*get_file(char *filename, char **envp);

#endif