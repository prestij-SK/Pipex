#include "pipex.h"

void	exit_with_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}