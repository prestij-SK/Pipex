#ifndef PIPEX_H
# define PIPEX_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "get_next_line.h"

typedef struct Vars
{
	int	file_in;
	int	file_out;
	int	arg_idnex;
} 	PipexVars;

/*
	libft_funcs.c
*/
size_t	ft_strlen(const char *s);
void	ft_free_split(char **mat);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);

/*
	cmd_execute.c
*/
void	cmd_execute(char *cmd, char **envp);

/*
	utils.c
*/
void	exit_with_error(char *str);
int		get_descriptor(char *file_name, char c);

/*
	process_handler.c
*/
void	here_doc(char *limiter, int argc);
void	child_processes(int argc, char **argv, char **envp, PipexVars *p_vars);

#endif