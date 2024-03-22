#include "pipex.h"

// ./pipex f1 cmd1 =>cmd2<= =>f2<=
static void	second_child_process(char **argv, char **envp, int *pipe)
{
	int	descriptor;

	// O_CREAT - create if doesn't exist
	// O_TRUNC - empty it if exists
	descriptor = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (descriptor == -1)
		exit_with_error("File opening Error!\n");
	if (close(pipe[1]) == -1)
		exit_with_error("Pipe close Error!\n");
	if (dup2(pipe[0], STDIN_FILENO) == -1)
		exit_with_error("Descriptor Duplication Error!\n");
	if (dup2(descriptor, STDOUT_FILENO) == -1)
		exit_with_error("Descriptor Duplication Error!\n");
	if (close(pipe[0]) == -1)
		exit_with_error("Pipe close Error!\n");
	cmd_execute(argv[3], envp);
}

// ./pipex =>f1<= =>cmd1<= cmd2 f2
static void	first_child_process(char **argv, char **envp, int *pipe)
{
	int	descriptor;

	descriptor = open(argv[1], O_RDONLY, 0777);
	if (descriptor == -1)
		exit_with_error("File opening Error!\n");
	if (close(pipe[0]) == -1)
		exit_with_error("Pipe close Error!\n");
	if (dup2(pipe[1], STDOUT_FILENO) == -1)
		exit_with_error("Descriptor Duplication Error!\n");
	if (dup2(descriptor, STDIN_FILENO) == -1)
		exit_with_error("Descriptor Duplication Error!\n");
	if (close(pipe[1]) == -1)
		exit_with_error("Pipe close Error!\n");
	cmd_execute(argv[2], envp);
}

static void	start_pipex(char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid1; // we could use simple 'int', but pid_t will handle for every type of OS
	pid_t	pid2;
	int		status;

	if (pipe(fd) == -1)
		exit_with_error("Pipe set Error!\n");
	pid1 = fork();
	pid2 = fork();
	if (pid1 == -1 || pid2 == -1)
		exit_with_error("Process fork Error!\n");
	if (pid1 == 0 && pid2 != 0)
		first_child_process(argv, envp, fd);
	else
		if (pid1 != 0 && pid2 == 0)
			second_child_process(argv, envp, fd);
	close(fd[0]);
	close(fd[1]);
	if (!(pid1 == 0 && pid2 == 0))
	{
		if (waitpid(pid1, &status, 0) == -1)
			exit_with_error("Waitpid Error!\n");
		if (status != 0)
			exit(EXIT_FAILURE);
		if (waitpid(pid2, &status, 0) == -1)
			exit_with_error("Waitpid Error!\n");
		if (status != 0)
			exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv, char **envp)
{
	if (argc == 5)
		start_pipex(argv, envp);
	else
		ft_putstr_fd("Wrong Argument Rules!\n", 2);
	return (0);
}
