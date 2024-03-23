#include "pipex.h"

void	exit_with_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

int	get_descriptor(char *file_name, char c)
{
	int	descriptor;

	descriptor = -1;
	if (c == 'h') // for here_doc
		descriptor = open(file_name,  O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (c == 'o') // for OUT
		descriptor = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (c == 'i') // for IN
		descriptor = open(file_name, O_RDONLY, 0777);
	else
		exit_with_error("Wrong Option!\n");
	if (descriptor == -1)
		exit_with_error("File opening Error!\n");
	return (descriptor);
}